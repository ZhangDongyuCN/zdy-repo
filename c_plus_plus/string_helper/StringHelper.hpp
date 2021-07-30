#ifndef _STRING_HELPER_
#define _STRING_HELPER_

#include <string>
#include <cctype>
#include <algorithm>
#include <set>
#include <functional>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cmath>

//宏定义：参数流向
#define IN
#define OUT
#define INOUT

class StringHelper {
public:
    inline static void Capitalize(INOUT std::string& str);

    inline static void Title(INOUT std::string& str);

    inline static void Center(INOUT std::string& str, IN size_t width, IN const char fillchar = ' ');

    inline static void Ljust(INOUT std::string& str, IN size_t width, IN const char fillchar = ' ');

    inline static void Rjust(INOUT std::string& str, IN size_t width, IN const char fillchar = ' ');

    inline static void Zfill(INOUT std::string& str, IN size_t width);

    inline static void Lstrip(INOUT std::string& str, IN const std::string& chars = " ");

    inline static void Rstrip(INOUT std::string& str, IN const std::string& chars = " ");

    inline static void Strip(INOUT std::string& str, IN const std::string& chars = " ");

    inline static void Lower(INOUT std::string& str);

    inline static void Upper(INOUT std::string& str);

    inline static void Swapcase(INOUT std::string& str);

    inline static void CamelCase2Underline(INOUT std::string& str);

    inline static void Underline2CamelCase(INOUT std::string& str, IN bool toLowerCamelCase = false);

    inline static std::vector<std::string> Split(IN const std::string& str, IN const std::string& delims = " ");

    inline static std::vector<std::string> SplitCamelCase(IN const std::string& str);

    inline static std::vector<std::string> SplitUnderline(IN const std::string& str);

    inline static std::vector<std::string> SplitLines(IN const std::string& str, IN bool keepends = false);

    inline static char Max(IN const std::string& str);

    inline static char Min(IN const std::string& str);

    inline static size_t Count(IN const std::string& str, IN const std::string& sub,
                               IN size_t start = 0, IN size_t end = 0);

    inline static bool Isalnum(IN const std::string& str);

    inline static bool Isalpha(IN const std::string& str);

    inline static bool Isdigit(IN const std::string& str);

    inline static bool IsdigitWithDelim(IN const std::string& str, IN const std::string& delims = " ");

    inline static bool Isxdigit(IN const std::string& str);

    inline static bool IsxdigitWithDelim(IN const std::string& str, IN const std::string& delims = " ");

    inline static bool Islower(IN const std::string& str);

    inline static bool Isupper(IN const std::string& str);

    inline static bool Isspace(IN const std::string& str);

    inline static bool IsTitle(IN const std::string& str);

    inline static bool StartsWith(IN const std::string& str, IN const std::string& sub,
                                  IN size_t start = 0, IN size_t end = 0);

    inline static bool EndsWith(IN const std::string& str, IN const std::string& sub,
                                IN size_t start = 0, IN size_t end = 0);

    inline static std::string Join(IN const std::vector<std::string>& token, IN const std::string& linkChars = " ");

    inline static std::vector<std::string> ToVector(IN const std::string& str);

    template<class T>
    inline static T ToNum(IN const std::string& str);

    template<class T>
    inline static std::vector<T> ExtractNum(IN const std::string& str, IN const std::string& delims = " ");

    inline static std::string ReadLine();

    inline static std::string ReadLineEndByEnter(IN size_t size = 1);

    inline static std::vector<std::string> ReadLines(IN size_t size);
};

/*
 * 功能：将字符串的第一个字母变成大写，其它字母变小写。
 */
inline void StringHelper::Capitalize(INOUT std::string& str) {
    if (str.size() >= 1) {
        str[0] = toupper(str[0]);
        std::transform(str.begin() + 1, str.end(), str.begin() + 1, tolower);
    }
}

