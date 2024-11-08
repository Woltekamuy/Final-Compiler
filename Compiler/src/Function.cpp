#include "../include/Function.h"

// ���캯��ʵ��
Function::Function(const Word& word, const std::string& returnType)
    : content(word.getContent()), returnType(returnType) {}

// ��ȡ�����б�
std::vector<int> Function::getParas() const {
    return paras;
}

// ���ò����б�
void Function::setParas(const std::vector<int>& paras) {
    this->paras = paras;
}

// ��ȡ������
std::string Function::getContent() const {
    return content;
}

// ��ȡ��������
std::string Function::getReturnType() const {
    return returnType;
}
