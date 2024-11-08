#include "../include/Symbol.h"

Symbol::Symbol(){}

// ���캯��ʵ��
Symbol::Symbol(const std::string& type, int intType, const Word& word, int areaID)
    : type(type), intType(intType), content(word.getContent()), areaID(areaID) {}

// ��ȡ����
std::string Symbol::getType() const {
    return type;
}

// ��ȡint����
int Symbol::getIntType() const {
    return intType;
}

// ��ȡ����ID
int Symbol::getAreaID() const {
    return areaID;
}

// ��дtoString����
std::string Symbol::toString() const {
    return content;
}
