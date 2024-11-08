#include "../include/Symbols.h"
#include <stdexcept>

// 构造函数实现
Symbols::Symbols() {}

// 添加符号
void Symbols::addSymbol(const std::string& type, int intType, const Word& word, int areaID) {
    symbolHashMap[word.getContent()] = Symbol(type, intType, word, areaID);
}

// 检查符号是否存在
bool Symbols::hasSymbol(const Word& word) const {
    return symbolHashMap.find(word.getContent()) != symbolHashMap.end();
}

// 获取符号
Symbol Symbols::getSymbol(const Word& word) const {
    auto it = symbolHashMap.find(word.getContent());
    if (it != symbolHashMap.end()) {
        return it->second;
    }
    throw std::runtime_error("Symbol not found");
}

// 检查是否为常量
bool Symbols::isConst(const Word& word) const {
    auto symbol = symbolHashMap.at(word.getContent());
    return (symbol.getType() == "const_int" || symbol.getType() == "const_char");
}

// 输出所有符号
std::string Symbols::toString() const {
    std::string result = "{";
    for (const auto& pair : symbolHashMap) {
        result += pair.second.toString() + ", ";
    }
    if (result.length() > 1) {
        result.pop_back();  // 删除最后的 ", "
        result.pop_back();
    }
    result += "}";
    return result;
}
