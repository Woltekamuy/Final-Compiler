#pragma once


#include <unordered_map>
#include "Symbol.h"  // ����Symbol��ͷ�ļ�
#include "Word.h"    // ����Word��ͷ�ļ�

class Symbols {
private:
    std::unordered_map<std::string, Symbol> symbolHashMap;

public:
    Symbols();  // ���캯��

    // ��ӷ���
    void addSymbol(const std::string& type, int intType, const Word& word, int areaID);

    // �������Ƿ����
    bool hasSymbol(const Word& word) const;

    // ��ȡ����
    Symbol getSymbol(const Word& word) const;

    // ����Ƿ�Ϊ����
    bool isConst(const Word& word) const;

    // ������з���
    std::string toString() const;
};