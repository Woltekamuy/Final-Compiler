#include "../../include/CodeGeneration/RetInfo.h"

RetInfo::RetInfo(int eip, const std::unordered_map<std::string, Var>& varTable, int stackPtr, int paraNum, int callArgsNum, int nowArgsNum)
    : eip(eip), varTable(varTable), stackPtr(stackPtr), paraNum(paraNum), callArgsNum(callArgsNum), nowArgsNum(nowArgsNum) {}

int RetInfo::getEip() const {
    return eip;
}

std::unordered_map<std::string, Var>& RetInfo::getVarTable()  {
    return varTable;
}

int RetInfo::getStackPtr() const {
    return stackPtr;
}

int RetInfo::getParaNum() const {
    return paraNum;
}

int RetInfo::getCallArgsNum() const {
    return callArgsNum;
}

int RetInfo::getNowArgsNum() const {
    return nowArgsNum;
}
