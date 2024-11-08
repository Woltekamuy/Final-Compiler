#include "../include/Function.h"

// 构造函数实现
Function::Function(const Word& word, const std::string& returnType)
    : content(word.getContent()), returnType(returnType) {}

// 获取参数列表
std::vector<int> Function::getParas() const {
    return paras;
}

// 设置参数列表
void Function::setParas(const std::vector<int>& paras) {
    this->paras = paras;
}

// 获取函数名
std::string Function::getContent() const {
    return content;
}

// 获取返回类型
std::string Function::getReturnType() const {
    return returnType;
}
