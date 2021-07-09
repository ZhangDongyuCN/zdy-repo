# -*- coding: UTF-8 -*-
# python 3.9.0 64bit

import os
import exifread


def dealRepateName(info, root, picName, count):
    '''
    Description : 处理多张照片有相同拍摄时间的情况，如果多张照片有相同的拍摄时间，则命名格式为：IMG_20170616_221206.JPG，IMG_20170616_221206_1.JPG，...
    Modify      : 2020.12.11
    Input       : IN info：照片属性信息
                  IN root：照片所在的目录
                  IN picName：照片名字
                  IN count：拥有相同拍摄时间的照片个数计数器
    Return      :
    Caution     :
    '''
    newName = 'IMG_' + info[0:10] + '_' + info[11:11 + 8] + '_' + str(count) + os.path.splitext(picName)[1]
    newName = newName.replace(':', '')
    newName = os.path.join(root, newName)
    try:
        os.rename(picName, newName)
        print('{} --> {}'.format(os.path.basename(picName), os.path.basename(newName)))
    except Exception as e:
        dealRepateName(info, root, picName, count + 1)


def renamePic(dir):
    '''
    Description : 根据照片拍摄时间递归重命名一个目录下所有的照片名字，命名格式为：IMG_20170616_221206.JPG，IMG_20170616_221206_1.JPG，...
    Modify      : 2020.12.11
    Input       : IN dir：目录路径
    Return      :
    Caution     :
    '''
    shootingTimeField = 'EXIF DateTimeOriginal'
    for root, dirs, files in os.walk(dir):
        for name in files:
            # exifread.process_file(fd) 可能抛出异常，所以这里用 try 捕获
            try:
                picName = os.path.join(root, name)
                fd = open(picName, 'rb')
                tags = exifread.process_file(fd)
                fd.close()
                if shootingTimeField in tags:
                    info = str(tags[shootingTimeField])
                    newName = 'IMG_' + info[0:10] + '_' + info[11:11 + 8] + os.path.splitext(picName)[1]
                    newName = newName.replace(':', '')
                    newName = os.path.join(root, newName)

                    try:
                        os.rename(picName, newName)
                        print('{} --> {}'.format(os.path.basename(picName), os.path.basename(newName)))
                    except Exception as e:
                        dealRepateName(info, root, picName, 1)
                else:
                    print('pass: {}'.format(os.path.basename(picName)))
            except Exception as e:
                print('pass: {}'.format(name))


if '__main__' == __name__:
    dir = input('Please input picture directory path which you want to rename: ')
    renamePic(dir)
    print('Done!')
    input('Press any key to exit!')
