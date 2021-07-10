# -*- coding: UTF-8 -*-

import time
import exifread
from os import walk, rename
from os.path import basename, join, splitext, getctime, getmtime


def _gen_new_file_name(timeStamp, format):
    locTime = time.localtime(timeStamp)
    return time.strftime(format, locTime)


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


def _dea_repate_pic_name(info, root, pic_name, count):
    '''
    Description : 处理多张照片有相同拍摄时间的情况，如果多张照片有相同的拍摄时间，则命名格式为：IMG_20170616_221206.JPG，IMG_20170616_221206_1.JPG，...
    Modify      : 2020.12.11
    Input       : IN info：照片属性信息
                  IN root：照片所在的目录
                  IN pic_name：照片名字
                  IN count：拥有相同拍摄时间的照片个数计数器
    Return      :
    Caution     :
    '''
    new_name = 'IMG_' + info[0:10] + '_' + info[11:11 + 8] + '_' + str(count) + splitext(pic_name)[1]
    new_name = new_name.replace(':', '')
    new_name = join(root, new_name)
    try:
        rename(pic_name, new_name)
        print('{} --> {}'.format(basename(pic_name), basename(new_name)))
    except Exception as e:
        _dea_repate_pic_name(info, root, pic_name, count + 1)


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


def rename_pic(dir):
    '''
    Description : 根据照片拍摄时间递归重命名一个目录下所有的照片名字，命名格式为：IMG_20170616_221206.JPG，IMG_20170616_221206_1.JPG，...
    Modify      : 2020.12.11
    Input       : IN dir：目录路径
    Return      :
    Caution     :
    '''
    shooting_time_field = 'EXIF DateTimeOriginal'
    for root, dirs, files in walk(dir):
        for name in files:
            # exifread.process_file(fd) 可能抛出异常，所以这里用 try 捕获
            try:
                pic_name = join(root, name)
                fd = open(pic_name, 'rb')
                tags = exifread.process_file(fd)
                fd.close()
                if shooting_time_field in tags:
                    info = str(tags[shooting_time_field])
                    new_name = 'IMG_' + info[0:10] + '_' + info[11:11 + 8] + splitext(pic_name)[1]
                    new_name = new_name.replace(':', '')
                    new_name = join(root, new_name)

                    try:
                        rename(pic_name, new_name)
                        print('{} --> {}'.format(basename(pic_name), basename(new_name)))
                    except Exception as e:
                        _dea_repate_pic_name(info, root, pic_name, 1)
                else:
                    print('pass: {}'.format(basename(pic_name)))
            except Exception as e:
                print('pass: {}'.format(name))
