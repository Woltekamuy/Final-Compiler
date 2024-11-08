#include "../include/KeyWordMap.h"

// 构造函数实现
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

// 获取关键词类型
std::string KeyWordMap::getType(const std::string& ident) const {
    auto it = keyWords.find(ident);
    if (it != keyWords.end()) {
        return it->second;
    }
    return ""; // 返回空字符串表示未找到
}

// 判断是否为关键词
bool KeyWordMap::isKey(const std::string& str) const {
    return keyWords.find(str) != keyWords.end();
}
