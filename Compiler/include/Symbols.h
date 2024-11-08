#pragma once


#include <unordered_map>
#include "Symbol.h"  // 引入Symbol类头文件
#include "Word.h"    // 引入Word类头文件

class Symbols {
private:
    std::unordered_map<std::string, Symbol> symbolHashMap;

public:
    Symbols();  // 构造函数

    // 添加符号
    void addSymbol(const std::string& type, int intType, const Word& word, int areaID);

    // 检查符号是否存在
    bool hasSymbol(const Word& word) const;

    // 获取符号
    Symbol getSymbol(const Word& word) const;

    // 检查是否为常量
    bool isConst(const Word& word) const;

    // 输出所有符号
    std::string toString() const;
};