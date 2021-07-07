//include system header
#include <iostream>

//include module header
#include "operatingMode.h"
#include "ParseConfig.hpp"

using std::cout;
using std::cin;
using std::endl;

using ParseConfig = sinicheveen::ParseConfig;

int main()
{
    //解析配置文件
    ParseConfig::s_strConfFilePath("collateMarkdownArtile.conf");
    ParseConfig::parseConfFileIntoMap();

    while (true)
    {
        //运行模式
        cout << endl;
        cout << "=========================================\
===================================================" << endl;
        cout << endl << "operating mode: " << endl;
        cout << " 0 -> exit program" << endl;
        cout << " 1 -> the database does not exist yet, create a database" << endl;
        cout << " 2 -> insert onedrive direct link infomation to database" << endl;
        cout << " 3 -> replace Markdown article image url with onedrive direct link" << endl;
        cout << " 4 -> add image description in Markdown article: ![xxxx](1.img/20190605202304965.png) --> ![20190605202304965.png](1.img/20190605202304965.png)" << endl;
        cout << " 5 -> add two space before each enter in Markdown article" << endl;
        cout << " 6 -> clear font color in Markdown article: <font color=XXX>XXX</font> --> XXX" << endl;
        cout << " 7 -> change bold grammar to asterisk in Markdown article: <b>XXX</b> --> **XXX**" << endl;
        cout << " 8 -> change bold grammar to html format in Markdown article: **XXX** --> <b>XXX</b>" << endl;
        cout << " 9 -> add reward code in Markdown article" << endl;
        cout << "10 -> delete reward code in Markdown article" << endl;
        cout << "11 -> add post header in Markdown article" << endl;
        cout << "12 -> delete post header in Markdown article" << endl;
        cout << "13 -> add maxiang header in Markdown article" << endl;
        cout << "14 -> replace Markdown article image url with aliyun oss link" << endl;
        cout << "15 -> delete space before each enter in Markdown article" << endl;
        cout << "16 -> rename image name in Markdown article" << endl;
        cout << "17 -> search content in Markdown article" << endl;
        cout << "18 -> replace content in Markdown article" << endl << endl;

        //输入运行模式对应的数字代码，启动该模式
        int operatingMode;
        cout << "please input the operating mode num: " << endl;
        cin >> operatingMode;
        cout << endl;

        switch (operatingMode)
        {
        case 0:
            return 0;
        case 1:
            createDatabase();
            break;
        case 2:
            insertImgInfoToDatabaseOneDrive();
            break;
        case 3:
            replaceImgUrlOneDrive();
            break;
        case 4:
            addImgDescription();
            break;
        case 5:
            addTwoSpaceBeforeEachEnter();
            break;
        case 6:
            clearFontColor();
            break;
        case 7:
            changeBoldGrammarToAsterisk();
            break;
        case 8:
            changeBoldGrammarToHtmlFormat();
            break;
        case 9:
            addRewardCode();
            break;
        case 10:
            deleteRewardCode();
            break;
        case 11:
            addPostHeader();
            break;
        case 12:
            delPostHeader();
            break;
        case 13:
            addMaXiangHeader();
            break;
        case 14:
            replaceImgUrlOSS();
            break;
        case 15:
            delSpaceBeforeEachEnter();
            break;
        case 16:
            renameImg();
            break;
        case 17:
            regexSearch();
            break;
        case 18:
            regexReplace();
            break;
        default:
            break;
        }
    }    //while

}    //main