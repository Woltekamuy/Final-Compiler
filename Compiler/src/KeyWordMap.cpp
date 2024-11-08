#include "../include/KeyWordMap.h"

// ���캯��ʵ��
KeyWordMap::KeyWordMap() {
    keyWords["main"] = "MAINTK";
    keyWords["const"] = "CONSTTK";
    keyWords["int"] = "INTTK";
    keyWords["char"] = "CHARTK";
    keyWords["break"] = "BREAKTK";
    keyWords["continue"] = "CONTINUETK";
    keyWords["if"] = "IFTK";
    keyWords["else"] = "ELSETK";
    keyWords["!"] = "NOT";
    keyWords["&&"] = "AND";
    keyWords["||"] = "OR";
    keyWords["for"] = "FORTK";
    keyWords["while"] = "WHILETK";
    keyWords["getint"] = "GETINTTK";
    keyWords["getchar"] = "GETCHARTK";
    keyWords["printf"] = "PRINTFTK";
    keyWords["return"] = "RETURNTK";
    keyWords["+"] = "PLUS";
    keyWords["-"] = "MINU";
    keyWords["void"] = "VOIDTK";
    keyWords["*"] = "MULT";
    keyWords["/"] = "DIV";
    keyWords["%"] = "MOD";
    keyWords["<"] = "LSS";
    keyWords["<="] = "LEQ";
    keyWords[">"] = "GRE";
    keyWords[">="] = "GEQ";
    keyWords["=="] = "EQL";
    keyWords["!="] = "NEQ";
    keyWords["="] = "ASSIGN";
    keyWords[";"] = "SEMICN";
    keyWords[","] = "COMMA";
    keyWords["("] = "LPARENT";
    keyWords[")"] = "RPARENT";
    keyWords["["] = "LBRACK";
    keyWords["]"] = "RBRACK";
    keyWords["{"] = "LBRACE";
    keyWords["}"] = "RBRACE";
}

// ��ȡ�ؼ�������
std::string KeyWordMap::getType(const std::string& ident) const {
    auto it = keyWords.find(ident);
    if (it != keyWords.end()) {
        return it->second;
    }
    return ""; // ���ؿ��ַ�����ʾδ�ҵ�
}

// �ж��Ƿ�Ϊ�ؼ���
bool KeyWordMap::isKey(const std::string& str) const {
    return keyWords.find(str) != keyWords.end();
}
