#pragma once

#include <fstream>
#include <string>
#include <sstream>

class FileProcessor {
private:
    std::ifstream reader;
    std::ofstream writer;
    std::ofstream errorWriter;
    std::string code;

    // 将文件内容读取为字符串
    std::string transferFileToCode();

public:
    // 构造函数，初始化文件读取和写入操作
    FileProcessor();

    std::ifstream& getReader() { return reader; }

    // 获取代码内容
    std::string getCode() const;

    // 获取输出文件流
    std::ofstream& getWriter();

    // 获取错误写入文件流
    std::ofstream& getErrorWriter();
};