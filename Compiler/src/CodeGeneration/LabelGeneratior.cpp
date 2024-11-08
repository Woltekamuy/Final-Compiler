#include "../../include/CodeGeneration/LabelGenerator.h"

std::string LabelGenerator::getLabel(const std::string& type) {
    count++;
    return "label_" + type + "_" + std::to_string(count);
}
