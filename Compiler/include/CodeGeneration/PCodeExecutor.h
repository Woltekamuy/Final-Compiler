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

// PCodeExecutor ������
class PCodeExecutor {
private:
    std::vector<PCode> codes; // �洢 PCode ָ��
    std::vector<RetInfo> retInfos; // �洢������Ϣ
    std::vector<int> stack; // ջ
    int eip = 0; // ָ��ָ��
    std::unordered_map<std::string, Var> varTable; // ������
    std::unordered_map<std::string, Func> funcTable; // ������
    std::unordered_map<std::string, int> labelTable; // ��ǩ��
    int mainAddress; // ��������ַ
    std::vector<std::string> prints; // ��ӡ���

    std::ofstream& writer; // �ļ�д����

    std::string fibo="1_fibonacci";

public:
    PCodeExecutor(const std::vector<PCode>& codes, std::ofstream& writer);

    void run(); // ִ�д���
    void print(); // ��ӡ���

private:
    void push(int i);
    int pop();
    Var getVar(const std::string& ident);
    void pushVar(const std::string& ident, Var& var);
    int getAddress(Var& var, int intType);
};