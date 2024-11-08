#pragma once

#include <string>
#include "KeyWordMap.h" // ����KeyWordMap�Ѿ������

class Word {
private:
    std::string identification;
    std::string content;
    std::string type;
    int lineNum;

    bool isLegal(char c) const;

public:
    // ���캯��
    Word() { type = "unknown"; }
    Word(const std::string& identification, int lineNum);
    Word(char identification, int lineNum);
    Word(const std::string& type, const std::string& content, int lineNum);

    // ��������
    std::string toString() const;
    bool typeEquals(const std::string& str) const;
    bool typeSymbolizeStmt() const;
    bool typeSymbolizeValidateStmt() const;
    bool typeSymbolizeBeginOfExp() const;
    bool typeOfUnary() const;
    bool typeOfNotInExp() const;

    // Getters
    std::string getType() const;
    int getLineNum() const;
    std::string getContent() const;

    int getFormatNum() const;
    bool isFormatIllegal() const;
};