/*
 * 功能：返回一个指定的宽度width居中的字符串，fillchar为填充的字符，默认为空格。
 * 注意：若`(width - str.length) % 2 != 0`，则前边的填充字符数比后边的多1。
 */
inline void StringHelper::Center(INOUT std::string& str, IN size_t width, IN const char fillchar) {
    if (width > str.size()) {
        size_t padLen = width - str.size();
        std::string newStr(width, fillchar);
        std::move(str.begin(), str.end(), newStr.begin() + ceil(padLen / 2.0));
        str = std::move(newStr);
    }
}

/*
 * 功能：返回一个原字符串左对齐，并使用某种字符(默认空格)填充至指定长度的新字符串。
 *       如果指定的长度小于原字符串的长度则返回原字符串。
 */
inline void StringHelper::Ljust(std::string& str, size_t width, const char fillchar) {
    if (width > str.size()) {
        size_t oldSize = str.size();
        str.resize(width);
        std::fill(str.begin() + oldSize, str.end(), fillchar);
    }
}

/*
 * 功能：返回一个原字符串右对齐，并使用某种字符(默认空格)填充至指定长度的新字符串。
 *       如果指定的长度小于原字符串的长度则返回原字符串。
 */
inline void StringHelper::Rjust(std::string& str, size_t width, const char fillchar) {
    if (width > str.size()) {
        std::string newStr(width, fillchar);
        std::move(str.begin(), str.end(), newStr.begin() + width - str.size());
        str = std::move(newStr);
    }
}

/*
 * 功能：返回指定长度的字符串，原字符串右对齐，前面填充0。
 */
inline void StringHelper::Zfill(INOUT std::string& str, IN size_t width) {
    StringHelper::Rjust(str, width, '0');
}

/*
 * 功能：用于统计字符串里某个字符出现的次数
 * 参数：sub：搜索的子字符串
 *       start：字符串开始搜索的位置。默认0为第一个字符。
 *       end：字符串中结束搜索的位置。默认0为最后一个字符。
 * 注意：若end大于字符串长度，则自动设置为最后一个字符。
 */
inline size_t StringHelper::Count(IN const std::string& str, IN const std::string& sub,
                                  IN size_t start, IN size_t end) {
    if (start > str.size() - 1) {
        return 0;
    }

    if (end == 0 ||
        end > str.size() - 1) {
        end = str.size() - 1;
    }


    size_t count = 0;
    size_t pos = str.find(sub, start);
    while (pos != std::string::npos &&
           pos <= end - sub.size() + 1) {
        count++;
        pos = str.find(sub, std::min(pos + sub.size(), end));
    }
    
    return count;
}

/*
 * 功能：用于判断字符串是否以指定前缀开始，如果以指定前缀开始则返回true，否则返回false。
 *       可选参数"start"与"end"为检索字符串的开始与结束位置。
 * 参数：sub：搜索的子字符串
 *       start：字符串开始搜索的位置。默认0为第一个字符。
 *       end：字符串中结束搜索的位置。默认0为最后一个字符。
 * 注意：若end大于字符串长度，则自动设置为最后一个字符。
 */
inline bool StringHelper::StartsWith(IN const std::string& str, IN const std::string& sub,
                                     IN size_t start, IN size_t end) {
    if (start > str.size() - 1) {
        return false;
    }

    if (end == 0 ||
        end > str.size() - 1) {
        end = str.size() - 1;
    }

    if (end - start + 1 < sub.size()) {
        return false;
    } else {
        return std::equal(str.begin() + start, str.begin() + +start + sub.size(), sub.begin());
    }
}

/*
 * 功能：用于判断字符串是否以指定后缀结尾，如果以指定后缀结尾则返回true，否则返回false。
 *       可选参数"start"与"end"为检索字符串的开始与结束位置。
 * 参数：sub：搜索的子字符串
 *       start：字符串开始搜索的位置。默认0为第一个字符。
 *       end：字符串中结束搜索的位置。默认0为最后一个字符。
 * 注意：若end大于字符串长度，则自动设置为最后一个字符。
 */
