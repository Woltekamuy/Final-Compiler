#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "Word.h"
#include "KeyWordMap.h"

class LexicalAnalyser {
private:
    std::string code;
    int lineNum = 1;
    int index = 0;
    std::vector<Word> words;

    // 获取下一个字符
    char getChar();
    // 回退字符
    void unGetChar();

    // 词法分析
    void analyse();
    // 处理注释
    void analyseSlash();
    // 处理关系运算符
    void analyseRelation(char c);
    // 处理字符串
    void analyseCitation();
    // 处理逻辑运算符
    void analyseLogic(char pre);
    // 处理数字
    void analyseDigit(char pre);
    // 处理字符
    void analyseChar();
    // 处理字母和标识符
    void analyseLetter(char pre);

public:
    // 构造函数
    LexicalAnalyser();

    // 输出词法分析结果
    void printWords(std::ofstream& writer);

    // 获取分析结果
    std::vector<Word> getWords() const;
};