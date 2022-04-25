import re
from random import SystemRandom


class GenerateRandomPwd:
    RANDOM_INSTANCE = SystemRandom()
    CHAR_SET = ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~!@#$%^&*")
    CHAR_SET_LEN = len(CHAR_SET)
    COMPLEXITY_PATTERNS = (r"[0-9]", r"[a-z]", r"[A-Z]", r'[~!@#$%^&*]')
    COMPILE_COMPLEXITY_PATTERNS = [re.compile(i) for i in COMPLEXITY_PATTERNS]

    @staticmethod
    def _get_random_integer():
        return GenerateRandomPwd.RANDOM_INSTANCE.randint(0, GenerateRandomPwd.CHAR_SET_LEN - 1)

    @staticmethod
    def _check_pwd_complexity(pwd):
        curr_pwd_complexity = 0
        for pattern in GenerateRandomPwd.COMPILE_COMPLEXITY_PATTERNS:
            if pattern.findall(pwd):
                curr_pwd_complexity = curr_pwd_complexity + 1
            if curr_pwd_complexity == len(GenerateRandomPwd.COMPLEXITY_PATTERNS):
                return True

        return False

    @staticmethod
    def generate_random_pwd(pwd_len):
        if not isinstance(pwd_len, int):
            raise Exception("pwd_len should be int")

        pwd = ""
        while len(pwd) < pwd_len:
            random_number = GenerateRandomPwd._get_random_integer()
            random_char = GenerateRandomPwd.CHAR_SET[random_number]
            pwd = f"{pwd}{random_char}"
            if len(pwd) == pwd_len:
                if GenerateRandomPwd._check_pwd_complexity(pwd):
                    break
                pwd = pwd[1:]

        return pwd
