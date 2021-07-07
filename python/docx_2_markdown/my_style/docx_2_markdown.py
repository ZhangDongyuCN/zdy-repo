# -*- coding: UTF-8 -*-

import os
import shutil
import glob
import time
import docx
import yaml
import random
import re
import xml.etree.ElementTree as ET
from xml.etree import ElementTree
from docx.document import Document
from docx.oxml.table import CT_Tbl
from docx.oxml.text.paragraph import CT_P
from docx.table import _Cell, Table
from docx.text.paragraph import Paragraph
from docx.shared import RGBColor
from io import StringIO
from zipfile import ZipFile


def iter_block_items(parent):
    '''
    Yield each paragraph and table child within *parent*, in document order.
    Each returned value is an instance of either Table or Paragraph. *parent*
    would most commonly be a reference to a main Document object, but
    also works for a _Cell object, which itself can contain paragraphs and tables.
    '''
    if isinstance(parent, Document):
        parent_elm = parent.element.body
    elif isinstance(parent, _Cell):
        parent_elm = parent._tc
    else:
        raise ValueError("something's not right")

    for child in parent_elm.iterchildren():
        if isinstance(child, CT_P):
            yield Paragraph(child, parent)
        elif isinstance(child, CT_Tbl):
            yield Table(child, parent)


def find_all_hyper_link_in_docx(docx_path):
    hyperlink_dict = {}
    with ZipFile(docx_path) as zf:
        # 提取资源ID
        content = zf.read('word/document.xml').decode()
        # 这个正则表达式中有2个模式，模式是整个正则表达式，1是ID
        pattern = r'(<w:hyperlink.+?r:id="(.+?)".+?</w:hyperlink>)'

        # findall()只返回圆括号里的内容
        pairs = re.findall(pattern, content)
        content = zf.read('word/_rels/document.xml.rels').decode()

        for pair in pairs:
            # 根据ID提取对应的超链接地址
            pattern = f'<Relationship Id="{pair[1]}" .*? Target="(.+?)"'
            target = re.findall(pattern, content)[0]

            # 超链接文本可能在多个run中，连接到一起
            pattern = '<w:t>(.+?)</w:t>'
            txt = "".join(re.findall(pattern, pair[0]))
            txt = txt.replace('&lt;', '<').replace('&gt;', '>')

            # 添加到字典中
            hyperlink_dict.setdefault(txt, []).append('[' + txt + '](' + target + ')')
    return hyperlink_dict


def deal_hyperlink_text(md_text, paragraph, hyperlink_dict):
    # 通过段落xml解析文本，解析的文本包括超链接文本
    xml = str(paragraph.paragraph_format.element.xml)
    wt_list = re.findall('<w:t[\S\s]*?</w:t>', xml)
    raw_text = u''
    for wt in wt_list:
        raw_text += re.sub('<[\S\s]*?>', u'', wt)
    raw_text = raw_text.replace('&lt;', '<').replace('&gt;', '>')

    # raw文本对比md_text文本，找出缺少的超链接
    # md_text是使用block.runs方式读取的文本，该方式读取不了超链接文本
    new_md_text = []
    hyperlink_text_list = []
    ignore_char = ['`', '*', '^', '\n']
    idx_raw_text = 0
    for idx_md_text in range(len(md_text)):
        if md_text[idx_md_text] in ignore_char:
            new_md_text.append(md_text[idx_md_text])
        elif md_text[idx_md_text] == raw_text[idx_raw_text]:
            new_md_text.append(md_text[idx_md_text])
            idx_raw_text += 1
        else:
            hyperlink_text_list.append(raw_text[idx_raw_text])
            idx_raw_text += 1
            while md_text[idx_md_text] != raw_text[idx_raw_text]:
                hyperlink_text_list.append(raw_text[idx_raw_text])
                idx_raw_text += 1
            hyperlink_text_str = ''.join(hyperlink_text_list)
            hyperlink_text_list.clear()
            hyperlink_md_style = hyperlink_dict.get(hyperlink_text_str)
            if hyperlink_md_style is not None:
                new_md_text.extend(hyperlink_md_style[0])
                del hyperlink_md_style[0]
            new_md_text.append(md_text[idx_md_text])
            idx_raw_text += 1
    if idx_raw_text < len(raw_text):
        hyperlink_md_style = hyperlink_dict.get(raw_text[idx_raw_text:])
        if hyperlink_md_style is not None:
            new_md_text.extend(hyperlink_md_style[0])
            del hyperlink_md_style[0]
    return ''.join(new_md_text)


