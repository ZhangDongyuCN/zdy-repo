# -*- coding: UTF-8 -*-

from math import fabs
import re
import time
from os import walk, rename
from os.path import basename, join, splitext, getctime, getmtime, exists
from tkinter.messagebox import NO

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
                logger_error(f'???????????????{basename(old_name)}??????????????????{e}')
        for dir in dirs:
            old_name = join(root, dir)
            new_name = join(root, _hump_2_underline(dir))
            try:
                rename(old_name, new_name)
                logger_info(f"{basename(old_name)} --> {basename(new_name)}")
            except Exception as e:
                logger_error(f'???????????????{basename(old_name)}??????????????????{e}')


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
                logger_error(f'???????????????{basename(old_name)}??????????????????{e}')
        for dir in dirs:
            old_name = join(root, dir)
            new_name = join(root, _underline_2_hump(dir))
            try:
                rename(old_name, new_name)
                logger_info(f"{basename(old_name)} --> {basename(new_name)}")
            except Exception as e:
                logger_error(f'???????????????{basename(old_name)}??????????????????{e}')


def rename_file_by_regex(dir, pattern, repl):
    '''
    ????????????????????????????????????
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


def rename_photo_with_regex_format(file, base_name, ext, long_name, root):
    match_dict = {
        "????????????": (r"^(MTXX_)(\d{8})(\d{6})(\.)(.+)$", r"IMG_\g<2>_\g<3>\g<4>\g<5>"),
        "????????????": (r"^(\d{4})(-)(\d{2})(-)(\d{2})( )(\d{6})(\.)(.+)$", r"IMG_\g<1>\g<3>\5_\g<7>\g<8>\g<9>"),
        "april_2017-10-17-16-27-35-775.jpg": (r"^(.*)(\d{4})(.)(\d{2})(.)(\d{2})(.)(\d{2})(.)(\d{2})(.)(\d{2})(.*)(\.)(.+)$", r"IMG_\g<2>\g<4>\g<6>_\g<8>\g<10>\g<12>\g<14>\g<15>"),
        "PIC_20220222_201411812.jpg": (r"^(.*)(\d{8}_)(\d{6})(.*)(\.)(.+)", r"IMG_\g<2>\g<3>\g<5>\g<6>")
    }

    for _, (pattern, repl) in match_dict.items():
        match_obj = re.search(pattern, file)
        if match_obj:
            short_name = file
            short_new_name = re.sub(pattern, repl, short_name)
            long_new_name = join(root, short_new_name)

            try:
                rename(long_name, long_new_name)
                logger_info(f"{basename(long_name)} --> {basename(long_new_name)}")
            except Exception:
                _deal_repate_file_name(root, long_name, splitext(short_new_name)[0], splitext(short_new_name)[1], 1)

    # ?????????????????????????????????????????????????????????????????????????????????
    if exists(long_name):
        return False
    return True


def rename_photo_with_timestamp_regex_format(file, base_name, ext, long_name, root):
    match_dict = {
        "??????": (r"^(mmexport)(\d{13})(\D*)$", 2),
        "???????????????": (r"^(.*)(\d{13})(.*)$", 2)
    }

    for _, (pattern, group) in match_dict.items():
        match_obj = re.search(pattern, base_name)
        if match_obj:
            timestamp = float(match_obj.group(group)[0:10])  # localtime??????????????????????????????????????????10???
            time.localtime(timestamp)
            short_new_name = _gen_new_file_name(timestamp, "IMG_%Y%m%d_%H%M%S")
            long_new_name = join(root, short_new_name + ext)

            try:
                rename(long_name, long_new_name)
                logger_info(f"{basename(long_name)} --> {basename(long_new_name)}")
            except Exception:
                _deal_repate_file_name(root, long_name, short_new_name, ext, 1)

    # ?????????????????????????????????????????????????????????????????????????????????
    if exists(long_name):
        return False
    return True


def rename_photo_by_shooting_time(file, base_name, ext, long_name, root):
    '''
    ????????????????????????????????????????????????????????????????????????IMG_20170616_221206.JPG???IMG_20170616_221206_1.JPG???...
    '''
    shooting_time_field = 'EXIF DateTimeOriginal'
    # exifread.process_file(fd) ???????????????????????????????????? try ??????
    try:
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
            logger_info(f'???????????????{basename(long_name)}')
    except Exception as e:
        logger_error(f'???????????????{file}??????????????????{e}')

    # ?????????????????????????????????????????????????????????????????????????????????
    if exists(long_name):
        return False
    return True


def rename_dir_photos(dir):
    '''
    ??????????????????????????????????????????????????????????????????????????????
    '''
    for root, dirs, files in walk(dir):
        for file in files:
            base_name, ext = splitext(file)
            long_name = join(root, file)

            if ext in (".mp4", ):
                continue

            if re.search(r"^IMG_\d{8}_\d{6}\..+$", file):
                continue

            if rename_photo_by_shooting_time(file, base_name, ext, long_name, root):
                continue

            if rename_photo_with_timestamp_regex_format(file, base_name, ext, long_name, root):
                continue

            rename_photo_with_regex_format(file, base_name, ext, long_name, root)
