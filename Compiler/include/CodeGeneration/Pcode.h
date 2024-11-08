#pragma once

#include"CodeType.h"
#include <string>   // std::string
#include <iostream> // std::ostream, std::ostringstream
#include <sstream>  // std::ostringstream

enum class ValueType {
    INT,
    STRING,
    UNKNOWN
};
// PCode 类声明
class PCode {
private:
    
    CodeType type;    // 存储指令类型
    ValueType valuetype1;
    ValueType valuetype2;
    std::string value1 = "";  // 存储第一个值
    std::string value2 = "";  // 存储第二个值

public:
    // 构造函数
    explicit PCode(CodeType type); // 只有类型
    PCode(CodeType type, std::string value1,ValueType valuetype1); // 类型和一个值
    PCode(CodeType type, std::string value1, ValueType valuetype1, std::string value2, ValueType valuetype2); // 类型和两个值

    // 设置第二个值
    void setValue2(std::string value2,ValueType valuetype2);

    // 获取类型和值
    CodeType getType() const;
    std::string getValue1() const;
    std::string getValue2() const;
    ValueType getValue1Type() const;
    ValueType getValue2Type() const;

    // 转换为字符串
    std::string toString() const;
};