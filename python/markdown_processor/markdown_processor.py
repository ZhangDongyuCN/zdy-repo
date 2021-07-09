# -*- coding: UTF-8 -*-
# python = 3.8.5


import os
import shutil


def proc_single_md_img_path(md_file_path, local_imgs_storage_dir_path):
    '''
    逐个解析markdown文件中的图片名字，并从本地markdown图片库中查找图片实际路径，进行替换。

    注意：其实只是把local_imgs_storage_dir和解析出的图片名字拼成一个完整的路径，然后替换原文图片路径，
          并不是去图片库中查找，需要自己确保markdown图片在图片库中存在。
    '''

    if not os.path.isfile(md_file_path):
        raise ValueError('parameter: md_file_path must be file path')

    f_md = open(md_file_path, 'rt', encoding='utf-8')
    txt_src = f_md.read()
    txt_des = ''
    start_pos = 0
    img_pos1 = txt_src.find('![', start_pos)
    while -1 != img_pos1:
        img_pos2 = txt_src.find(']', img_pos1 + 2)
        img_pos3 = txt_src.find(')', img_pos2 + 1)
        img_name = txt_src[img_pos1 + 2:img_pos2]
        txt_des += txt_src[start_pos:img_pos2 + 1] + '(' + os.path.join(local_imgs_storage_dir_path, img_name) + ')'
        start_pos = img_pos3 + 1
        img_pos1 = txt_src.find('![', start_pos)
    txt_des += txt_src[start_pos:]
    f_md.close()

    f_md = open(md_file_path, 'wt', encoding='utf-8')
    f_md.write(txt_des)
    f_md.close()


def recursion_proc_md_img_path(dir_path, local_imgs_storage_dir_path):
    '''
    递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，逐个解析markdown文件中的图片名字，
    并从本地markdown图片库中查找图片实际路径，进行替换。

    注意：其实只是把local_imgs_storage_dir和解析出的图片名字拼成一个完整的路径，然后替换原文图片路径，
          并不是去图片库中查找，需要自己确保markdown图片在图片库中存在。
    '''

    if not os.path.isdir(dir_path):
        raise ValueError('parameter: dir_path must be dir path')

    for maindir, subdir_list, file_name_list in os.walk(dir_path):
        for file_name in file_name_list:
            file_abs_path = os.path.join(maindir, file_name)
            ext = os.path.splitext(file_abs_path)[1]
            if '.md' == ext:
                print('current process:', file_abs_path)
                proc_single_md_img_path(file_abs_path, local_imgs_storage_dir_path)


def extra_single_md_img(md_file_path, local_imgs_storage_dir_path):
    '''
    从本地markdown图片库中查找markdown文件中的图片，复制到与markdown文件同名、同路径的目录下，
    并相应修改markdown文件中的图片路径。

    注意：需要自己确保markdown图片在图片库中存在。
    '''

    if not os.path.isfile(md_file_path):
        raise ValueError('parameter: md_file_path must be file path')
    if not os.path.isdir(local_imgs_storage_dir_path):
        raise ValueError('parameter: local_imgs_storage_dir_path must be dir path')

    f_md = open(md_file_path, 'rt', encoding='utf-8')
    txt_src = f_md.read()
    txt_des = ''
    start_pos = 0
    img_pos1 = txt_src.find('![', start_pos)
    while -1 != img_pos1:
        # 创建与markdown文件同名、同路径的目录，用于保存markdown文件图片
        md_img_storage_dir_path = md_file_path.replace('.md', '.imgs')
        if not os.path.exists(md_img_storage_dir_path):
            os.makedirs(md_img_storage_dir_path)

        img_pos2 = txt_src.find(']', img_pos1 + 2)
        img_pos3 = txt_src.find(')', img_pos2 + 1)
        img_name = txt_src[img_pos1 + 2:img_pos2]
        img_file_path = os.path.join(local_imgs_storage_dir_path, img_name)
        md_img_storage_file_path = os.path.join(md_img_storage_dir_path, img_name)

        if os.path.exists(img_file_path):
            shutil.copyfile(img_file_path, md_img_storage_file_path, follow_symlinks=False)
            txt_des += txt_src[start_pos:img_pos2 + 1] + '(' \
                       + md_img_storage_file_path[len(os.path.dirname(md_file_path)) + 1:] \
                       + ')'
        else:
            print('image: "{}" not found in local images storage dir, please check in: "{}"'.format(img_file_path,
                                                                                                    md_file_path))
            txt_des += txt_src[start_pos:img_pos3 + 1]

        start_pos = img_pos3 + 1
        img_pos1 = txt_src.find('![', start_pos)

    txt_des += txt_src[start_pos:]
    f_md.close()

    f_md = open(md_file_path, 'wt', encoding='utf-8')
    f_md.write(txt_des)
    f_md.close()


