#include "FileOperation.h"

pair<BOOL, string> FileOperation::getDirSize(IN const string& dirLongName, OUT DLONG& dirSize)
{
    DIR* pDir = opendir(dirLongName.c_str());
    if (NULL != pDir)
    {
        struct dirent* pDirent = NULL;
        struct stat fileStat;
        string fileLongNameTmp;
        while (NULL != (pDirent = readdir(pDir)))
        {
            if (0 == strcmp(pDirent->d_name, ".") || 0 == strcmp(pDirent->d_name, ".."))
            {
                continue;
            }
            else if (DT_DIR != pDirent->d_type)
            {
                fileLongNameTmp = dirLongName + "/" + pDirent->d_name;
                if (0 == stat(fileLongNameTmp.c_str(), &fileStat))
                {
                    dirSize += fileStat.st_size;
                }
                else
                {
                    return make_pair(false, "file long name: " + fileLongNameTmp + ", error message: " + strerror(errno));
                }
            }
            else
            {
                pair<BOOL, string> exeStat = getDirSize(dirLongName + "/" + pDirent->d_name, dirSize);
                if (false == exeStat.first)
                {
                    return exeStat;
                }
            }
        }

        return make_pair(true, "");
    }
    else
    {
        return make_pair(false, "dir long name: " + dirLongName + ", error message: " + strerror(errno));
    }
}    //getDirSize()

pair<BOOL, string> FileOperation::getDirSizeAndMaxFileSize(IN const string& dirLongName, OUT DLONG& dirSize, OUT DLONG& maxFileSize)
{
    DIR* pDir = opendir(dirLongName.c_str());
    if (NULL != pDir)
    {
        struct dirent* pDirent = NULL;
        struct stat fileStat;
        string fileLongNameTmp;
        while (NULL != (pDirent = readdir(pDir)))
        {
            if (0 == strcmp(pDirent->d_name, ".") || 0 == strcmp(pDirent->d_name, ".."))
            {
                continue;
            }
            else if (DT_DIR != pDirent->d_type)
            {
                fileLongNameTmp = dirLongName + "/" + pDirent->d_name;
                if (0 == stat(fileLongNameTmp.c_str(), &fileStat))
                {
                    dirSize += fileStat.st_size;
                    if (fileStat.st_size > maxFileSize)
                    {
                        maxFileSize = fileStat.st_size;
                    }
                }
                else
                {
                    return make_pair(false, "file long name: " + fileLongNameTmp + ", error message: " + strerror(errno));
                }
            }
            else
            {
                pair<BOOL, string> exeStat = getDirSizeAndMaxFileSize(dirLongName + "/" + pDirent->d_name, dirSize, maxFileSize);
                if (false == exeStat.first)
                {
                    return exeStat;
                }
            }
        }

        return make_pair(true, "");
    }
    else
    {
        return make_pair(false, "dir long name: " + dirLongName + ", error message: " + strerror(errno));
    }
}    //getDirSizeAndMaxFileSize()

pair<BOOL, string> FileOperation::getFileSize(IN const string& fileLongName, OUT DLONG& fileSize)
{
    fileSize = 0;

    struct stat fileStat;
    if (0 == stat(fileLongName.c_str(), &fileStat))
    {
        fileSize = fileStat.st_size;
        return make_pair(true, "");
    }
    else
    {
        return make_pair(false, "file long name: " + fileLongName + ", error message: " + strerror(errno));
    }
}    //getFileSize()

pair<BOOL, string> FileOperation::getDiskSpace(IN const string& diskPath, OUT DLONG& totalSpace, OUT DLONG& freeSpace)
{
    totalSpace = 0;
    freeSpace = 0;

    struct statfs diskStatfs;
    if (statfs(diskPath.c_str(), &diskStatfs) >= 0)
    {
        totalSpace = (DLONG)diskStatfs.f_bsize * (DLONG)diskStatfs.f_blocks;
        freeSpace = (DLONG)diskStatfs.f_bsize * (DLONG)diskStatfs.f_bfree;
        return make_pair(true, "");
    }
    else
    {
        return make_pair(false, "disk path: " + diskPath + ", error message: " + strerror(errno));
    }
}    //getDiskSpace()

pair<BOOL, string> FileOperation::getAllFileShortNamesInDir(IN const string& dirLongName, OUT vector<string>& allFileShortNames)
{
    DIR* pDir = opendir(dirLongName.c_str());
    if (NULL != pDir)
    {
        struct dirent* pDirent = NULL;
        struct stat fileStat;
        while (NULL != (pDirent = readdir(pDir)))
        {
            if (0 == strcmp(pDirent->d_name, ".") || 0 == strcmp(pDirent->d_name, ".."))
            {
                continue;
            }
            else if (DT_DIR != pDirent->d_type)
            {
                allFileShortNames.emplace_back(pDirent->d_name);
            }
            else
            {
                pair<BOOL, string> exeStat = getAllFileShortNamesInDir(dirLongName + "/" + pDirent->d_name, allFileShortNames);
                if (false == exeStat.first)
                {
                    return exeStat;
                }
            }
        }

        return make_pair(true, "");
    }
    else
    {
        return make_pair(false, "dir long name: " + dirLongName + ", error message: " + strerror(errno));
    }
}    //getAllFileShortNamesInDir()

