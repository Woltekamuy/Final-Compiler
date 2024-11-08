#include "../include/Error.h"

// ���캯��ʵ��
Error::Error(int n, const std::string& type) : n(n), type(type) {}

// ��ȡ���� n
int Error::getN() const {
    return n;
}

// ��дtoString����
std::string Error::toString() const {
    return std::to_string(n) + " " + type;
}

std::ostream& operator<<(std::ostream& os, const Error& error) {
    os << error.toString();  // ʹ�� toString ������ȡ�ַ������
    return os;  // �������������֧����ʽ���
}