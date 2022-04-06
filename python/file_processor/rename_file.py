# -*- coding: UTF-8 -*-

import re
import time
from os import walk, rename
from os.path import basename, join, splitext, getctime, getmtime

import exifread

from logger import logger_info, logger_error


def _gen_new_file_name(timestamp, format):
    loc_time = time.localtime(timestamp)
    return time.strftime(format, loc_time)


def _deal_repate_file_name(root, long_name, short_new_name, ext, count):
    long_new_name = join(root, short_new_name + '_' + str(count) + ext)
    try:
        rename(long_name, long_new_name)
        logger_info(f"{basename(long_name)} --> {basename(long_new_name)}")
    except Exception:
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
                logger_info(f"{basename(long_name)} --> {basename(long_new_name)}")
            except Exception:
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
                logger_info(f"{basename(long_name)} --> {basename(long_new_name)}")
            except Exception:
                _deal_repate_file_name(root, long_name, short_new_name, ext, 1)


def hump_2_underline_for_dir(dir):
    for root, dirs, files in walk(dir):
        for file in files:
            base_name, ext = splitext(file)
            old_name = join(root, file)
            new_name = join(root, _hump_2_underline(base_name) + ext)
            try:
                rename(old_name, new_name)
                logger_info(f"{basename(old_name)} --> {basename(new_name)}")
            except Exception as e:
                logger_error(f'跳过文件：{basename(old_name)}，异常信息：{e}')
        for dir in dirs:
            old_name = join(root, dir)
            new_name = join(root, _hump_2_underline(dir))
            try:
                rename(old_name, new_name)
                logger_info(f"{basename(old_name)} --> {basename(new_name)}")
            except Exception as e:
                logger_error(f'跳过文件：{basename(old_name)}，异常信息：{e}')


def underline_2_hump_for_dir(dir):
    for root, dirs, files in walk(dir):
        for file in files:
            base_name, ext = splitext(file)
            old_name = join(root, file)
            new_name = join(root, _underline_2_hump(base_name) + ext)
            try:
                rename(old_name, new_name)
                logger_info(f"{basename(old_name)} --> {basename(new_name)}")
            except Exception as e:
                logger_error(f'跳过文件：{basename(old_name)}，异常信息：{e}')
        for dir in dirs:
            old_name = join(root, dir)
            new_name = join(root, _underline_2_hump(dir))
            try:
                rename(old_name, new_name)
                logger_info(f"{basename(old_name)} --> {basename(new_name)}")
            except Exception as e:
                logger_error(f'跳过文件：{basename(old_name)}，异常信息：{e}')


def rename_file_by_regex(dir, pattern, repl):
    '''
    根据正则规则重命名文件。
    '''
    for root, dirs, files in walk(dir):
        for file in files:
            short_name = file
            long_name = join(root, file)

            short_new_name = re.sub(pattern, repl, short_name)
            long_new_name = join(root, short_new_name)

            try:
                rename(long_name, long_new_name)
                logger_info(f"{basename(long_name)} --> {basename(long_new_name)}")
            except Exception:
                _deal_repate_file_name(root, long_name, splitext(short_new_name)[0], splitext(short_new_name)[1], 1)


def rename_pic_by_shooting_time(dir):
    '''
    根据照片拍摄时间递归重命名一个目录下所有的照片名字，命名格式为：IMG_20170616_221206.JPG，IMG_20170616_221206_1.JPG，...
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
                        logger_info(f"{basename(long_name)} --> {basename(long_new_name)}")
                    except Exception:
                        _deal_repate_file_name(root, long_name, short_new_name, ext, 1)
                else:
                    logger_info(f'跳过文件：{basename(long_name)}')
            except Exception as e:
                logger_error(f'跳过文件：{file}，异常信息：{e}')


def rename_pic_with_weixin_format(dir):
    '''
    针对目录下（包含子目录）下的微信照片进行名称规范化处理。
    '''
    for root, dirs, files in walk(dir):
        for file in files:
            base_name, ext = splitext(file)
            long_name = join(root, file)

            # 微信照片的命名格式为：mmexport1405241187825.jpg，中间的数字是13位的毫秒级时间戳
            match_obj = re.search(r"^(mmexport)(\d{13})(\D*)$", base_name)
            if match_obj:
                timestamp = float(match_obj.group(2)[0:10])  # localtime只能接收秒级的时间戳，也就是10位
                time.localtime(timestamp)
                short_new_name = _gen_new_file_name(timestamp, "IMG_%Y%m%d_%H%M%S")
                long_new_name = join(root, short_new_name + ext)

                try:
                    rename(long_name, long_new_name)
                    logger_info(f"{basename(long_name)} --> {basename(long_new_name)}")
                except Exception:
                    _deal_repate_file_name(root, long_name, short_new_name, ext, 1)


def rename_pic_with_meituxiuxiu_format(dir):
    '''
    针对目录下（包含子目录）下的美图秀秀照片进行名称规范化处理。
    '''
    pass


def rename_pic_with_baiduwangpan_format(dir):
    '''
    针对目录下（包含子目录）下的百度网盘照片进行名称规范化处理。
    '''
    for root, dirs, files in walk(dir):
        for file in files:
            short_name = file
            long_name = join(root, file)

            short_new_name = re.sub(r"^(\d{4})(-)(\d{2})(-)(\d{2})( )(\d{6})(\.)(.+)$",
                                    r"IMG_\g<1>\g<3>\5_\g<7>\g<8>\g<9>",
                                    short_name)
            long_new_name = join(root, short_new_name)

            try:
                rename(long_name, long_new_name)
                logger_info(f"{basename(long_name)} --> {basename(long_new_name)}")
            except Exception:
                _deal_repate_file_name(root, long_name, splitext(short_new_name)[0], splitext(short_new_name)[1], 1)