inline bool StringHelper::EndsWith(IN const std::string& str, IN const std::string& sub,
                                   IN size_t start, IN size_t end) {
    if (start > str.size() - 1) {
        return false;
    }

    if (end == 0 ||
        end > str.size() - 1) {
        end = str.size() - 1;
    }

    if (end - start + 1 < sub.size()) {
        return false;
    } else {
        return std::equal(str.rbegin() + (str.size() - end - 1),
                          str.rbegin() + (str.size() - end - 1 + sub.size()),
                          sub.rbegin());
    }
}

/*
 * 功能：将序列中的字符串以指定的字符连接生成一个新的字符串。
 */
inline std::string StringHelper::Join(IN const std::vector<std::string>& token, IN const std::string& linkChars) {
    std::string linkdStr;
    for (size_t i = 0; i < token.size() - 1; i++) {
        linkdStr.append(token[i] + linkChars);
    }
    linkdStr.append(*(token.end() - 1));
    return linkdStr;
}

/*
 * 功能：字符串逐字符切割转vector。
 */
inline std::vector<std::string> StringHelper::ToVector(IN const std::string& str) {
    std::vector<std::string> token;
    token.reserve(str.size());
    std::for_each(str.begin(), str.end(), [&](const char& e) { token.push_back(std::string(1, e)); });
    return token;
}

/*
 * 功能：返回“标题化”的字符串，就是说所有单词的首个字母转化为大写，其余字母均为小写(见IsTitle())。
 */
inline void StringHelper::Title(IN std::string& str) {
    if (!str.empty()) {
        // may add some char else in the future
        std::set<char> punctuation{',', '.', '?'};

        str[0] = toupper(str[0]);

        for (size_t i = 1; i < str.size(); i++) {
            char prevChar = str[i - 1];
            if (isspace(prevChar) || isdigit(prevChar) || punctuation.find(prevChar) != punctuation.end()) {
                str[i] = toupper(str[i]);
            } else {
                str[i] = tolower(str[i]);
            }
        }
    }
}

/*
 * 功能：检测字符串是否只由字母和数字(十进制)组成。
 */
inline bool StringHelper::Isalnum(IN const std::string& str) {
    if (!str.empty()) {
        return std::all_of(str.begin(), str.end(), isalnum);
    } else {
        return false;
    }
}

/*
 * 功能：检测字符串是否只由字母组成。
 */
inline bool StringHelper::Isalpha(const std::string& str) {
    if (!str.empty()) {
        return std::all_of(str.begin(), str.end(), isalpha);
    } else {
        return false;
    }
}

/*
 * 功能：检测字符串是否只由数字(十进制)组成。
 */
inline bool StringHelper::Isdigit(const std::string& str) {
    if (!str.empty()) {
        return std::all_of(str.begin(), str.end(), isdigit);
    } else {
        return false;
    }
}

/*
 * 功能：检测字符串是不是以某种分隔符分隔的数字(十进制)字符串。
 *       支持单个分隔符，例如：delims = ","
 *       也支持多个分隔符，例如：delims = ",-"
 */
inline bool StringHelper::IsdigitWithDelim(IN const std::string& str, IN const std::string& delims) {
    if (!str.empty()) {
        return std::all_of(str.begin(), str.end(), [&](const char& e) {
            if (isdigit(e) ||
                delims.find(e) != std::string::npos) {
                return true;
            } else {
                return false;
            }
        });
    } else {
        return false;
    }
}

/*
 * 功能：检测字符串是否只由数字(16进制)组成。
 */
inline bool StringHelper::Isxdigit(const std::string& str) {
    if (!str.empty()) {
        return std::all_of(str.begin(), str.end(), isxdigit);
    } else {
        return false;
    }
}

/*
 * 功能：检测字符串是不是以某种分隔符分隔的数字(十六进制)字符串。
 *       支持单个分隔符，例如：delims = ","
 *       也支持多个分隔符，例如：delims = ",-"
 */
