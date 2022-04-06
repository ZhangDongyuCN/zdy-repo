# -*- coding: UTF-8 -*-
# python = 3.8.5

import time
from os import walk, makedirs
from os.path import isfile, basename, join, isdir, splitext, dirname, exists
from shutil import move, rmtree

from logger import init_log, logger_info, logger_error


def proc_single_md_img_path(md_file_path, local_imgs_storage_dir_path):
    """
    逐个解析markdown文件中的图片名字，并从本地markdown图片库中查找图片实际路径，进行替换。

    注意：其实只是把local_imgs_storage_dir和解析出的图片名字拼成一个完整的路径，然后替换原文图片路径，
          并不是去图片库中查找，需要自己确保markdown图片在图片库中存在。
    """

    if not isfile(md_file_path):
        logger_info(f"参数「{md_file_path}」必须是文件路径")
        return
    if splitext(md_file_path)[1] != ".md":
        logger_info(f"参数「{md_file_path}」必须是.md文件")
        return
    if not isdir(local_imgs_storage_dir_path):
        logger_info(f"参数「{local_imgs_storage_dir_path}」必须是目录路径")
        return

    with open(md_file_path, "rt", encoding="utf-8") as f_md:
        txt_src = f_md.read()
        txt_des = ""
        start_pos = 0
        img_pos1 = txt_src.find("![", start_pos)
        while -1 != img_pos1:
            img_pos2 = txt_src.find("(", img_pos1 + 2)
            img_pos3 = txt_src.find(")", img_pos2 + 1)
            img_path = txt_src[img_pos2 + 1:img_pos3]
            img_name = basename(img_path)
            txt_des += txt_src[start_pos:img_pos1] \
                       + "![" + img_name + "]" \
                       + "(" + join(local_imgs_storage_dir_path, img_name) + ")"
            start_pos = img_pos3 + 1
            img_pos1 = txt_src.find("![", start_pos)
        txt_des += txt_src[start_pos:]

    with open(md_file_path, "wt", encoding="utf-8") as f_md:
        f_md.write(txt_des)


def recursion_proc_md_img_path(dir_path, local_imgs_storage_dir_path):
    """
    递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，逐个解析markdown文件中的图片名字，
    并从本地markdown图片库中查找图片实际路径，进行替换。

    注意：其实只是把local_imgs_storage_dir和解析出的图片名字拼成一个完整的路径，然后替换原文图片路径，
          并不是去图片库中查找，需要自己确保markdown图片在图片库中存在。
    """

    if not isdir(dir_path):
        logger_info(f"参数「{dir_path}」必须是目录路径")
        return
    if not isdir(local_imgs_storage_dir_path):
        logger_info(f"参数「{local_imgs_storage_dir_path}」必须是目录路径")
        return

    for maindir, subdir_list, file_name_list in walk(dir_path):
        for file_name in file_name_list:
            file_abs_path = join(maindir, file_name)
            ext = splitext(file_abs_path)[1]
            if ".md" == ext:
                logger_info(f"正在处理「{file_abs_path}」")
                proc_single_md_img_path(file_abs_path, local_imgs_storage_dir_path)


