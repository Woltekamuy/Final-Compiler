#include "../../include/CodeGeneration/PCodeExecutor.h"


PCodeExecutor::PCodeExecutor(const std::vector<PCode>& codes, std::ofstream& writer)
    : codes(codes), writer(writer) {
    for (int i = 0; i < codes.size(); i++) {
        PCode code = codes[i];
        // 获取主函数地址
        if (code.getType() == CodeType::MAIN) {
            mainAddress = i;
        }
        // 获取标签
        if (code.getType() == CodeType::LABEL) {
            labelTable.emplace(code.getValue1(), i);
        }
        // 获取函数
        if (code.getType() == CodeType::FUNC) {
            funcTable.emplace(code.getValue1(), Func(i, std::stoi(code.getValue2())));
        }
    }
}

void PCodeExecutor::push(int i) {
    stack.push_back(i);
}

int PCodeExecutor::pop() {
    int val = stack.back();
    stack.pop_back();
    return val;
}

void PCodeExecutor::pushVar(const std::string& ident,Var& var) {
    if (varTable.find(ident) != varTable.end()) {
       varTable[ident]=var;
    }
    else {
       retInfos[0].getVarTable()[ident]=var;
    }
}

Var PCodeExecutor::getVar(const std::string& ident) {
    if (varTable.find(ident) != varTable.end()) {
        return varTable[ident];
    }
    else {
        return retInfos[0].getVarTable().at(ident);
    }
}

int PCodeExecutor::getAddress(Var& var, int intType) {
    int address = 0;
    int n = var.getDimension() - intType;
    if (n == 0) {
        address = var.getIndex();
    }
    if (n == 1) {
        int i = pop();
        if (var.getDimension() == 1) {
            address = var.getIndex() + i;
        }
        else {
            address = var.getIndex() + var.getDim2() * i;
        }
    }
    if (n == 2) {
        int j = pop();
        int i = pop();
        address = var.getIndex() + var.getDim2() * i + j;
    }
    return address;
}