inline bool StringHelper::IsxdigitWithDelim(IN const std::string& str, IN const std::string& delims) {
    if (!str.empty()) {
        return std::all_of(str.begin(), str.end(), [&](const char& e) {
            if (isxdigit(e) ||
                delims.find(e) != std::string::npos) {
                return true;
            } else {
                return false;
            }
        });
    } else {
        return false;
    }
}

/*
 * 功能：检测字符串是否只由小写字母组成。
 */
inline bool StringHelper::Islower(const std::string& str) {
    if (!str.empty()) {
        return std::all_of(str.begin(), str.end(), islower);
    } else {
        return false;
    }
}

/*
 * 功能：检测字符串是否只由大写字母组成。
 */
inline bool StringHelper::Isupper(const std::string& str) {
    if (!str.empty()) {
        return std::all_of(str.begin(), str.end(), isupper);
    } else {
        return false;
    }
}

/*
 * 功能：检测字符串是否只包含空白。
 */
inline bool StringHelper::Isspace(const std::string& str) {
    if (!str.empty()) {
        return std::all_of(str.begin(), str.end(), isspace);
    } else {
        return false;
    }
}

/*
 * 功能：如果字符串是标题化的(见Title())则返回true，否则返回false。
 */
inline bool StringHelper::IsTitle(const std::string& str) {
    if (str.empty() || islower(str[0])) {
        return false;
    } else {
        // may add some char else in the future
        std::set<char> punctuation{',', '.', '?'};

        for (size_t i = 1; i < str.size(); i++) {
            char prevChar = str[i - 1];
            if (isspace(prevChar) || isdigit(prevChar) || punctuation.find(prevChar) != punctuation.end()) {
                if (islower(str[i])) {
                    return false;
                }
            } else {
                if (isupper(str[i])) {
                    return false;
                }
            }
        }

        return true;
    }
}

/*
 * 功能：转换字符串中所有大写字符为小写。
 */
inline void StringHelper::Lower(IN std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), tolower);
}

/*
 * 功能：转换字符串中所有小写字符为大写。
 */
inline void StringHelper::Upper(IN std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), toupper);
}

/*
 * 功能：对字符串的大小写字母进行转换。
 */
inline void StringHelper::Swapcase(IN std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](char& e) {
        if (islower(e)) {
            e = toupper(e);
        } else if (isupper(e)) {
            e = tolower(e);
        }
        return e;
    });
}

/*
 * 功能：驼峰(大驼峰和小驼峰均可)转下划线。
 *       例如：“ZhangSan"和"zhangSan"均可转成"zhang_san"。
 * 注意：需要用户保证输入是正确的驼峰，否则转换结果未知。
 */
inline void StringHelper::CamelCase2Underline(INOUT std::string& str) {
    if (!str.empty()) {
        std::string newStr;
        for (size_t i = 0; i < str.size(); i++) {
            if (i != str.size() - 1 && isupper(str[i + 1])) {
                newStr += tolower(str[i]);
                newStr += '_';
            } else {
                newStr += str[i];
            }
        }
        str = move(newStr);
    }
}

/*
 * 功能：下划线转驼峰(大驼峰和小驼峰均可，默认转为大驼峰)。
 * 注意：需要用户保证输入是正确的下划线风格，否则转换结果未知。
 */
inline void StringHelper::Underline2CamelCase(INOUT std::string& str, IN bool toLowerCamelCase) {
    if (!str.empty()) {
        std::string newStr;
        if (toLowerCamelCase) {
            newStr += tolower(str[0]);
        } else {
            newStr += toupper(str[0]);
        }
        for (size_t i = 1; i < str.size(); i++) {
            if (str[i] == '_') {
                continue;
            } else if (str[i - 1] == '_') {
                newStr += toupper(str[i]);
            } else {
                newStr += str[i];
            }
        }
        str = move(newStr);
    }
}

/*
 * 功能：截掉字符串左边指定字符（默认为空格；可以指定多个字符）。
 */
