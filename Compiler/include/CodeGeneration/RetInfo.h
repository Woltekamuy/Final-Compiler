#pragma once

#include <string>
#include <unordered_map>
#include "../../include/CodeGeneration/Var.h"

class RetInfo {
private:
    int eip;
    std::unordered_map<std::string, Var> varTable;
    int stackPtr;
    int paraNum;
    int callArgsNum;
    int nowArgsNum;

public:
    RetInfo(int eip, const std::unordered_map<std::string, Var>& varTable, int stackPtr, int paraNum, int callArgsNum, int nowArgsNum);

    int getEip() const;
    std::unordered_map<std::string, Var>& getVarTable();
    int getStackPtr() const;
    int getParaNum() const;
    int getCallArgsNum() const;
    int getNowArgsNum() const;
};