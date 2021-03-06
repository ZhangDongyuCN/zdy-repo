# -*- coding: UTF-8 -*-

import base64
import shutil
from functools import partial
from os import makedirs, listdir
from os.path import exists, join


def encode_file(file, txt):
    with open(file, 'rb') as fin, open(txt, 'wt') as fout:
        data = fin.read()
        base64_data = base64.b64encode(data)
        fout.write(base64_data.decode())


def decode_file(txt, file):
    with open(txt, 'rt') as fin, open(file, 'wb') as fout:
        base64_data = fin.read()
        data = base64.b64decode(base64_data)
        fout.write(data)


def split_txt_file(txt, each_file_mb, save_dir):
    each_file_b = each_file_mb * 1024 * 1024
    if exists(save_dir):
        shutil.rmtree(save_dir)
    makedirs(save_dir)

    with open(txt, 'r') as fin:
        blocks = iter(partial(fin.read, each_file_b), '')
        i = 1
        for block in blocks:
            with open(join(save_dir, str(i) + '.txt'), 'w') as fout:
                fout.write(block)
            i += 1


def merge_txt_file(txt_dir, txt):
    with open(txt, 'w') as fout:
        for i in range(1, len(listdir(txt_dir)) + 1):
            with open(join(txt_dir, str(i) + '.txt'), 'r') as fin:
                data = fin.read()
                fout.write(data)