inline void StringHelper::Lstrip(IN std::string& str, IN const std::string& chars) {
    std::set<char> charsSet(chars.begin(), chars.end());
    size_t pos = 0;
    for (size_t i = 0; i < str.size(); i++) {
        if (charsSet.find(str[i]) == charsSet.end()) {
            pos = i;
            break;
        }
    }
    if (pos != 0) {
        str = str.substr(pos);
    }
}

/*
 * 功能：截掉字符串右边指定字符（默认为空格；可以指定多个字符）。
 */
inline void StringHelper::Rstrip(IN std::string& str, IN const std::string& chars) {
    std::set<char> charsSet(chars.begin(), chars.end());
    size_t pos = str.size() - 1;
    size_t len = str.size();
    for (size_t i = len - 1; i >= 0; i--) {
        if (charsSet.find(str[i]) == charsSet.end()) {
            pos = i;
            break;
        }
    }
    if (pos != str.size() - 1) {
        str = str.substr(0, pos + 1);
    }
}

/*
 * 功能：截掉字符串两边指定字符（默认为空格；可以指定多个字符）。
 */
inline void StringHelper::Strip(IN std::string& str, IN const std::string& chars) {
    std::set<char> charsSet(chars.begin(), chars.end());

    size_t len = str.size();
    size_t startPos = 0;
    for (size_t i = 0; i < len; i++) {
        if (charsSet.find(str[i]) == charsSet.end()) {
            startPos = i;
            break;
        }
    }

    size_t endPos = len - 1;
    for (size_t i = len - 1; i >= 0; i--) {
        if (charsSet.find(str[i]) == charsSet.end()) {
            endPos = i;
            break;
        }
    }

    if (startPos != 0 &&
        endPos != len - 1) {
        str = str.substr(startPos, endPos + 1 - startPos);
    }
}

/*
 * 功能：返回字符串中最大的字母。
 */
inline char StringHelper::Max(IN const std::string& str) {
    if (!str.empty()) {
        char maxChar = str[0];
        for (const auto& e:str) {
            if (e > maxChar) {
                maxChar = e;
            }
        }
        return maxChar;
    } else {
        throw std::range_error("The string is empty!");
    }
}

/*
 * 功能：返回字符串中最小的字母。
 */
inline char StringHelper::Min(IN const std::string& str) {
    if (!str.empty()) {
        char minChar = str[0];
        for (const auto& e:str) {
            if (e < minChar) {
                minChar = e;
            }
        }
        return minChar;
    } else {
        throw std::range_error("The string is empty!");
    }
}

/*
 * 功能：通过指定分隔符对字符串进行切片。
 *       支持单个分隔符，例如：delims = ","
 *       也支持多个分隔符，例如：delims = ",-."
 */
inline std::vector<std::string> StringHelper::Split(IN const std::string& str, IN const std::string& delims) {
    std::vector<std::string> token;
    size_t startPos = str.find_first_not_of(delims, 0);
    size_t endPos = str.find_first_of(delims, startPos);
    while (startPos != std::string::npos) {
        token.push_back(str.substr(startPos, endPos - startPos));
        startPos = str.find_first_not_of(delims, endPos);
        endPos = str.find_first_of(delims, startPos);
    }
    return token;
}

/*
 * 功能：按照行('\r', '\r\n', '\n')分隔，返回一个包含各行作为元素的数组，
 *       如果参数keepends为false，不包含换行符，如果为true，则保留换行符。
 */
