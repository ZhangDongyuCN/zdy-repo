# -*- coding: UTF-8 -*-

import modify_ext
import rename_file
import encode_file

if '__main__' == __name__:
    msg = '''
功能：文件处理器

运行模式：
    ****************************** 重命名 ******************************
    
     1 --> 更改文件后缀
    
     2 --> 文件后缀改为小写
    
     3 --> 文件后缀改为大写
     
     4 --> 驼峰文件名转下划线
     
     5 --> 下划线文件名转驼峰
    
     6 --> 根据文件修改时间重命名文件
    
     7 --> 根据文件创建时间重命名文件（仅限Windows）
    
     8 --> 根据照片拍摄时间重命名照片

    ****************************** 编解码 ******************************
    
     9 --> base64方式编码单个文件

    10 --> base64方式解码单个文件

    11 --> 分割txt文件

    12 --> 合并txt文件
    '''

    print(msg)

    run_mode = input('\n输入运行模式: \n')

    if '1' == run_mode:
        dir = input('请输入文件所在目录: \n')
        ext = input('请输入新后缀（例如：.txt）: \n')
        modify_ext.change_ext(dir, ext)

    elif '2' == run_mode:
        dir = input('请输入文件所在目录: \n')
        modify_ext.to_lower_case_ext(dir)

    elif '3' == run_mode:
        dir = input('请输入文件所在目录: \n')
        modify_ext.to_upper_case_ext(dir)

    elif '4' == run_mode:
        dir = input('请输入文件所在目录: \n')
        rename_file.hump_2_underline_for_dir(dir)

    elif '5' == run_mode:
        dir = input('请输入文件所在目录: \n')
        rename_file.underline_2_hump_for_dir()

    elif '6' == run_mode:
        dir = input('请输入文件所在目录: \n')
        format = input('请输入命名格式: (1：默认格式：%Y%m%d_%H%M%S, e.g. 20102029_202550.txt): \n')
        if '1' == format:
            format = '%Y%m%d_%H%M%S'
        rename_file.rename_file_by_modify_time(dir, format)

    elif '7' == run_mode:
        dir = input('请输入文件所在目录: \n')
        format = input('请输入命名格式: (1：默认格式：%Y%m%d_%H%M%S, e.g. 20102029_202550.txt): \n')
        if '1' == format:
            format = '%Y%m%d_%H%M%S'
        rename_file.rename_file_by_create_time(dir, format)

    elif '8' == run_mode:
        dir = input('请输入照片所在目录: \n')
        rename_file.rename_pic(dir)

    elif '9' == run_mode:
        file = input('请输入要编码的文件路径（e.g. D:\\xx.zip）：\n')
        txt = input('请输入编码文件的保存路径（e.g. D:\\xx.txt）：\n')
        encode_file.encode_file(file, txt)

    elif '10' == run_mode:
        txt = input('请输入编码文件的路径（e.g. D:\\xx.txt）：\n')
        file = input('请输入解码后的文件保存路径（e.g. D:\\xx.zip）：\n')
        encode_file.decode_file(txt, file)

    elif '11' == run_mode:
        txt = input('请输入要分割的txt文件路径（e.g. D:\\xx.txt）：\n')
        each_file_mb = input('请输入分割大小（单位：MB，e.g. 1）：\n')
        save_dir = input('请输入分割后的txt文件保存目录（e.g. D:\\dir）：\n')
        encode_file.split_txt_file(txt, int(each_file_mb), save_dir)

    elif '12' == run_mode:
        txt_dir = input('请输入要合并的txt文件目录（e.g. D:\\dir）：\n')
        txt = input('请输入合并后的txt文件保存路径（e.g. D:\\xx.txt）：\n')
        encode_file.merge_txt_file(txt_dir, txt)

    else:
        print('运行模式输入错误!')
