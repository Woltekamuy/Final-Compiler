#pragma once

#include <string>
#include <vector>
#include "Word.h"  // 引入Word类头文件

class Function {
private:
    std::string content;
    std::string returnType;
    std::vector<int> paras;

public:
    // 构造函数
    Function(){}
    Function(const Word& word, const std::string& returnType);

    // 获取参数列表
    std::vector<int> getParas() const;

    // 设置参数列表
    void setParas(const std::vector<int>& paras);

    // 获取函数名
    std::string getContent() const;

    // 获取返回类型
    std::string getReturnType() const;
};