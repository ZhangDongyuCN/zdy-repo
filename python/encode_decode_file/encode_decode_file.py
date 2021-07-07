# -*- coding: UTF-8 -*-
# python 3.8.5

import os
import shutil
import base64


def encode_file(file, txt):
    with open(file, 'rb') as fin, open(txt, 'wt') as fout:
        data = fin.read()
        base64_data = base64.b64encode(data)
        fout.write(base64_data.decode())


def decode_file(txt, file):
    with open(txt, 'rt') as fin, open(file, 'wb') as fout:
        base64_data = fin.read()
        data = base64.b64decode(base64_data)
        fout.write(data)


def split_txt_file(txt, each_file_mb, save_dir):
    if os.path.exists(save_dir):
        shutil.rmtree(save_dir)
    os.makedirs(save_dir)

    with open(txt, 'r') as fin:
        i = 1
        while True:
            data = fin.read(each_file_mb * 1024 * 1024)
            with open(os.path.join(save_dir, str(i) + '.txt'), 'w') as fout:
                fout.write(data)
            i += 1
            if len(data) < each_file_mb:
                break


def merge_txt_file(txt_dir, txt):
    with open(txt, 'w') as fout:
        for i in range(1, len(os.listdir(txt_dir)) + 1):
            with open(os.path.join(txt_dir, str(i) + '.txt'), 'r') as fin:
                data = fin.read()
                fout.write(data)


#####################################################################################################################################################
if '__main__' == __name__:
    msg = '''
功能：编码、解码、分割、合并文件

运行模式：
    1：base64方式编码单个文件
    
    2：base64方式解码单个文件
    
    3：分割txt文件
    
    4：合并txt文件
    '''
    print(msg)

    run_mode = input('请输入运行模式：\n')
    if '1' == run_mode:
        file = input('输入要编码的文件路径（e.g. D:\\xx.zip）：\n')
        txt = input('输入编码文件的保存路径（e.g. D:\\xx.txt）：\n')
        encode_file(file, txt)
    elif '2' == run_mode:
        txt = input('输入编码文件的路径（e.g. D:\\xx.txt）：\n')
        file = input('输入解码后的文件保存路径（e.g. D:\\xx.zip）：\n')
        decode_file(txt, file)
    elif '3' == run_mode:
        txt = input('输入要分割的txt文件路径（e.g. D:\\xx.txt）：\n')
        each_file_mb = input('输入分割大小（单位：MB，e.g. 1）：\n')
        save_dir = input('输入分割后的txt文件保存目录（e.g. D:\\dir）：\n')
        split_txt_file(txt, int(each_file_mb), save_dir)
    elif '4' == run_mode:
        txt_dir = input('输入要合并的txt文件目录（e.g. D:\\dir）：\n')
        txt = input('输入合并后的txt文件保存路径（e.g. D:\\xx.txt）：\n')
        merge_txt_file(txt_dir, txt)
    else:
        print('运行模式输入错误！')

    print('Done!')
    input('Press any key to exit!')
