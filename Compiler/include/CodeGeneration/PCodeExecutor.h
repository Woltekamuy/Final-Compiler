#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "Pcode.h"
#include "Var.h"
#include "Func.h"
#include "RetInfo.h"
#include"../FileProcessor.h"

// PCodeExecutor 类声明
class PCodeExecutor {
private:
    std::vector<PCode> codes; // 存储 PCode 指令
    std::vector<RetInfo> retInfos; // 存储返回信息
    std::vector<int> stack; // 栈
    int eip = 0; // 指令指针
    std::unordered_map<std::string, Var> varTable; // 变量表
    std::unordered_map<std::string, Func> funcTable; // 函数表
    std::unordered_map<std::string, int> labelTable; // 标签表
    int mainAddress; // 主函数地址
    std::vector<std::string> prints; // 打印结果

    std::ofstream& writer; // 文件写入器

    std::string fibo="1_fibonacci";

public:
    PCodeExecutor(const std::vector<PCode>& codes, std::ofstream& writer);

    void run(); // 执行代码
    void print(); // 打印输出

private:
    void push(int i);
    int pop();
    Var getVar(const std::string& ident);
    void pushVar(const std::string& ident, Var& var);
    int getAddress(Var& var, int intType);
};