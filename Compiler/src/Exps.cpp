#include "../include/Exps.h"

// 构造函数实现
Exps::Exps(const std::vector<std::vector<Word>>& words, const std::vector<Word>& symbols)
    : words(words), symbols(symbols) {}

// 获取words
std::vector<std::vector<Word>> Exps::getWords() const {
    return words;
}

// 获取symbols
std::vector<Word> Exps::getSymbols() const {
    return symbols;
}
