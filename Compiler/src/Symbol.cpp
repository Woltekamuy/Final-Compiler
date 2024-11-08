#include "../include/Symbol.h"

Symbol::Symbol(){}

// 构造函数实现
Symbol::Symbol(const std::string& type, int intType, const Word& word, int areaID)
    : type(type), intType(intType), content(word.getContent()), areaID(areaID) {}

// 获取类型
std::string Symbol::getType() const {
    return type;
}

// 获取int类型
int Symbol::getIntType() const {
    return intType;
}

// 获取区域ID
int Symbol::getAreaID() const {
    return areaID;
}

// 重写toString方法
std::string Symbol::toString() const {
    return content;
}
