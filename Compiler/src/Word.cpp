#include "../include/Word.h"

// ���캯��ʵ��
Word::Word(const std::string& identification, int lineNum)
    : identification(identification), lineNum(lineNum) {
    KeyWordMap keyWordMap;
    this->type = keyWordMap.getType(this->identification);
    this->content = this->identification;
}

Word::Word(char identification, int lineNum)
    : identification(1, identification), lineNum(lineNum) {
    KeyWordMap keyWordMap;
    this->type = keyWordMap.getType(this->identification);
    this->content = this->identification;
}

Word::Word(const std::string& type, const std::string& content, int lineNum)
    : type(type), content(content), lineNum(lineNum) {}

// ��������ʵ��
std::string Word::toString() const {
    return type + " " + content;
}

bool Word::typeEquals(const std::string& str) const {
    return type == str;
}

bool Word::typeSymbolizeStmt() const {
    return type == "IDENFR" || type == "LBRACE" || type == "IFTK" ||
        type == "ELSETK" || type == "WHILETK" || type == "FORTK" ||
        type == "BREAKTK" || type == "CONTINUETK" || type == "RETURNTK" ||
        type == "PRINTFTK" || type == "SEMICN" || typeSymbolizeBeginOfExp();
}

bool Word::typeSymbolizeValidateStmt() const {
    return type == "IFTK" || type == "ELSETK" || type == "FORTK" ||
        type == "WHILETK" || type == "BREAKTK" || type == "CONTINUETK" ||
        type == "RETURNTK" || type == "PRINTFTK" || type == "SEMICN";
}

bool Word::typeSymbolizeBeginOfExp() const {
    return type == "LPARENT" || type == "IDENFR" || type == "INTCON" ||
        type == "CHARCON" || type == "NOT" || type == "PLUS" ||
        type == "MINU";
}

bool Word::typeOfUnary() const {
    return type == "PLUS" || type == "MINU" || type == "NOT";
}

bool Word::typeOfNotInExp() const {
    return type == "CONSTTK" || type == "INTTK" || type == "CHARTK" ||
        type == "BREAKTK" || type == "CONTINUETK" || type == "IFTK" ||
        type == "ELSETK" || type == "WHILETK" || type == "GETINTTK" ||
        type == "GETCHARTK" || type == "PRINTFTK" || type == "RETURNTK";
}

// Getters
std::string Word::getType() const {
    return type;
}

int Word::getLineNum() const {
    return lineNum;
}

std::string Word::getContent() const {
    return content;
}

// ͳ�Ƹ�ʽ���ַ����е�%d����
int Word::getFormatNum() const {
    int n = 0;
    for (size_t i = 0; i < content.length(); ++i) {
        if (i + 1 < content.length() && content[i] == '%' && content[i + 1] == 'd') {
            n++;
        }
    }
    return n;
}

// ����ʽ�Ƿ�Ϸ�
bool Word::isFormatIllegal() const {
    for (size_t i = 1; i < content.length() - 1; ++i) {
        char c = content[i];
        if (!isLegal(c)) {
            if (c == '%' && content[i + 1] == 'd') {
                continue;
            }
            return true;
        }
        else {
            if (c == '\\' && content[i + 1] != 'n') {
                return true;
            }
        }
    }
    return false;
}

// �ж��ַ��Ƿ�Ϸ�
bool Word::isLegal(char c) const {
    return c == 32 || c == 33 || (c >= 40 && c <= 126);
}
