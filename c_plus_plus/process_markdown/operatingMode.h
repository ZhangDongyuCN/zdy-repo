#pragma once

//include system header
#include <string>
#include <vector>

//include module header
#include"macroDefine.h"

using std::string;
using std::vector;

//数据库还不存在，新建数据库
void createDatabase();

//向数据库中插入图片的 OneDrive 直链信息
void insertImgInfoToDatabaseOneDrive();

//替换 Markdown 文章图片链接为 OneDrive 直链
void replaceImgUrlOneDrive();

//在 Markdown 文章图片链接中添加图片描述
//即 ![xxxx](1.img/20190605202304965.png) --> ![20190605202304965.png](1.img/20190605202304965.png)
void addImgDescription();

//在 Markdown 文章换行符前边添加两个空格，代码区不添加
void addTwoSpaceBeforeEachEnter();

//有颜色字体改为加粗
//即 <font color=XXX>XXX</font> --> XXX
void clearFontColor();

//<b>XXX</b> --> **XXX**
void changeBoldGrammarToAsterisk();

//**XXX** --> <b>XXX</b>
void changeBoldGrammarToHtmlFormat();

//添加打赏码
void addRewardCode();

//删除打赏码
void deleteRewardCode();

//添加文章推送头
void addPostHeader();

//删除文章推送头
void delPostHeader();

//添加马克飞象头
void addMaXiangHeader();

//替换 Markdown 文章图片链接为阿里云对象存储 OSS 链接（其它云也可以，更改配置文件即可）
void replaceImgUrlOSS();

//删除换行符前边的空白字符
void delSpaceBeforeEachEnter();

//更改图片名字
void renameImg();

//正则搜索
void regexSearch();

//正则替换
void regexReplace();

//读取指定文件夹下所有文件的名字
void readFileNameInDir(IN string strDir, INOUT vector<string>& vFileFullPath);

//获取时间戳（毫秒级别）
int64_t getMillSecSinceEpoch();