def write_paragraph(doc, block, f_md, md_save_path, config, hyperlink_dict):
    # 配置项
    h1c = config["head_1_color"]
    h2c = config["head_2_color"]
    h3c = config["head_3_color"]
    h4c = config["head_4_color"]
    h5c = config["head_5_color"]
    lcc = config["line_code_color"]
    h1n = config["head_1_#_num"]
    img_mode = config["img_mode"]
    imgs_path = config["imgs_path"]

    # 处理段落内容
    # + 通过颜色判断段落是否为标题（最多支持五级标题）
    if len(block.runs) > 0:
        rgb = block.runs[0].font.color.rgb
        if rgb == RGBColor(eval('0x' + h1c[0:2]), eval('0x' + h1c[2:4]), eval('0x' + h1c[4:6])):  # 一级标题
            text_type = 'Heading 1'
        elif rgb == RGBColor(eval('0x' + h2c[0:2]), eval('0x' + h2c[2:4]), eval('0x' + h2c[4:6])):  # 二级标题
            text_type = 'Heading 2'
        elif rgb == RGBColor(eval('0x' + h3c[0:2]), eval('0x' + h3c[2:4]), eval('0x' + h3c[4:6])):  # 三级标题
            text_type = 'Heading 3'
        elif rgb == RGBColor(eval('0x' + h4c[0:2]), eval('0x' + h4c[2:4]), eval('0x' + h4c[4:6])):  # 四级标题
            text_type = 'Heading 4'
        elif rgb == RGBColor(eval('0x' + h5c[0:2]), eval('0x' + h5c[2:4]), eval('0x' + h5c[4:6])):  # 五级标题
            text_type = 'Heading 5'
        else:
            text_type = ''
    else:
        text_type = ''

    # + 按照一定格式处理段落内容
    text = ''
    if text_type != '':  # 如果是标题
        # 拼接文本
        for run in block.runs:
            text += run.text

        # 文本写入文件
        if text_type == 'Heading 1':
            f_md.write('#' * (h1n + 0) + ' ' + text + '\n')
        elif text_type == 'Heading 2':
            f_md.write('#' * (h1n + 1) + ' ' + text + '\n')
        elif text_type == 'Heading 3':
            f_md.write('#' * (h1n + 2) + ' ' + text + '\n')
        elif text_type == 'Heading 4':
            f_md.write('#' * (h1n + 3) + ' ' + text + '\n')
        elif text_type == 'Heading 5':
            f_md.write('#' * (h1n + 4) + ' ' + text + '\n')
        else:
            raise ValueError('text_type is not right, current text_type is {}'.format(text_type))

    else:  # 如果是段落
        # 拼接文本
        for run in block.runs:
            if run.font.color.rgb == RGBColor(eval('0x' + lcc[0:2]),
                                              eval('0x' + lcc[2:4]),
                                              eval('0x' + lcc[4:6])):  # 行代码
                text += '`' + run.text + '`'
            elif run.font.bold or run.font.cs_bold:  # 加粗
                text += '**' + run.text + '**'
            elif run.font.italic or run.font.cs_italic:  # 斜体
                text += '*^' + run.text + '*^'  # 故意设置为*^，从后边代码看这样做的目的
            else:  # 普通文本
                text += run.text

        # 添加超链接
        text = deal_hyperlink_text(text, block, hyperlink_dict)

        # 项目符号/编号
        if block.style.name == 'List Paragraph' or block.paragraph_format.left_indent is not None:
            text = '- ' + text

        # 处理连续判定为加粗的文本
        text = text.replace('****', '')

        # 处理连续判定为倾斜的文本
        text = text.replace('*^*^', '')
        text = text.replace('*^', '*')

        # 处理图片
        for run in block.runs:
            xml = str(run.element.xml)
            xml_dict = dict([node for _, node in ElementTree.iterparse(StringIO(xml), events=['start-ns'])])
            root = ET.fromstring(xml)

            # Check if pic is there in the xml of the element. If yes, then extract the image data.
            if 'pic:pic' in xml:
                for pic in root.findall('.//pic:pic', xml_dict):
                    cNvPr_elem = pic.find("pic:nvPicPr/pic:cNvPr", xml_dict)
                    name_attr = cNvPr_elem.get("name")
                    blip_elem = pic.find("pic:blipFill/a:blip", xml_dict)
                    embed_attr = blip_elem.get(
                        "{http://schemas.openxmlformats.org/officeDocument/2006/relationships}embed")
                    document_part = doc.part
                    image_part = document_part.related_parts[embed_attr]
                    image_blob = image_part._blob

                    # 图片可能没有名字，需要特殊处理
                    def gen_random_name():
                        random_list = []
                        for i in range(6):
                            random_list.append(str(random.randint(0, 9)))
                        return str(int(round(time.time() * 1000000))) + '_' + ''.join(random_list) + '.png'

                    # 获取图片名字
                    if '' == name_attr:  # 如果名字为空
                        name_attr = gen_random_name()
                    elif name_attr.find('.') == -1:  # 如果只有名字，没有后缀，增加后缀
                        name_attr += '.png'

                    # 保存图片
                    if img_mode == 1:
                        dir_name = os.path.dirname(md_save_path)
                        file_name, _ = os.path.splitext(os.path.basename(md_save_path))
                        img_dir = os.path.join(dir_name, file_name + '.imgs')
                        if not os.path.exists(img_dir):
                            os.mkdir(img_dir)
                        img_path = os.path.join(img_dir, name_attr)
                        with open(img_path, 'wb') as f_img:
                            f_img.write(image_blob)
                        text = '![' + name_attr + '](' \
                               + img_path[len(dir_name) + 1:] \
                               + ')'
                    elif img_mode == 2:
                        if not os.path.exists(imgs_path):
                            os.makedirs(imgs_path)
                        img_path = os.path.join(imgs_path, name_attr)
                        with open(img_path, 'wb') as f_img:
                            f_img.write(image_blob)
                        text = '![' + name_attr + '](' + img_path + ')'
                    else:
                        raise ValueError("img_mode must be 1 or 2")

        # 文本写入文件
        f_md.write(text + '\n')


