#include "../include/Error.h"

// 构造函数实现
Error::Error(int n, const std::string& type) : n(n), type(type) {}

// 获取整数 n
int Error::getN() const {
    return n;
}

// 重写toString方法
std::string Error::toString() const {
    return std::to_string(n) + " " + type;
}

std::ostream& operator<<(std::ostream& os, const Error& error) {
    os << error.toString();  // 使用 toString 方法获取字符串输出
    return os;  // 返回输出流对象，支持链式输出
}