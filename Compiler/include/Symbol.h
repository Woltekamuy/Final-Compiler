#pragma once

#include <string>
#include "Word.h"  // ����Word��ͷ�ļ�

class Symbol {
private:
    std::string type;
    int intType;
    std::string content;
    int areaID;

public:
    // ���캯��
    Symbol();
    Symbol(const std::string& type, int intType, const Word& word, int areaID);

    // ��ȡ����
    std::string getType() const;

    // ��ȡint����
    int getIntType() const;

    // ��ȡ����ID
    int getAreaID() const;

    // ��дtoString����
    std::string toString() const;
};