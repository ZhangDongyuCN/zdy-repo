# -*- coding: UTF-8 -*-

import logging
import os
import sys


def init_log(log_path):
    if os.path.isfile(log_path):
        os.unlink(log_path)
    logging.basicConfig(filename=log_path,
                        level=logging.DEBUG,
                        format="%(asctime)s %(levelname)s %(message)s ")


def logger_info(msg):
    if sys._getframe().f_back:
        file_name = os.path.basename(sys._getframe().f_back.f_code.co_filename)
        func_name = sys._getframe().f_back.f_code.co_name
        lineno = sys._getframe().f_back.f_lineno
    else:
        file_name = os.path.basename(sys._getframe().f_code.co_filename)
        func_name = sys._getframe().f_code.co_name
        lineno = sys._getframe().f_lineno
    print(msg)
    logging.info(f"{file_name}[{func_name} {lineno}] {msg}")


def logger_error(msg):
    file_name = os.path.basename(sys._getframe().f_back.f_code.co_filename)
    func_name = sys._getframe().f_back.f_code.co_name
    lineno = sys._getframe().f_back.f_lineno
    msg = f"{file_name}[{func_name} {lineno}] {msg}"
    print(msg)
    logging.error(msg)
