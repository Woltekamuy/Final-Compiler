#pragma once
#include <string>

enum class CodeType {
	LABEL,
	VAR,
	PUSH,
	POP,
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	CMPEQ,
	CMPNE,
	CMPGT,
	CMPLT,
	CMPGE,
	CMPLE,
	AND,
	OR,
	NOT,
	NEG,
	POS,
	JZ,
	JNZ,
	JMP,
	MAIN,
	FUNC,
	ENDFUNC,
	PARA,
	RET,
	CALL,
	RPARA,
	GETINT,
	GETCHAR,
	PRINT,
	DIMVAR,
	VALUE,
	ADDRESS,
	PLACEHOLDER,
	COMPLEMENT,
	EXIT
};

// �� CodeType ת��Ϊ�ַ����ĺ���
std::string to_string(CodeType type);