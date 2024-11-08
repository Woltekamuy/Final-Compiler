#pragma once

#include <iostream>
#include <string>

class Error {
private:
    int n;
    std::string type;

public:
    // 构造函数
    Error(int n, const std::string& type);

    // 获取整数 n
    int getN() const;

    // 重写toString方法
    std::string toString() const;
};

std::ostream& operator<<(std::ostream& os, const Error& error);