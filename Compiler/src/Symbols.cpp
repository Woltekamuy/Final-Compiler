#include "../include/Symbols.h"
#include <stdexcept>

// ���캯��ʵ��
Symbols::Symbols() {}

// ��ӷ���
void Symbols::addSymbol(const std::string& type, int intType, const Word& word, int areaID) {
    symbolHashMap[word.getContent()] = Symbol(type, intType, word, areaID);
}

// �������Ƿ����
bool Symbols::hasSymbol(const Word& word) const {
    return symbolHashMap.find(word.getContent()) != symbolHashMap.end();
}

// ��ȡ����
Symbol Symbols::getSymbol(const Word& word) const {
    auto it = symbolHashMap.find(word.getContent());
    if (it != symbolHashMap.end()) {
        return it->second;
    }
    throw std::runtime_error("Symbol not found");
}

// ����Ƿ�Ϊ����
bool Symbols::isConst(const Word& word) const {
    auto symbol = symbolHashMap.at(word.getContent());
    return (symbol.getType() == "const_int" || symbol.getType() == "const_char");
}

// ������з���
std::string Symbols::toString() const {
    std::string result = "{";
    for (const auto& pair : symbolHashMap) {
        result += pair.second.toString() + ", ";
    }
    if (result.length() > 1) {
        result.pop_back();  // ɾ������ ", "
        result.pop_back();
    }
    result += "}";
    return result;
}
