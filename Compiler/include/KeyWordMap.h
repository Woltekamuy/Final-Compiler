#pragma once

#include <string>
#include <unordered_map>

class KeyWordMap {
private:
    std::unordered_map<std::string, std::string> keyWords;

public:
    KeyWordMap();  // ���캯��

    std::string getType(const std::string& ident) const;
    bool isKey(const std::string& str) const;
};
