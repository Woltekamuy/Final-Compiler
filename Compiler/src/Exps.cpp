#include "../include/Exps.h"

// ���캯��ʵ��
Exps::Exps(const std::vector<std::vector<Word>>& words, const std::vector<Word>& symbols)
    : words(words), symbols(symbols) {}

// ��ȡwords
std::vector<std::vector<Word>> Exps::getWords() const {
    return words;
}

// ��ȡsymbols
std::vector<Word> Exps::getSymbols() const {
    return symbols;
}
