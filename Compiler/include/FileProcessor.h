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

    // ���ļ����ݶ�ȡΪ�ַ���
    std::string transferFileToCode();

public:
    // ���캯������ʼ���ļ���ȡ��д�����
    FileProcessor();

    std::ifstream& getReader() { return reader; }

    // ��ȡ��������
    std::string getCode() const;

    // ��ȡ����ļ���
    std::ofstream& getWriter();

    // ��ȡ����д���ļ���
    std::ofstream& getErrorWriter();
};