inline std::vector<std::string> StringHelper::SplitLines(IN const std::string& str, bool keepends) {
    std::vector<std::string> token;
    if (!str.empty()) {
        if (keepends == false) {    //keepends == false
            std::string substr;
            size_t len = str.size();
            for (size_t i = 0; i < len; i++) {
                if (str[i] == '\n') {
                    token.push_back(std::move(substr));
                    substr.clear();
                } else if (str[i] == '\r') {
                    if (i != len - 1 &&
                        str[i + 1] == '\n') {
                        continue;
                    } else {
                        token.push_back(std::move(substr));
                        substr.clear();
                    }
                } else if (i == len - 1) {
                    substr += str[i];
                    token.push_back(std::move(substr));
                    substr.clear();
                } else {
                    substr += str[i];
                }
            }
        } else {    //keepends == true
            std::string substr;
            size_t len = str.size();
            for (size_t i = 0; i < len; i++) {
                if (str[i] == '\n') {
                    substr += str[i];
                    token.push_back(std::move(substr));
                    substr.clear();
                } else if (str[i] == '\r') {
                    if (i != len - 1 &&
                        str[i + 1] == '\n') {
                        substr += str[i];
                    } else {
                        substr += str[i];
                        token.push_back(std::move(substr));
                        substr.clear();
                    }
                } else if (i == len - 1) {
                    substr += str[i];
                    token.push_back(std::move(substr));
                    substr.clear();
                } else {
                    substr += str[i];
                }
            }
        }   //end keepends
        return token;
    } else {
        return token;
    }
}

/*
 * 功能：字符串转数字
 */
template<class T>
inline T StringHelper::ToNum(IN const std::string& str) {
    if (StringHelper::Isdigit(str)) {
        std::istringstream iss(str);
        T num;
        iss >> num;
        return num;
    } else {
        throw std::runtime_error("It is not a digit string!");
    }
}

/*
 * 功能：从以某种分隔符分隔的数字(十进制)字符串中提取数字。
 *       支持单个分隔符，例如：delims = ","
 *       也支持多个分隔符，例如：delims = ",-"
 */
template<class T>
inline std::vector<T> StringHelper::ExtractNum(IN const std::string& str, IN const std::string& delims) {
    if (StringHelper::IsdigitWithDelim(str, delims)) {
        std::vector<T> number;
        size_t startPos = str.find_first_not_of(delims, 0);
        size_t endPos = str.find_first_of(delims, startPos);
        while (startPos != std::string::npos) {
            number.push_back(StringHelper::ToNum<T>(str.substr(startPos, endPos - startPos)));
            startPos = str.find_first_not_of(delims, endPos);
            endPos = str.find_first_of(delims, startPos);
        }
        return number;
    } else {
        throw std::runtime_error("It is not a digit string with specified delim!");
    }
}

/*
 * 功能：分割驼峰风格字符串。
 */
inline std::vector<std::string> StringHelper::SplitCamelCase(IN const std::string& str) {
    std::vector<std::string> token;
    if (!str.empty()) {
        std::string subStr;
        for (size_t i = 0; i < str.size(); i++) {
            if ((i == str.size() - 1) ||
                (i != str.size() - 1 && isupper(str[i + 1]))) {
                subStr += str[i];
                token.push_back(move(subStr));
                subStr.clear();
            } else {
                subStr += str[i];
            }
        }
        return token;
    } else {
        return token;
    }
}

/*
 * 功能：分割下划线风格字符串。
 */
inline std::vector<std::string> StringHelper::SplitUnderline(IN const std::string& str) {
    return StringHelper::Split(str, "_");
}

/*
 * 功能：从标准输入读取一行。
 */
inline std::string StringHelper::ReadLine() {
    std::string line;
    getline(std::cin, line);
    return line;
}

/*
 * 功能：从标准输入读取一行，直到遇到size个'\n'为止。
 */
inline std::string StringHelper::ReadLineEndByEnter(IN size_t size) {
    std::string line;
    char c;
    int count = 0;
    while (c = std::cin.get()) {
        if (c != '\n') {
            line += c;
        } else {
            count++;
            if (count >= size) {
                break;
            }
        }
    }
    return line;
}

/*
 * 功能：从标准输入读取多行。
 */
inline std::vector<std::string> StringHelper::ReadLines(size_t size) {
    std::vector<std::string> lines(size);
    for (int i = 0; i < size; ++i) {
        lines[i] = ReadLine();
    }
    return lines;
}

#endif  //_STRING_HELPER_