def write_table(block, f_md):
    f_md.write('```' + '\n')
    for i in range(len(block.rows)):
        f_md.write(block.cell(i, 0).text + '\n')
    f_md.write('```' + '\n')


def docx_2_markdown(docx_path, md_save_path, config):
    hyperlink_dict = find_all_hyper_link_in_docx(docx_path)

    f_md = open(file=md_save_path, mode='wt', encoding='utf-8')
    doc = docx.Document(docx_path)
    for block in iter_block_items(doc):
        if isinstance(block, Paragraph):
            write_paragraph(doc, block, f_md, md_save_path, config, hyperlink_dict)
        elif isinstance(block, Table):
            write_table(block, f_md)
    f_md.close()


def enter_fun():
    # 解析配置文件
    config = yaml.load(open('./config.yaml', 'rt', encoding='utf-8'), Loader=yaml.FullLoader)
    word_mode = config["word_mode"]
    src_path = config["src_path"]
    save_path = config["save_path"]

    if word_mode == 1:
        docx_2_markdown(src_path, save_path, config)
    if word_mode == 2:
        # 创建目录
        if os.path.exists(save_path):
            shutil.rmtree(save_path)
        os.makedirs(save_path)

        # 列出所有docx文档
        docx_files = glob.glob(os.path.join(src_path, '*.docx'))

        # docx 2 markdown
        for docx_file in docx_files:
            md_file = os.path.join(save_path, os.path.splitext(os.path.basename(docx_file))[0] + '.md')
            docx_2_markdown(docx_file, md_file, config)
