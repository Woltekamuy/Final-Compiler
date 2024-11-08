#pragma once

#include <string>
#include "Word.h"  // 引入Word类头文件

class Symbol {
private:
    std::string type;
    int intType;
    std::string content;
    int areaID;

public:
    // 构造函数
    Symbol();
    Symbol(const std::string& type, int intType, const Word& word, int areaID);

    // 获取类型
    std::string getType() const;

    // 获取int类型
    int getIntType() const;

    // 获取区域ID
    int getAreaID() const;

    // 重写toString方法
    std::string toString() const;
};