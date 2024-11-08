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
// PCode ������
class PCode {
private:
    
    CodeType type;    // �洢ָ������
    ValueType valuetype1;
    ValueType valuetype2;
    std::string value1 = "";  // �洢��һ��ֵ
    std::string value2 = "";  // �洢�ڶ���ֵ

public:
    // ���캯��
    explicit PCode(CodeType type); // ֻ������
    PCode(CodeType type, std::string value1,ValueType valuetype1); // ���ͺ�һ��ֵ
    PCode(CodeType type, std::string value1, ValueType valuetype1, std::string value2, ValueType valuetype2); // ���ͺ�����ֵ

    // ���õڶ���ֵ
    void setValue2(std::string value2,ValueType valuetype2);

    // ��ȡ���ͺ�ֵ
    CodeType getType() const;
    std::string getValue1() const;
    std::string getValue2() const;
    ValueType getValue1Type() const;
    ValueType getValue2Type() const;

    // ת��Ϊ�ַ���
    std::string toString() const;
};