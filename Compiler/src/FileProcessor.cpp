#include "../include/FileProcessor.h"
#include <iostream>

FileProcessor::FileProcessor()
{
    reader.open("testfile.txt");
    writer.open("pcoderesult.txt");
    errorWriter.open("error.txt");
    code = transferFileToCode();
}

// ���ļ����ݶ�ȡΪ�ַ���
std::string FileProcessor::transferFileToCode() {
    std::stringstream buffer;
    std::string line;
    while (std::getline(reader, line)) {
        buffer << line << "\n";
    }
    return buffer.str();
}

// ��ȡ��������
std::string FileProcessor::getCode() const {
    return code;
}

// ��ȡ����ļ���
std::ofstream& FileProcessor::getWriter() {
    return writer;
}

// ��ȡ����д���ļ���
std::ofstream& FileProcessor::getErrorWriter() {
    return errorWriter;
}
