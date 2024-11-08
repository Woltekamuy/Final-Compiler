#pragma once

#include <string>

class LabelGenerator {
private:
    int count = 0;

public:
    std::string getLabel(const std::string& type);
};