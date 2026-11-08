#pragma once
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unordered_map>

#include <sys/stat.h>
#include <sys/mman.h>

#include <optional>

#include <expected>

// define a value struct { order, value }

enum class ErrorCode
{
    InvalidInput,
    SystemFailure
};

std::unordered_map<std::string, int> readTxt(std::string filePath)
{
    std::unordered_map<std::string, int> mergeMapData;

    std::cout << "opening: " << filePath << std::endl;
    // open the file
    int fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1)
    {
        throw std::invalid_argument("File cant be open");
    }
    struct stat fst;
    fstat(fd, &fst);
    char *mergeFileMap = static_cast<char *>(mmap(NULL, fst.st_size, PROT_READ, MAP_SHARED, fd, 0));
    if (mergeFileMap == MAP_FAILED)
    {
        throw std::invalid_argument("Failed to create map");
    }

    // read file build hashmap
    // raed file from 0 to final byte
    int line = 0;
    bool isScanKey = true;
    int i = 0;
    while (i < fst.st_size)
    {
        // int data;
        std::string key;
        // data = line;
        while (mergeFileMap[i] != '\n')
        {
            if (mergeFileMap[i] != ' ')
            {
                key += mergeFileMap[i];
            }
            ++i;
        }
        mergeMapData[key] = line;
        ++line;
        isScanKey = true;
        ++i;
    }

    return mergeMapData;
}