#pragma once

#include <vector>
#include "Word.h"  // 引入Word类头文件

class Exps {
private:
    std::vector<std::vector<Word>> words;  // 存储二维Word数组
    std::vector<Word> symbols;             // 存储Word数组

public:
    // 构造函数
    Exps(const std::vector<std::vector<Word>>& words, const std::vector<Word>& symbols);

    // 获取words
    std::vector<std::vector<Word>> getWords() const;

    // 获取symbols
    std::vector<Word> getSymbols() const;
};