pair<BOOL, string> FileOperation::getAllFileLongNamesInDir(IN const string& dirLongName, OUT vector<string>& allFileLongNames)
{
    DIR* pDir = opendir(dirLongName.c_str());
    if (NULL != pDir)
    {
        struct dirent* pDirent = NULL;
        struct stat fileStat;
        while (NULL != (pDirent = readdir(pDir)))
        {
            if (0 == strcmp(pDirent->d_name, ".") || 0 == strcmp(pDirent->d_name, ".."))
            {
                continue;
            }
            else if (DT_DIR != pDirent->d_type)
            {
                allFileLongNames.emplace_back(dirLongName + "/" + pDirent->d_name);
            }
            else
            {
                pair<BOOL, string> exeStat = getAllFileLongNamesInDir(dirLongName + "/" + pDirent->d_name, allFileLongNames);
                if (false == exeStat.first)
                {
                    return exeStat;
                }
            }
        }

        return make_pair(true, "");
    }
    else
    {
        return make_pair(false, "dir long name: " + dirLongName + ", error message: " + strerror(errno));
    }
}    //getAllFileLongNamesInDir()

pair<BOOL, string> FileOperation::moveFileOrDir(IN const string& srcLongName, IN const string& destLongName)
{
    if (0 == rename(srcLongName.c_str(), destLongName.c_str()))
    {
        return make_pair(true, "");
    }
    else
    {
        return make_pair(false, "src file long name: " + srcLongName +
            ", dest file long name: " + destLongName + ", error message: " + strerror(errno));
    }
}    //moveFileOrDir()

pair<BOOL, string> FileOperation::removeFile(IN const string& fileLongName)
{
    if (0 == remove(fileLongName.c_str()))
    {
        return make_pair(true, "");
    }
    else
    {
        return make_pair(false, "file long name: " + fileLongName + ", error message: " + strerror(errno));
    }
}    //removeFile()

pair<BOOL, string> FileOperation::removeDir(IN const string& dirLongName)
{
    DIR* pDir = opendir(dirLongName.c_str());
    if (NULL != pDir)
    {
        struct dirent* pDirent = NULL;
        struct stat fileStat;
        while (NULL != (pDirent = readdir(pDir)))
        {
            if (0 == strcmp(pDirent->d_name, ".") || 0 == strcmp(pDirent->d_name, ".."))
            {
                continue;
            }
            else if (DT_DIR != pDirent->d_type)
            {
                pair<BOOL, string> exeStat = removeFile(dirLongName + "/" + pDirent->d_name);
                if (false == exeStat.first)
                {
                    return exeStat;
                }
            }
            else
            {
                pair<BOOL, string> exeStat = removeDir(dirLongName + "/" + pDirent->d_name);
                if (false == exeStat.first)
                {
                    return exeStat;
                }
            }
        }

        return make_pair(true, "");
    }
    else
    {
        return make_pair(false, "dir long name: " + dirLongName + ", error message: " + strerror(errno));
    }
}    //removeDir()

pair<BOOL, string> FileOperation::copyFile(IN const string& srcFileLongName, IN const string& destLongFileName)
{
    INT len = 0;
    FILE* pSrcFile = NULL;
    FILE* pDestFile = NULL;
    UINT bufferSize = 1 * 1024 * 1024;    //设置的过大也不会对性能有什么显著提升
    vector<CHAR> buffer(bufferSize, 0);

    if (NULL == (pSrcFile = fopen(srcFileLongName.c_str(), "rb")))
    {
        return make_pair(false, "source file long name: " + srcFileLongName + ", error message: " + strerror(errno));
    }
    if (NULL == (pDestFile = fopen(destLongFileName.c_str(), "wb")))
    {
        return make_pair(false, "dest file long name: " + destLongFileName + ", error message: " + strerror(errno));
    }

    while ((len = fread(&buffer[0], sizeof(BYTE), bufferSize, pSrcFile)) > 0)
    {
        fwrite(&buffer[0], sizeof(CHAR), len, pDestFile);
    }
    fclose(pDestFile);
    fclose(pSrcFile);

    return make_pair(true, "");
}    //copyFile()

pair<BOOL, string> FileOperation::copyDir(IN const string& srcDirPath, IN const string& destDirPath)
{
    if (0 != mkdir(destDirPath.c_str(), 00777))
    {
        return make_pair(false, "dest dir path: " + destDirPath + ", error message: " + strerror(errno));
    }

    DIR* pDir = opendir(srcDirPath.c_str());
    if (NULL != pDir)
    {
        struct dirent* pDirent = NULL;
        while (NULL != (pDirent = readdir(pDir)))
        {
            if (0 == strcmp(pDirent->d_name, ".") || 0 == strcmp(pDirent->d_name, ".."))
            {
                continue;
            }
            else if (DT_DIR != pDirent->d_type)
            {
                pair<BOOL, string> exeStat = copyFile(srcDirPath + "/" + pDirent->d_name, destDirPath + "/" + pDirent->d_name);
                if (false == exeStat.first)
                {
                    return exeStat;
                }
            }
            else
            {
                pair<BOOL, string> exeStat = copyDir(srcDirPath + "/" + pDirent->d_name, destDirPath + "/" + pDirent->d_name);
                if (false == exeStat.first)
                {
                    return exeStat;
                }
            }
        }

        return make_pair(true, "");
    }
    else
    {
        return make_pair(false, "source dir path: " + srcDirPath + ", error message: " + strerror(errno));
    }
}    //copyDir()

pair<BOOL, string> FileOperation::truncateFile(IN const string& fileLongName, IN const DLONG len)
{
    if (0 == truncate(fileLongName.c_str(), len))
    {
        return make_pair(true, "");
    }
    else
    {
        return make_pair(false, "file long name: " + fileLongName + ", error message: " + strerror(errno));
    }
}    //truncateFile()