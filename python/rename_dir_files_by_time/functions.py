# -*- coding: UTF-8 -*-
# python 3.9.0 64bit

import os
import time


def generateNewName(timeStamp, format):
    locTime = time.localtime(timeStamp)
    return time.strftime(format, locTime)


def dealRepateName(root, longName, shortNewName, ext, count):
    longNewName = os.path.join(root, shortNewName + '_' + str(count) + ext)
    try:
        os.rename(longName, longNewName)
        print('{} --> {}'.format(os.path.basename(longName), os.path.basename(longNewName)))
    except Exception as e:
        dealRepateName(root, longName, shortNewName, ext, count + 1)


def renameFileByModifyTime(dir, format):
    longNewName = ''
    shortNewName = ''
    for root, dirs, files in os.walk(dir):
        for name in files:
            baseName, ext = os.path.splitext(name)
            longName = os.path.join(root, name)
            modifyTimeStamp = os.path.getmtime(longName)
            shortNewName = generateNewName(modifyTimeStamp, format)
            longNewName = os.path.join(root, shortNewName + ext)

            try:
                os.rename(longName, longNewName)
                print('{} --> {}'.format(os.path.basename(longName), os.path.basename(longNewName)))
            except Exception as e:
                dealRepateName(root, longName, shortNewName, ext, 1)


def renameFileByCreateTime(dir, format):
    longNewName = ''
    shortNewName = ''
    for root, dirs, files in os.walk(dir):
        for name in files:
            baseName, ext = os.path.splitext(name)
            longName = os.path.join(root, name)
            createTimeStamp = os.path.getctime(longName)
            shortNewName = generateNewName(createTimeStamp, format)
            longNewName = os.path.join(root, shortNewName + ext)

            try:
                os.rename(longName, longNewName)
                print('{} --> {}'.format(os.path.basename(longName), os.path.basename(longNewName)))
            except Exception as e:
                dealRepateName(root, longName, shortNewName, ext, 1)
