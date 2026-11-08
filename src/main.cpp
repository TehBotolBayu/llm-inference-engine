#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <cuda_runtime.h>
#include <cublas_v2.h>
#include "json.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <string>

#include "../awo/awo.h"
#include "../include/algorithm/minheap.hpp"
#include "../include/reader/mergeReader.hpp"
#include "../include/tokenUtils/mergeTxtUtil.hpp"

#include <vector>
#include <unordered_map>

using namespace std;
// include/algorithm/minheap.hpp
// #include "half.hpp"
// using half_float::half;

using json = nlohmann::json;

void readMergeTxt(std::string mergeFile, std::string prompt)
{
    std::unordered_map<std::string, int> mergeMapData = readTxt(mergeFile);
    // std::cout << "\n" + key << " ; order: " << mergeMapData[key] << std::endl;

    std::vector<std::string> tokens = buildMergedText(prompt, mergeMapData);
}

void readSafeTensor(std::string filePath)
{
    // safetensor read
    int fd = open(filePath.c_str(), O_RDONLY);

    if (fd == -1)
    {
        std::cerr << "file nya jelek mirip aldi" << std::endl;
        return;
    }

    struct stat stf;
    fstat(fd, &stf);

    char *fileMap = static_cast<char *>(mmap(NULL, stf.st_size, PROT_READ, MAP_SHARED, fd, 0));

    if (fileMap == MAP_FAILED)
    {
        std::cerr << "fail to retrive mmap" << std::endl;
        return;
    }

    if (stf.st_size <= sizeof(uint64_t))
    {
        std::cerr << "Corrupted safetensor file. Header size not found in int 64 format" << std::endl;
        return;
    }

    try
    {
        uint64_t headerSize = *reinterpret_cast<uint64_t *>(fileMap);

        if (headerSize + sizeof(uint64_t) >= static_cast<size_t>(stf.st_size))
        {
            std::cerr << "No data found , or model corrupted" << std::endl;
            return;
        }

        const char *startByte = fileMap + sizeof(uint64_t);

        // 3. Read headerSize bytes into a string buffer
        std::string_view jsonBuffer(startByte, headerSize);

        // 4. Parse the extracted JSON string
        json jsonPunyaAldi = json::parse(jsonBuffer);

        // Print header key summary as verification
        std::cout << "Berhasil parse safetensors header! Keys found:\n";
        int i = 0;
        for (auto &[key, value] : jsonPunyaAldi.items())
        {
            std::cout << i << ". - key: " << key
                      // << "; value: " << value
                      << "\n";
            i++;
        }

        // embedding layer test

        std::string prompt = "How are you";

        // int token = 123;

        // const char *dataStart = fileMap + sizeof(uint64_t) + headerSize;

        // const uint16_t *embedding = reinterpret_cast<const uint16_t *>(dataStart + 3148800 + 123 * 768 * 2);

        // just to make the fp16 human readable value (using compiler lib)
        // const _Float16 *embedding = reinterpret_cast<const _Float16 *>(dataStart + 3148800 + 123 * 768 * 2);
        // std::cout << "using float\n" << std::endl;
        // for (int i = 0; i < 768; ++i)
        // {
        //     float value = static_cast<float>(embedding[i]);
        //     std::cout << value << ' ';
        // }
        // std::cout << std::endl;

        // 1. add the token + positional embedding

        munmap(fileMap, stf.st_size);
        close(fd);
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "jsonnya gagal dibaca: " << e.what() << std::endl;
    }
}

void readVocab(std::string vocabFile)
{
    int vd = open(vocabFile.c_str(), O_RDONLY);
    if (vd == -1)
    {
        std::cerr << "error vocab json" << std::endl;
        return;
    }
    struct stat vst;
    fstat(vd, &vst);
    char *vocabFileMap = static_cast<char *>(mmap(NULL, vst.st_size, PROT_READ, MAP_SHARED, vd, 0));
    if (vocabFileMap == MAP_FAILED)
    {
        std::cerr << "fail to retrive mmap" << std::endl;
        return;
    }
    try
    {
        // 3. Read headerSize bytes into a string buffer
        std::string_view vocabJsonBuffer(vocabFileMap, vst.st_size);

        // 4. Parse the extracted JSON string
        json vocabParsedJson = json::parse(vocabJsonBuffer);

        std::string prompt = "testing";

        std::cout << "this is token for the word " << prompt << " : " << vocabParsedJson[prompt] << std::endl;
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "jsonnya gagal dibaca: " << e.what() << std::endl;
    }
}

void starterFileReader(const std::string &dir, const std::string &prompt)
{
    // 1. Open file in binary mode
    std::string filePath = dir + "/model.safetensors";
    std::string mergeFile = dir + "/merges.txt";
    std::string vocabFile = dir + "/vocab.json";

    readMergeTxt(mergeFile, prompt);

    return;
}

int main(int argc, char *argv[])
{
    std::string filePath = "/mnt/c/Users/bayuu/project/llm/facebook-opt-125m";
    std::string prompt = "";
    if (argc >= 2)
    {
        prompt = argv[1];
    }

    try
    {
        starterFileReader(filePath, prompt);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

// min heap test
// int main()
// {
// int sz;
// vector<int> vv;
// cout << "how many array: "; cin >> sz;

// int i = 0;
// while (i < sz) {
//     cout << "\n" << "input data: ";
//     int input; cin >> input;
//     vv.push_back(input);
//     cout << "\n";
//     // insertMinHeap(vv, input);
//     ++i;
// }

// buildMinHeap(vv);

// cout << "\n result: \n";
// i = 0;
// while (i < sz) {
//     cout << vv[i] << ", ";
//     ++i;
// }
// cout << "\n";
// }
