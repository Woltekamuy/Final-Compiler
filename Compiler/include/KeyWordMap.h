#pragma once

#include <string>
#include <unordered_map>

class KeyWordMap {
private:
    std::unordered_map<std::string, std::string> keyWords;

public:
    KeyWordMap();  // ¹¹Ôìº¯Êý

    std::string getType(const std::string& ident) const;
    bool isKey(const std::string& str) const;
};
