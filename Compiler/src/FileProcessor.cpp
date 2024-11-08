#include "../include/FileProcessor.h"
#include <iostream>

FileProcessor::FileProcessor()
{
    reader.open("testfile.txt");
    writer.open("pcoderesult.txt");
    errorWriter.open("error.txt");
    code = transferFileToCode();
}

// 将文件内容读取为字符串
std::string FileProcessor::transferFileToCode() {
    std::stringstream buffer;
    std::string line;
    while (std::getline(reader, line)) {
        buffer << line << "\n";
    }
    return buffer.str();
}

// 获取代码内容
std::string FileProcessor::getCode() const {
    return code;
}

// 获取输出文件流
std::ofstream& FileProcessor::getWriter() {
    return writer;
}

// 获取错误写入文件流
std::ofstream& FileProcessor::getErrorWriter() {
    return errorWriter;
}