def recursion_extra_md_img(dir_path, local_imgs_storage_dir_path):
    '''
    递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，从本地markdown图片库中查找markdown文件中的图片，
    复制到与markdown文件同名、同路径的目录下，并相应修改markdown文件中的图片路径。

    注意：需要自己确保markdown图片在图片库中存在。
    '''

    if not os.path.isdir(dir_path):
        raise ValueError('parameter: dir_path must be dir path')
    if not os.path.isdir(local_imgs_storage_dir_path):
        raise ValueError('parameter: local_imgs_storage_dir_path must be dir path')

    for maindir, subdir_list, file_name_list in os.walk(dir_path):
        for file_name in file_name_list:
            file_abs_path = os.path.join(maindir, file_name)
            ext = os.path.splitext(file_abs_path)[1]
            if '.md' == ext:
                print('current process:', file_abs_path)
                extra_single_md_img(file_abs_path, local_imgs_storage_dir_path)


def recursion_aggregate_md_img(dir_path, local_imgs_storage_dir_path):
    '''
    递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，把位于markdown文件同名、同路径目录下的图片转移到
    本地markdown图片库，并相应修改markdown文件中的图片路径。
    '''

    if not os.path.isdir(dir_path):
        raise ValueError('parameter: dir_path must be dir path')
    if not os.path.exists(local_imgs_storage_dir_path):
        raise ValueError('parameter: local_imgs_storage_dir_path must exist')

    for maindir, subdir_list, file_name_list in os.walk(dir_path):
        # 建立索引map：{"xxx": "xxx.img", ...}
        # 如果markdown的文件名为：xxx.md，那么它的图片存在同名目录下：e.g. xxx.imgs（不一定是 .imgs 后缀，也可能是其它）
        img_dir_idx_map = {}
        for subdir in subdir_list:
            img_dir_idx_map[os.path.basename(os.path.splitext(os.path.basename(subdir))[0])] = subdir

        for file_name in file_name_list:
            file_abs_path = os.path.join(maindir, file_name)
            ext = os.path.splitext(file_abs_path)[1]
            if '.md' == ext:
                print('current process:', file_abs_path)

                # 复制图片到本地图片库
                file_basename_without_ext = os.path.splitext(os.path.basename(file_abs_path))[0]
                if file_basename_without_ext in img_dir_idx_map:
                    file_img_dir_path = os.path.join(maindir,
                                                     img_dir_idx_map[file_basename_without_ext])
                    for img in os.listdir(file_img_dir_path):
                        shutil.copyfile(os.path.join(file_img_dir_path, img),
                                        os.path.join(local_imgs_storage_dir_path, img))
                    shutil.rmtree(file_img_dir_path)

                # 替换文件图片链接
                proc_single_md_img_path(file_abs_path, local_imgs_storage_dir_path)