def extra_single_md_img(md_file_path, local_imgs_storage_dir_path):
    """
    从本地markdown图片库中查找markdown文件中的图片，移动到与markdown文件同名、同路径的目录下，
    并相应修改markdown文件中的图片路径。

    注意：需要自己确保markdown图片在图片库中存在。
    """

    if not isfile(md_file_path):
        logger_info(f"参数「{md_file_path}」必须是文件路径")
        return
    if splitext(md_file_path)[1] != ".md":
        logger_info(f"参数「{md_file_path}」必须是.md文件")
        return
    if not isdir(local_imgs_storage_dir_path):
        logger_info(f"参数「{local_imgs_storage_dir_path}」必须是目录路径")
        return

    # 创建与markdown文件同名、同路径的目录，用于保存markdown文件图片
    md_img_storage_dir_path = md_file_path.replace(".md", ".imgs")
    if not exists(md_img_storage_dir_path):
        makedirs(md_img_storage_dir_path)

    with open(md_file_path, "rt", encoding="utf-8") as f_md:
        txt_src = f_md.read()
        txt_des = ""
        start_pos = 0
        img_pos1 = txt_src.find("![", start_pos)
        while -1 != img_pos1:
            img_pos2 = txt_src.find("(", img_pos1 + 2)
            img_pos3 = txt_src.find(")", img_pos2 + 1)
            img_path = txt_src[img_pos2 + 1:img_pos3]
            img_name = basename(img_path)
            md_img_storage_file_path = join(md_img_storage_dir_path, img_name)
            if exists(img_path):
                move(img_path, md_img_storage_file_path)
                txt_des += txt_src[start_pos:img_pos1] \
                           + "![" + img_name + "]" \
                           + "(" + "./" + basename(md_img_storage_dir_path) + "/" + img_name + ")"
            else:
                logger_info(f"未在本地图片库找到「{img_path}」，请确认「{md_file_path}」")
                txt_des += txt_src[start_pos:img_pos3 + 1]
            start_pos = img_pos3 + 1
            img_pos1 = txt_src.find("![", start_pos)
        txt_des += txt_src[start_pos:]

    with open(md_file_path, "wt", encoding="utf-8") as f_md:
        f_md.write(txt_des)


def recursion_extra_md_img(dir_path, local_imgs_storage_dir_path):
    """
    递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，从本地markdown图片库中查找markdown文件中的图片，
    移动到与markdown文件同名、同路径的目录下，并相应修改markdown文件中的图片路径。

    注意：需要自己确保markdown图片在图片库中存在。
    """

    if not isdir(dir_path):
        logger_info(f"参数「{dir_path}」必须是目录路径")
        return
    if not isdir(local_imgs_storage_dir_path):
        logger_info(f"参数「{local_imgs_storage_dir_path}」必须是目录路径")
        return

    for maindir, subdir_list, file_name_list in walk(dir_path):
        for file_name in file_name_list:
            file_abs_path = join(maindir, file_name)
            ext = splitext(file_abs_path)[1]
            if ".md" == ext:
                logger_info(f"正在处理「{file_abs_path}」")
                extra_single_md_img(file_abs_path, local_imgs_storage_dir_path)


def aggregate_single_md_img(md_file_path, local_imgs_storage_dir_path):
    """
    把位于markdown文件同名、同路径目录下的图片转移到本地markdown图片库，并相应修改markdown文件中的图片路径。
    """

    if not isfile(md_file_path):
        logger_info(f"参数「{md_file_path}」必须是文件路径")
        return
    if splitext(md_file_path)[1] != ".md":
        logger_info(f"参数「{md_file_path}」必须是.md文件")
        return
    if not isdir(local_imgs_storage_dir_path):
        logger_info(f"参数「{local_imgs_storage_dir_path}」必须是目录路径")
        return

    # 校验图片存储目录是否存在（图片存储在与md文件同路径且同名的目录下）
    img_storage_dir = md_file_path.replace(".md", ".imgs")
    if not exists(img_storage_dir):
        logger_info("没有找到markdown图片存储目录，目录必须是「xxx.imgs」格式")
        return

    with open(md_file_path, "rt", encoding="utf-8") as f_md:
        txt_src = f_md.read()
        txt_des = ""
        start_pos = 0
        img_pos1 = txt_src.find("![", start_pos)
        while -1 != img_pos1:
            img_pos2 = txt_src.find("(", img_pos1 + 2)
            img_pos3 = txt_src.find(")", img_pos2 + 1)
            img_path = txt_src[img_pos2 + 1:img_pos3]
            img_path = join(dirname(md_file_path), img_path)
            img_name = basename(img_path)
            md_img_storage_file_path = join(local_imgs_storage_dir_path, img_name)
            if exists(img_path):
                move(img_path, md_img_storage_file_path)
                txt_des += txt_src[start_pos:img_pos1] \
                           + "![" + img_name + "]" \
                           + "(" + md_img_storage_file_path + ")"
            else:
                logger_info(f"未在本地图片库找到「{img_path}」，请确认「{md_file_path}」")
                txt_des += txt_src[start_pos:img_pos3 + 1]
            start_pos = img_pos3 + 1
            img_pos1 = txt_src.find("![", start_pos)
        txt_des += txt_src[start_pos:]

    with open(md_file_path, "wt", encoding="utf-8") as f_md:
        f_md.write(txt_des)

    rmtree(img_storage_dir)


