# -*- coding: UTF-8 -*-

import my_style
import stand_word_style

if '__main__' == __name__:
    msg = '''
功能：把word文档转为markdown文档

运行模式：
    1 -> my_style
        1) 根据Word文字颜色判断是否为标题、行代码
        2) 根据Word文字是否加粗、倾斜进行加粗和倾斜判定
        3) 根据Word“项目符号/编号”判断是否为列表
        4) 根据Word 1x1的表格判断是否为块代码
        5) 可以处理Word文档中的图片
        6) 可以处理Word文档中的超链接
        7) 只能处理docx文档，不能处理doc文档
        
    2 -> stand_word_style
        1) 根据Word标题判断是否为标题
        2) 根据Word文字是否加粗进行加粗
        3) 根据Word“项目符号/编号”判断是否为列表
        4) 根据Word文字是否倾斜判断是否为行代码
        5) 根据Word 1x1的表格判断是否为块代码
        6) 可以处理Word文档中的图片
        7) 可以处理Word文档中的超链接
        8) 只能处理docx文档，不能处理doc文档
    '''

    print(msg)

    run_mode = input('输入运行模式：\n')

    if '1' == run_mode:
        my_style.docx_2_markdown.enter_fun()
    elif '2' == run_mode:
        stand_word_style.docx_2_markdown.enter_fun()
    else:
        print('运行模式输入错误，请退出程序，并重新运行')

    input('Press any key to exit!')
