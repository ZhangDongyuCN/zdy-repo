# -*- coding: UTF-8 -*-

import time
import exifread
from os import walk, rename
from os.path import basename, join, splitext, getctime, getmtime
import re


def _gen_new_file_name(timestamp, format):
    loc_time = time.localtime(timestamp)
    return time.strftime(format, loc_time)


def _deal_repate_file_name(root, long_name, short_new_name, ext, count):
    long_new_name = join(root, short_new_name + '_' + str(count) + ext)
    try:
        rename(long_name, long_new_name)
        print('{} --> {}'.format(basename(long_name), basename(long_new_name)))
    except Exception as e:
        _deal_repate_file_name(root, long_name, short_new_name, ext, count + 1)


def _hump_2_underline(text):
    res = []
    for index, char in enumerate(text):
        if char.isupper() and index != 0:
            res.append("_")
        res.append(char)
    return ''.join(res).lower()


def _underline_2_hump(text):
    arr = text.lower().split('_')
    res = []
    for i in arr:
        res.append(i[0].upper() + i[1:])
    return ''.join(res)


def rename_file_by_modify_time(dir, format):
    for root, dirs, files in walk(dir):
        for name in files:
            base_name, ext = splitext(name)
            long_name = join(root, name)
            modify_time_stamp = getmtime(long_name)
            short_new_name = _gen_new_file_name(modify_time_stamp, format)
            long_new_name = join(root, short_new_name + ext)

            try:
                rename(long_name, long_new_name)
                print('{} --> {}'.format(basename(long_name), basename(long_new_name)))
            except Exception as e:
                _deal_repate_file_name(root, long_name, short_new_name, ext, 1)


def rename_file_by_create_time(dir, format):
    for root, dirs, files in walk(dir):
        for name in files:
            base_name, ext = splitext(name)
            long_name = join(root, name)
            create_time_stamp = getctime(long_name)
            short_new_name = _gen_new_file_name(create_time_stamp, format)
            long_new_name = join(root, short_new_name + ext)

            try:
                rename(long_name, long_new_name)
                print('{} --> {}'.format(basename(long_name), basename(long_new_name)))
            except Exception as e:
                _deal_repate_file_name(root, long_name, short_new_name, ext, 1)


def hump_2_underline_for_dir(dir):
    for root, dirs, files in walk(dir):
        for file in files:
            base_name, ext = splitext(file)
            old_name = join(root, file)
            new_name = join(root, _hump_2_underline(base_name) + ext)
            rename(old_name, new_name)
        for dir in dirs:
            old_name = join(root, dir)
            new_name = join(root, _hump_2_underline(dir))
            rename(old_name, new_name)


def underline_2_hump_for_dir(dir):
    for root, dirs, files in walk(dir):
        for file in files:
            base_name, ext = splitext(file)
            old_name = join(root, file)
            new_name = join(root, _underline_2_hump(base_name) + ext)
            rename(old_name, new_name)
        for dir in dirs:
            old_name = join(root, dir)
            new_name = join(root, _underline_2_hump(dir))
            rename(old_name, new_name)


def rename_pic_by_shooting_time(dir):
    '''
    Description : 根据照片拍摄时间递归重命名一个目录下所有的照片名字，命名格式为：IMG_20170616_221206.JPG，IMG_20170616_221206_1.JPG，...
    Modify      : 2020.12.11
    Input       : IN dir：目录路径
    Return      :
    Caution     :
    '''
    shooting_time_field = 'EXIF DateTimeOriginal'
    for root, dirs, files in walk(dir):
        for file in files:
            # exifread.process_file(fd) 可能抛出异常，所以这里用 try 捕获
            try:
                base_name, ext = splitext(file)
                long_name = join(root, file)

                with open(long_name, 'rb') as fd:
                    tags = exifread.process_file(fd)

                if shooting_time_field in tags:
                    info = str(tags[shooting_time_field])
                    short_new_name = 'IMG_' + info[0:10] + '_' + info[11:11 + 8]
                    short_new_name = short_new_name.replace(':', '')
                    long_new_name = join(root, short_new_name + ext)

                    try:
                        rename(long_name, long_new_name)
                        print(f'{basename(long_name)} --> {basename(long_new_name)}')
                    except Exception:
                        _deal_repate_file_name(root, long_name, short_new_name, ext, 1)
                else:
                    print(f'pass: {basename(long_name)}')
            except Exception:
                print(f'pass: {file}')


def rename_pic_by_timestamp(dir):
    '''
    Description : 根据照片名中的时间戳重命名照片（针对微信，美图秀秀等软件保存的照片）
    Modify      : 2022.4.5
    Input       : IN dir：目录路径
    Return      :
    Caution     :
    '''
    for root, dirs, files in walk(dir):
        for file in files:
            base_name, ext = splitext(file)
            long_name = join(root, file)

            # 例如微信照片的命名格式为：mmexport1405241187825.jpg，中间的数字是13位的毫秒级时间戳
            match_obj = re.search(r"^(\D*)(\d{13})(\D*)$", base_name)
            if match_obj:
                timestamp = float(match_obj.group(2)[0:10])  # localtime只能接收秒级的时间戳，也就是10位
                time.localtime(timestamp)
                short_new_name = _gen_new_file_name(timestamp, "IMG_%Y%m%d_%H%M%S")
                long_new_name = join(root, short_new_name + ext)

                try:
                    rename(long_name, long_new_name)
                    print(f'{basename(long_name)} --> {basename(long_new_name)}')
                except Exception:
                    _deal_repate_file_name(root, long_name, short_new_name, ext, 1)


def rename_file_by_regex(dir, pattern, repl):
    '''
    Description : 根据正则规则重命名文件
    Modify      : 2022.4.5
    Input       : IN dir：目录路径
    Return      :
    Caution     :
    '''
    for root, dirs, files in walk(dir):
        for file in files:
            short_name = file
            long_name = join(root, file)

            short_new_name = re.sub(pattern, repl, short_name)
            long_new_name = join(root, short_new_name)

            try:
                rename(long_name, long_new_name)
                print(f'{basename(long_name)} --> {basename(long_new_name)}')
            except Exception:
                _deal_repate_file_name(root, long_name, splitext(short_new_name)[0], splitext(short_new_name)[1], 1)