def add_img_name_for_single_md(md_file_path):
    '''
    为markdown文件中的图片添加名字。e.g. ![](xx_dir/xx.png] -> ![xx.png](xx_dir/xx.png]。
    '''

    if not os.path.isfile(md_file_path):
        raise ValueError('parameter: md_file_path must be file path')

    f_md = open(md_file_path, 'rt', encoding='utf-8')
    txt_src = f_md.read()
    txt_des = ''
    start_pos = 0
    img_pos1 = txt_src.find('![', start_pos)
    while -1 != img_pos1:
        img_pos2 = txt_src.find('(', img_pos1 + 2)
        img_pos3 = txt_src.find(')', img_pos2 + 1)
        img_path = txt_src[img_pos2 + 1:img_pos3]
        img_name = img_path.split('/')[-1]
        txt_des += txt_src[start_pos:img_pos1] + '![' + img_name + ']' + txt_src[img_pos2:img_pos3 + 1]
        start_pos = img_pos3 + 1
        img_pos1 = txt_src.find('![', start_pos)
    txt_des += txt_src[start_pos:]
    f_md.close()

    f_md = open(md_file_path, 'wt', encoding='utf-8')
    f_md.write(txt_des)
    f_md.close()


def recursion_add_img_name(dir_path):
    '''
    递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，
    为markdown文件中的图片添加名字。e.g. ![](xx_dir/xx.png] -> ![xx.png](xx_dir/xx.png]。
    '''

    if not os.path.isdir(dir_path):
        raise ValueError('parameter: dir_path must be dir path')

    for maindir, subdir_list, file_name_list in os.walk(dir_path):
        for file_name in file_name_list:
            file_abs_path = os.path.join(maindir, file_name)
            ext = os.path.splitext(file_abs_path)[1]
            if '.md' == ext:
                print('current process:', file_abs_path)
                add_img_name_for_single_md(file_abs_path)


def add_two_space_after_each_line(md_file_path):
    '''
    每一行末尾增加两个空格
    '''

    if not os.path.isfile(md_file_path):
        raise ValueError('parameter: md_file_path must be file path')

    f_md = open(md_file_path, 'rt', encoding='utf-8')
    txt_des = ''
    line = f_md.readline()
    while line:
        txt_des += line[:len(line) - 1] + '  \n'
        line = f_md.readline()
    f_md.close()

    f_md = open(md_file_path, 'wt', encoding='utf-8')
    f_md.write(txt_des)
    f_md.close()


def recursion_add_two_space_after_each_lin(dir_path):
    '''
    递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，每一行末尾增加两个空格
    '''
    if not os.path.isdir(dir_path):
        raise ValueError('parameter: dir_path must be dir path')

    for maindir, subdir_list, file_name_list in os.walk(dir_path):
        for file_name in file_name_list:
            file_abs_path = os.path.join(maindir, file_name)
            ext = os.path.splitext(file_abs_path)[1]
            if '.md' == ext:
                print('current process:', file_abs_path)
                add_two_space_after_each_line(file_abs_path)


def remove_space_after_each_line(md_file_path):
    '''
    删除每行末尾空格
    '''

    if not os.path.isfile(md_file_path):
        raise ValueError('parameter: md_file_path must be file path')

    f_md = open(md_file_path, 'rt', encoding='utf-8')
    txt_des = ''
    line = f_md.readline()
    while line:
        txt_des += line.rstrip(' \n') + '\n'
        line = f_md.readline()
    f_md.close()

    f_md = open(md_file_path, 'wt', encoding='utf-8')
    f_md.write(txt_des)
    f_md.close()


def recursion_remove_space_after_each_line(dir_path):
    '''
    递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，删除每行末尾空格
    '''

    if not os.path.isdir(dir_path):
        raise ValueError('parameter: dir_path must be dir path')

    for maindir, subdir_list, file_name_list in os.walk(dir_path):
        for file_name in file_name_list:
            file_abs_path = os.path.join(maindir, file_name)
            ext = os.path.splitext(file_abs_path)[1]
            if '.md' == ext:
                print('current process:', file_abs_path)
                remove_space_after_each_line(file_abs_path)