def recursion_aggregate_md_img(dir_path, local_imgs_storage_dir_path):
    """
    递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，把位于markdown文件同名、同路径目录下的图片转移到
    本地markdown图片库，并相应修改markdown文件中的图片路径。
    """

    if not isdir(dir_path):
        logger_info(f"参数「{dir_path}」必须是目录路径")
        return
    if not isdir(local_imgs_storage_dir_path):
        logger_info(f"参数「{local_imgs_storage_dir_path}」必须是目录路径")
        return

    for maindir, subdir_list, file_name_list in walk(dir_path):
        for file_name in file_name_list:
            file_abs_path = join(maindir, file_name)
            ext = splitext(file_abs_path)[1]
            if ".md" == ext:
                logger_info(f"正在处理「{file_abs_path}」")
                aggregate_single_md_img(file_abs_path, local_imgs_storage_dir_path)


def add_img_name_for_single_md(md_file_path):
    """
    为markdown文件中的图片添加名字。e.g. ![*](xx_dir/xx.png] -> ![xx.png](xx_dir/xx.png]。
    """

    if not isfile(md_file_path):
        logger_info(f"参数「{md_file_path}」必须是文件路径")
        return
    if splitext(md_file_path)[1] != ".md":
        logger_info(f"参数「{md_file_path}」必须是.md文件")
        return

    with open(md_file_path, "rt", encoding="utf-8") as f_md:
        txt_src = f_md.read()
        txt_des = ""
        start_pos = 0
        img_pos1 = txt_src.find("![", start_pos)
        while -1 != img_pos1:
            img_pos2 = txt_src.find("(", img_pos1 + 2)
            img_pos3 = txt_src.find(")", img_pos2 + 1)
            img_path = txt_src[img_pos2 + 1:img_pos3]
            img_name = basename(img_path)
            txt_des += txt_src[start_pos:img_pos1] + "![" + img_name + "]" + txt_src[img_pos2:img_pos3 + 1]
            start_pos = img_pos3 + 1
            img_pos1 = txt_src.find("![", start_pos)
        txt_des += txt_src[start_pos:]

    with open(md_file_path, "wt", encoding="utf-8") as f_md:
        f_md.write(txt_des)


def recursion_add_img_name(dir_path):
    """
    递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，
    为markdown文件中的图片添加名字。e.g. ![*](xx_dir/xx.png] -> ![xx.png](xx_dir/xx.png]。
    """

    if not isdir(dir_path):
        logger_info(f"参数「{dir_path}」必须是目录路径")
        return

    for maindir, subdir_list, file_name_list in walk(dir_path):
        for file_name in file_name_list:
            file_abs_path = join(maindir, file_name)
            ext = splitext(file_abs_path)[1]
            if ".md" == ext:
                logger_info(f"正在处理「{file_abs_path}」")
                add_img_name_for_single_md(file_abs_path)


def add_two_space_after_each_line(md_file_path):
    """
    每一行末尾增加两个空格
    """

    if not isfile(md_file_path):
        logger_info(f"参数「{md_file_path}」必须是文件路径")
        return
    if splitext(md_file_path)[1] != ".md":
        logger_info(f"参数「{md_file_path}」必须是.md文件")
        return

    with open(md_file_path, "rt", encoding="utf-8") as f_md:
        txt_des = ""
        line = f_md.readline()
        while line:
            txt_des += line[:len(line) - 1] + "  \n"
            line = f_md.readline()

    with open(md_file_path, "wt", encoding="utf-8") as f_md:
        f_md.write(txt_des)


