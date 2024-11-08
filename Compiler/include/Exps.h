#pragma once

#include <vector>
#include "Word.h"  // ����Word��ͷ�ļ�

class Exps {
private:
    std::vector<std::vector<Word>> words;  // �洢��άWord����
    std::vector<Word> symbols;             // �洢Word����

public:
    // ���캯��
    Exps(const std::vector<std::vector<Word>>& words, const std::vector<Word>& symbols);

    // ��ȡwords
    std::vector<std::vector<Word>> getWords() const;

    // ��ȡsymbols
    std::vector<Word> getSymbols() const;
};