#include"../../include/CodeGeneration/CodeType.h"

std::string to_string(CodeType type) {
    switch (type) {
    case CodeType::LABEL:       return "LABEL";
    case CodeType::VAR:         return "VAR";
    case CodeType::PUSH:        return "PUSH";
    case CodeType::POP:         return "POP";
    case CodeType::ADD:         return "ADD";
    case CodeType::SUB:         return "SUB";
    case CodeType::MUL:         return "MUL";
    case CodeType::DIV:         return "DIV";
    case CodeType::MOD:         return "MOD";
    case CodeType::CMPEQ:       return "CMPEQ";
    case CodeType::CMPNE:       return "CMPNE";
    case CodeType::CMPGT:       return "CMPGT";
    case CodeType::CMPLT:       return "CMPLT";
    case CodeType::CMPGE:       return "CMPGE";
    case CodeType::CMPLE:       return "CMPLE";
    case CodeType::AND:         return "AND";
    case CodeType::OR:          return "OR";
    case CodeType::NOT:         return "NOT";
    case CodeType::NEG:         return "NEG";
    case CodeType::POS:         return "POS";
    case CodeType::JZ:          return "JZ";
    case CodeType::JNZ:         return "JNZ";
    case CodeType::JMP:         return "JMP";
    case CodeType::MAIN:        return "MAIN";
    case CodeType::FUNC:        return "FUNC";
    case CodeType::ENDFUNC:     return "ENDFUNC";
    case CodeType::PARA:        return "PARA";
    case CodeType::RET:         return "RET";
    case CodeType::CALL:        return "CALL";
    case CodeType::RPARA:       return "RPARA";
    case CodeType::GETINT:      return "GETINT";
    case CodeType::GETCHAR:     return "GETCHAR";
    case CodeType::PRINT:       return "PRINT";
    case CodeType::DIMVAR:      return "DIMVAR";
    case CodeType::VALUE:       return "VALUE";
    case CodeType::ADDRESS:     return "ADDRESS";
    case CodeType::PLACEHOLDER: return "PLACEHOLDER";
    case CodeType::COMPLEMENT:  return "COMPLEMENT";
    case CodeType::EXIT:        return "EXIT";
    default:                    return "UNKNOWN";
    }
}