def recursion_add_two_space_after_each_lin(dir_path):
    """
    递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，每一行末尾增加两个空格
    """

    if not isdir(dir_path):
        logger_info(f"参数「{dir_path}」必须是目录路径")
        return

    for maindir, subdir_list, file_name_list in walk(dir_path):
        for file_name in file_name_list:
            file_abs_path = join(maindir, file_name)
            ext = splitext(file_abs_path)[1]
            if ".md" == ext:
                logger_info(f"正在处理「{file_abs_path}」")
                add_two_space_after_each_line(file_abs_path)


def remove_space_after_each_line(md_file_path):
    """
    删除每行末尾空格
    """

    if not isfile(md_file_path):
        logger_info(f"参数「{md_file_path}」必须是文件路径")
        return
    if splitext(md_file_path)[1] != ".md":
        logger_info(f"参数「{md_file_path}」必须是.md文件")
        return

    with open(md_file_path, "rt", encoding="utf-8") as f_md:
        txt_des = ""
        line = f_md.readline()
        while line:
            txt_des += line.rstrip(" \n") + "\n"
            line = f_md.readline()

    with open(md_file_path, "wt", encoding="utf-8") as f_md:
        f_md.write(txt_des)


def recursion_remove_space_after_each_line(dir_path):
    """
    递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，删除每行末尾空格
    """

    if not isdir(dir_path):
        logger_info(f"参数「{dir_path}」必须是目录路径")
        return

    for maindir, subdir_list, file_name_list in walk(dir_path):
        for file_name in file_name_list:
            file_abs_path = join(maindir, file_name)
            ext = splitext(file_abs_path)[1]
            if ".md" == ext:
                logger_info(f"正在处理「{file_abs_path}」")
                remove_space_after_each_line(file_abs_path)