if '__main__' == __name__:
    msg = '''
功能：markdown文件处理器

运行模式：
    1：逐个解析markdown文件中的图片名字，并从本地markdown图片库中查找图片实际路径，进行替换。
    
    2：递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，逐个解析markdown文件中的图片名字，
       并从本地markdown图片库中查找图片实际路径，进行替换。
       
    3：从本地markdown图片库中查找markdown文件中的图片，复制到与markdown文件同名、同路径的目录下，
       并相应修改markdown文件中的图片路径。
       
    4：递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，从本地markdown图片库中查找markdown文件中的图片，
       复制到与markdown文件同名、同路径的目录下，并相应修改markdown文件中的图片路径。
       
    5：递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，把位于markdown文件同名、同路径目录下的图片转移到
       本地markdown图片库，并相应修改markdown文件中的图片路径。
       
    6：为markdown文件中的图片添加名字。e.g. ![](xx_dir/xx.png] -> ![xx.png](xx_dir/xx.png]。
    
    7：递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，
       为markdown文件中的图片添加名字。e.g. ![](xx_dir/xx.png] -> ![xx.png](xx_dir/xx.png]。
       
    8：每一行末尾增加两个空格
    
    9：递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，每一行末尾增加两个空格
    
    10：删除每行末尾空格
    
    11：递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，删除每行末尾空格
    '''

    print(msg)

    run_mode = input('输入运行模式：\n')

    if '1' == run_mode:
        md_file_path = input('输入要处理的markdown文件路径：\n')
        local_imgs_storage_dir_path = input('输入本地markdown图片库的目录路径：\n')
        proc_single_md_img_path(md_file_path, local_imgs_storage_dir_path)
        print('Done!')
    elif '2' == run_mode:
        dir_path = input('输入要处理的目录路径：\n')
        local_imgs_storage_dir_path = input('输入本地markdown图片库的目录路径：\n')
        recursion_proc_md_img_path(dir_path, local_imgs_storage_dir_path)
        print('All done!')
    elif '3' == run_mode:
        md_file_path = input('输入要处理的markdown文件路径：\n')
        local_imgs_storage_dir_path = input('输入本地markdown图片库的目录路径：\n')
        extra_single_md_img(md_file_path, local_imgs_storage_dir_path)
        print('Done!')
    elif '4' == run_mode:
        dir_path = input('输入要处理的目录路径：\n')
        local_imgs_storage_dir_path = input('输入本地markdown图片库的目录路径：\n')
        recursion_extra_md_img(dir_path, local_imgs_storage_dir_path)
        print('All done!')
    elif '5' == run_mode:
        dir_path = input('输入要处理的目录路径：\n')
        local_imgs_storage_dir_path = input('输入本地markdown图片库的目录路径：\n')
        recursion_aggregate_md_img(dir_path, local_imgs_storage_dir_path)
        print('All done!')
    elif '6' == run_mode:
        md_file_path = input('输入要处理的markdown文件路径：\n')
        add_img_name_for_single_md(md_file_path)
        print('Done!')
    elif '7' == run_mode:
        dir_path = input('输入要处理的目录路径：\n')
        recursion_add_img_name(dir_path)
        print('All done!')
    elif '8' == run_mode:
        md_file_path = input('输入要处理的markdown文件路径：\n')
        add_two_space_after_each_line(md_file_path)
        print('Done!')
    elif '9' == run_mode:
        dir_path = input('输入要处理的目录路径：\n')
        recursion_add_two_space_after_each_lin(dir_path)
        print('All done!')
    elif '10' == run_mode:
        md_file_path = input('输入要处理的markdown文件路径：\n')
        remove_space_after_each_line(md_file_path)
        print('Done!')
    elif '11' == run_mode:
        dir_path = input('输入要处理的目录路径：\n')
        recursion_remove_space_after_each_line(dir_path)
        print('All done!')
    else:
        print('运行模式输入错误，请退出程序，并重新运行')

    input('Press any key to exit!')
