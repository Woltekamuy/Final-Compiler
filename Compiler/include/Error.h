#pragma once

#include <iostream>
#include <string>

class Error {
private:
    int n;
    std::string type;

public:
    // ���캯��
    Error(int n, const std::string& type);

    // ��ȡ���� n
    int getN() const;

    // ��дtoString����
    std::string toString() const;
};

std::ostream& operator<<(std::ostream& os, const Error& error);