# -*- coding: UTF-8 -*-

from os import walk, rename
from os.path import basename, join, splitext

from logger import logger_info


def change_ext(dir, newExt):
    for root, dirs, files in walk(dir):
        for name in files:
            baseName, ext = splitext(name)
            old_name = join(root, name)
            new_name = join(root, baseName + newExt)
            try:
                rename(old_name, new_name)
                logger_info(f'{basename(old_name)} --> {basename(new_name)}')
            except Exception as e:
                logger_info(f'跳过文件：{basename(old_name)}，异常信息：{e}')


def to_lower_case_ext(dir):
    for root, dirs, files in walk(dir):
        for name in files:
            baseName, ext = splitext(name)
            ext = ext.lower()
            old_name = join(root, name)
            new_name = join(root, baseName + ext)
            try:
                rename(old_name, new_name)
                logger_info(f'{basename(old_name)} --> {basename(new_name)}')
            except Exception as e:
                logger_info(f'跳过文件：{basename(old_name)}，异常信息：{e}')


def to_upper_case_ext(dir):
    for root, dirs, files in walk(dir):
        for name in files:
            baseName, ext = splitext(name)
            ext = ext.upper()
            old_name = join(root, name)
            new_name = join(root, baseName + ext)
            try:
                rename(old_name, new_name)
                logger_info(f'{basename(old_name)} --> {basename(new_name)}')
            except Exception as e:
                logger_info(f'跳过文件：{basename(old_name)}，异常信息：{e}')
