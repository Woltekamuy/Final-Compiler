#pragma once

#include <string>
#include <vector>
#include "Word.h"  // ����Word��ͷ�ļ�

class Function {
private:
    std::string content;
    std::string returnType;
    std::vector<int> paras;

public:
    // ���캯��
    Function(){}
    Function(const Word& word, const std::string& returnType);

    // ��ȡ�����б�
    std::vector<int> getParas() const;

    // ���ò����б�
    void setParas(const std::vector<int>& paras);

    // ��ȡ������
    std::string getContent() const;

    // ��ȡ��������
    std::string getReturnType() const;
};