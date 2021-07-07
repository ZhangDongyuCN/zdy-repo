#include <iostream>
#include "FileOperation.h"


using namespace std;

INT main()
{
    DLONG fileSize = 0;
    FileOperation::getFileSize("/root/download/new1/FileOperation.h", fileSize);
    cout << fileSize << endl;
    auto exeStat = FileOperation::truncateFile("/root/download/new1/FileOperation.h", 10000);
    if (false == exeStat.first)
    {
        cout << exeStat.second << endl;
    }
    else
    {
        cout << "SUCC" << endl;
    }
    FileOperation::getFileSize("/root/download/new1/FileOperation.h", fileSize);
    cout << fileSize << endl;

    return 0;
}