if "__main__" == __name__:
    # 初始化日志
    init_log(log_path="./markdown_processor.log")

    # 任务处理
    try:
        while True:
            msg = """
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
功能：markdown文件处理器

运行模式：
    1：逐个解析markdown文件中的图片名字，并从本地markdown图片库中查找图片实际路径，进行替换。

    2：递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，逐个解析markdown文件中的图片名字，
       并从本地markdown图片库中查找图片实际路径，进行替换。

    3：从本地markdown图片库中查找markdown文件中的图片，移动到与markdown文件同名、同路径的目录下，
       并相应修改markdown文件中的图片路径。

    4：递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，从本地markdown图片库中查找markdown文件中的图片，
       移动到与markdown文件同名、同路径的目录下，并相应修改markdown文件中的图片路径。

    5：把位于markdown文件同名、同路径目录下的图片转移到本地markdown图片库，并相应修改markdown文件中的图片路径。

    6：递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，把位于markdown文件同名、同路径目录下的图片转移到
       本地markdown图片库，并相应修改markdown文件中的图片路径。

    7：为markdown文件中的图片添加名字。e.g. ![*](xx_dir/xx.png] -> ![xx.png](xx_dir/xx.png]。

    8：递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，
       为markdown文件中的图片添加名字。e.g. ![*](xx_dir/xx.png] -> ![xx.png](xx_dir/xx.png]。

    9：每一行末尾增加两个空格。

    10：递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，每一行末尾增加两个空格。

    11：删除每行末尾空格。

    12：递归逐个处理一个目录下的所有markdown文件，对于每个markdown文件，删除每行末尾空格。
                """

            print(msg)

            run_mode = input("输入运行模式：\n")

            if "1" == run_mode:
                md_file_path = input("输入要处理的markdown文件路径：\n")
                local_imgs_storage_dir_path = input("输入本地markdown图片库的目录路径：\n")
                logger_info(f"运行模式：{run_mode}，"
                            f"要处理的markdown文件路径：{md_file_path}，"
                            f"本地markdown图片库的目录路径：{local_imgs_storage_dir_path}")
                proc_single_md_img_path(md_file_path, local_imgs_storage_dir_path)

            elif "2" == run_mode:
                dir_path = input("输入要处理的目录路径：\n")
                local_imgs_storage_dir_path = input("输入本地markdown图片库的目录路径：\n")
                logger_info(f"运行模式：{run_mode}，"
                            f"要处理的目录路径：{dir_path}，"
                            f"本地markdown图片库的目录路径：{local_imgs_storage_dir_path}")
                recursion_proc_md_img_path(dir_path, local_imgs_storage_dir_path)

            elif "3" == run_mode:
                md_file_path = input("输入要处理的markdown文件路径：\n")
                local_imgs_storage_dir_path = input("输入本地markdown图片库的目录路径：\n")
                logger_info(f"运行模式：{run_mode}，"
                            f"要处理的markdown文件路径：{md_file_path}，"
                            f"本地markdown图片库的目录路径：{local_imgs_storage_dir_path}")
                extra_single_md_img(md_file_path, local_imgs_storage_dir_path)

            elif "4" == run_mode:
                dir_path = input("输入要处理的目录路径：\n")
                local_imgs_storage_dir_path = input("输入本地markdown图片库的目录路径：\n")
                logger_info(f"运行模式：{run_mode}，"
                            f"要处理的目录路径：{dir_path}，"
                            f"本地markdown图片库的目录路径：{local_imgs_storage_dir_path}")
                recursion_extra_md_img(dir_path, local_imgs_storage_dir_path)

            elif "5" == run_mode:
                md_file_path = input("输入要处理的markdown文件路径：\n")
                local_imgs_storage_dir_path = input("输入本地markdown图片库的目录路径：\n")
                logger_info(f"运行模式：{run_mode}，"
                            f"要处理的markdown文件路径：{md_file_path}，"
                            f"本地markdown图片库的目录路径：{local_imgs_storage_dir_path}")
                aggregate_single_md_img(md_file_path, local_imgs_storage_dir_path)

            elif "6" == run_mode:
                dir_path = input("输入要处理的目录路径：\n")
                local_imgs_storage_dir_path = input("输入本地markdown图片库的目录路径：\n")
                logger_info(f"运行模式：{run_mode}，"
                            f"要处理的目录路径：{dir_path}，"
                            f"本地markdown图片库的目录路径：{local_imgs_storage_dir_path}")
                recursion_aggregate_md_img(dir_path, local_imgs_storage_dir_path)

            elif "7" == run_mode:
                md_file_path = input("输入要处理的markdown文件路径：\n")
                logger_info(f"运行模式：{run_mode}，要处理的markdown文件路径：{md_file_path}")
                add_img_name_for_single_md(md_file_path)

            elif "8" == run_mode:
                dir_path = input("输入要处理的目录路径：\n")
                logger_info(f"运行模式：{run_mode}，要处理的目录路径：{dir_path}")
                recursion_add_img_name(dir_path)

            elif "9" == run_mode:
                md_file_path = input("输入要处理的markdown文件路径：\n")
                logger_info(f"运行模式：{run_mode}，要处理的markdown文件路径：{md_file_path}")
                add_two_space_after_each_line(md_file_path)

            elif "10" == run_mode:
                dir_path = input("输入要处理的目录路径：\n")
                logger_info(f"运行模式：{run_mode}，要处理的目录路径：{dir_path}")
                recursion_add_two_space_after_each_lin(dir_path)

            elif "11" == run_mode:
                md_file_path = input("输入要处理的markdown文件路径：\n")
                logger_info(f"运行模式：{run_mode}，要处理的markdown文件路径：{md_file_path}")
                remove_space_after_each_line(md_file_path)

            elif "12" == run_mode:
                dir_path = input("输入要处理的目录路径：\n")
                logger_info(f"运行模式：{run_mode}，要处理的目录路径：{dir_path}")
                recursion_remove_space_after_each_line(dir_path)

            else:
                logger_error(f"运行模式：{run_mode}，运行模式输入错误，请等待2秒后重新输入！")
                time.sleep(2)
                print("\n\n")
                continue

            logger_info("任务处理完毕，等待2秒后进入下一次循环！")
            time.sleep(2)
            print("\n\n")
    except Exception as e:
        logger_error(f"捕获到异常，代码内部逻辑错误，请及时修复！异常信息: {e}")
