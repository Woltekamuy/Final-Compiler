#include "../../include/CodeGeneration/Pcode.h"
#include <sstream>  // ���� std::ostringstream

// ���캯��ʵ��
PCode::PCode(CodeType type) : type(type) {}

PCode::PCode(CodeType type, std::string value1, ValueType valuetype1)
	: type(type), value1(value1), valuetype1(valuetype1), value2(""), valuetype2(ValueType::UNKNOWN) {
	// value2 �� valuetype2 Ĭ��Ϊ nullptr �� INT����ΪĬ��ֵ��
}

PCode::PCode(CodeType type, std::string value1, ValueType valuetype1, std::string value2, ValueType valuetype2)
	: type(type), value1(value1), valuetype1(valuetype1), value2(value2), valuetype2(valuetype2) {
	// ����ֵ������ʼ��
}

// ���õڶ���ֵ
void PCode::setValue2(std::string value2, ValueType valuetype2) {
	this->value2 = value2;
	this->valuetype2 = valuetype2;
}

// ��ȡ����
CodeType PCode::getType() const {
	return type;
}

// ��ȡ value1
std::string PCode::getValue1() const {
	return value1;
}

// ��ȡ value2
std::string PCode::getValue2() const {
	return value2;
}

ValueType PCode::getValue1Type() const
{
	return this->valuetype1;
}

ValueType PCode::getValue2Type() const
{
	return this->valuetype2;
}

// ת��Ϊ�ַ���
std::string PCode::toString() const {
    if (type == CodeType::LABEL) {
        return value1 + ": ";
    }
    if (type == CodeType::FUNC) {
        return "FUNC @" + value1 + ":";
    }
    if (type == CodeType::CALL) {
        return "$" + value1;
    }
    if (type == CodeType::PRINT) {
        return to_string(type) + " " + value1;
    }

    std::string a = !value1.empty() ? value1 : "";
    std::string b = !value2.empty() ? ", " + value2 : "";

    return to_string(type) + " " + a + b;
}