void PCodeExecutor::run() {
    int callArgsNum = 0;
    int nowArgsNum = 0;
    bool mainFlag = false;
    std::vector<int> rparas;

    for (; eip < codes.size(); eip++) {
        //printf("%d %u\n", eip, stack.size());
        PCode code = codes[eip];
        switch (code.getType()) {
        case CodeType::LABEL:
            break;

        case CodeType::VAR: {
            Var var(stack.size());
            varTable.emplace(code.getValue1(), var);
            break;
        }

        case CodeType::PUSH: {
            auto value1 = code.getValue1();
            if (!value1.empty() && code.getValue1Type()==ValueType::INT) {
                push(std::stoi(code.getValue1()));
            }
            break;
        }
        case CodeType::POP: {
            int value = pop();
            int address = pop();
            stack[address] = value;
            break;
        }

        case CodeType::ADD:
        case CodeType::SUB:
        case CodeType::MUL:
        case CodeType::DIV:
        case CodeType::MOD: {
            int b = pop();
            int a = pop();
            if (code.getType() == CodeType::ADD) {
                push(a + b);
            }
            else if (code.getType() == CodeType::SUB) {
                push(a - b);
            }
            else if (code.getType() == CodeType::MUL) {
                push(a * b);
            }
            else if (code.getType() == CodeType::DIV) {
                push(a / b);
            }
            else if (code.getType() == CodeType::MOD) {
                push(a % b);
            }
            break;
        }

        case CodeType::CMPEQ: {
            int b = pop();
            int a = pop();
            push(a == b ? 1 : 0);
            break;
        }

        case CodeType::CMPNE: {
            int b = pop();
            int a = pop();
            push(a != b ? 1 : 0);
            break;
        }
        case CodeType::CMPLT: {
            int b = pop();
            int a = pop();
            push(a < b ? 1 : 0);
            break;
        }
        case CodeType::CMPLE: {
            int b = pop();
            int a = pop();
            push(a <= b ? 1 : 0);
            break;
        }
        case CodeType::CMPGT: {
            int b = pop();
            int a = pop();
            push(a > b ? 1 : 0);
            break;
        }
        case CodeType::CMPGE: {
            int b = pop();
            int a = pop();
            push(a >= b ? 1 : 0);
            break;
        }
        case CodeType::AND: {
            bool b = pop() != 0;
            bool a = pop() != 0;
            push((a && b) ? 1 : 0);
            break;
        }
        case CodeType::OR: {
            bool b = pop() != 0;
            bool a = pop() != 0;
            push((a || b) ? 1 : 0);
            break;
        }
        case CodeType::NOT: {
            bool a = pop() != 0;
            push((!a) ? 1 : 0);
            break;
        }
        case CodeType::NEG: {
            push(-pop());
            break;
        }
        case CodeType::POS: {
            push(pop());
            break;
        }
        case CodeType::JZ: {
            if (stack.back() == 0) {
                eip = labelTable[code.getValue1()];
            }
            break;
        }
        case CodeType::JNZ: {
            if (stack.back() != 0) {
                eip = labelTable[code.getValue1()];
            }
            break;
        }
        case CodeType::JMP: {
            eip = labelTable[code.getValue1()];
            break;
        }
        case CodeType::FUNC: {
            if (!mainFlag) {
                eip = mainAddress - 1;
            }
            break;
        }
        case CodeType::MAIN: {
            mainFlag = true;
            retInfos.emplace_back(codes.size(), varTable, stack.size() - 1, 0, 0, 0);
            varTable.clear();
            break;
        }
        case CodeType::PARA: {
            int num = rparas.size() - callArgsNum + nowArgsNum;
            Var para(rparas[num]);
            int n = std::stoi(code.getValue2());
            para.setDimension(n);
            if (n == 2) {
                para.setDim2(pop());
            }
            varTable.emplace(code.getValue1(), para);
            nowArgsNum++;
            if (nowArgsNum == callArgsNum) {
                rparas.erase(rparas.end() - callArgsNum, rparas.end());
            }
            break;
        }
        case CodeType::RET: {
            int n = std::stoi(code.getValue1());
            RetInfo& info = retInfos.back();
            eip = info.getEip();
            varTable = info.getVarTable();
            callArgsNum = info.getCallArgsNum();
            nowArgsNum = info.getNowArgsNum();
            if (n == 1) {
                int offset = info.getStackPtr() + 1 - info.getParaNum();
                stack.erase(stack.begin() + offset, stack.end() - 1);
            }
            else {
                stack.erase(stack.begin() + info.getStackPtr() + 1 - info.getParaNum(), stack.end());
            }
            retInfos.pop_back();
            break;
        }
        case CodeType::CALL: {
            Func func = funcTable.at(code.getValue1());
            retInfos.emplace_back(eip, varTable, stack.size() - 1, func.getArgs(), func.getArgs(), nowArgsNum);
            eip = func.getIndex();
            varTable.clear();
            callArgsNum = func.getArgs();
            nowArgsNum = 0;
            break;
        }
        case CodeType::RPARA: { 
            int n = std::stoi(code.getValue1());
            if (n == 0) {
                rparas.push_back(stack.size() - 1);
            }
            else {
                rparas.push_back(stack.back());
            }
            break;
        }
        case CodeType::GETINT: {
            int input;
            std::cin >> input;
            push(input);
            break;
        }
        case CodeType::GETCHAR: {
            char input;
            std::cin >> input;
            push(input % 256);
            break;
        }
        case CodeType::PRINT: {
            std::string s = code.getValue1();
            int n = std::stoi(code.getValue2());
            std::string result;
            std::vector<int> paras;

            for (int i = 0; i < n; i++) {
                paras.push_back(pop());
            }

            int index = n - 1;
            for (int i = 0; i < s.length(); i++) {
                if (i + 1 < s.length()) {
                    if (s[i] == '%' && s[i + 1] == 'd') {
                        result.append(std::to_string(paras[index--]));
                        i++;
                        continue;
                    }
                    if (s[i] == '%' && s[i + 1] == 'c') {
                        result.append(1, (char)paras[index--]);
                        i++;
                        continue;
                    }
                }
                result.append(1, s[i]);
            }
            prints.push_back(result.substr(1, result.length() - 2));
            break;
        }
        case CodeType::VALUE: {
            Var var = getVar(code.getValue1());
            int n = std::stoi(code.getValue2());
            int address = getAddress(var, n);
            push(stack[address]);
            break;
        }
        case CodeType::ADDRESS: {
            Var var = getVar(code.getValue1());
            int n = std::stoi(code.getValue2());
            int address = getAddress(var, n);
            push(address);
            break;
        }
        case CodeType::DIMVAR: {
            Var var = getVar(code.getValue1());
            int n = std::stoi(code.getValue2());
            var.setDimension(n);
            if (n == 1) {
                int i = pop();
                var.setDim1(i);
                pushVar(code.getValue1(), var);
            }
            if (n == 2) {
                int j = pop(), i = pop();
                var.setDim1(i);
                var.setDim2(j);
                pushVar(code.getValue1(), var);
            }
            break;
        }
        case CodeType::COMPLEMENT:{
            Var var = getVar(code.getValue1());
            int n = std::stoi(code.getValue2());
            for (int i = 0; i < var.getDim1()-n; i++) {
                push(0);
            }
            break;
        }
        case CodeType::PLACEHOLDER: {
            Var var = getVar(code.getValue1());
            int n = std::stoi(code.getValue2());
            if (n == 0) {
                push(0);
            }
            if (n == 1) {
                for (int i = 0; i < var.getDim1(); i++) {
                    push(0);
                }
            }
            if (n == 2) {
                for (int i = 0; i < var.getDim1() * var.getDim2(); i++) {
                    push(0);
                }
            }
            break;
        }
        case CodeType::EXIT: {
            return;
        }
        default:
            break;
        }
    }
}

void PCodeExecutor::print() {
    for (const std::string& s : prints) {
        writer.write(s.c_str(), s.length());
    }
    writer.flush();
}
