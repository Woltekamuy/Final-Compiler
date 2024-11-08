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

    // ��ȡ��һ���ַ�
    char getChar();
    // �����ַ�
    void unGetChar();

    // �ʷ�����
    void analyse();
    // ����ע��
    void analyseSlash();
    // �����ϵ�����
    void analyseRelation(char c);
    // �����ַ���
    void analyseCitation();
    // �����߼������
    void analyseLogic(char pre);
    // ��������
    void analyseDigit(char pre);
    // �����ַ�
    void analyseChar();
    // ������ĸ�ͱ�ʶ��
    void analyseLetter(char pre);

public:
    // ���캯��
    LexicalAnalyser();

    // ����ʷ��������
    void printWords(std::ofstream& writer);

    // ��ȡ�������
    std::vector<Word> getWords() const;
};