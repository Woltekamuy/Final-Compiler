#include "../include/LexicalAnalyser.h"
#include "../include/FileProcessor.h"

// 构造函数，读取文件并进行词法分析
LexicalAnalyser::LexicalAnalyser()  {
    code = FileProcessor().getCode();  // 假设FileProcessor类已经正确实现
    analyse();
    words.push_back(Word());
}

// 获取下一个字符
char LexicalAnalyser::getChar() {
    if (index < code.length()) {
        char c = code[index];
        if (c == '\n') {
            lineNum++;
        }
        index++;
        return c;
    }
    else {
        return '\0';  // 文件结束
    }
}

// 回退字符
void LexicalAnalyser::unGetChar() {
    index--;
    char c = code[index];
    if (c == '\n') {
        lineNum--;
    }
}

// 进行词法分析
void LexicalAnalyser::analyse() {
    char c;
    while ((c = getChar()) != '\0') {
        if (c == ' ' || c == '\r' || c == '\t') {
            continue;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '%') {
            words.push_back(Word(c, lineNum));
        }
        else if (c == '/') {
            analyseSlash();
        }
        else if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') {
            words.push_back(Word(c, lineNum));
        }
        else if (c == '>' || c == '<' || c == '=' || c == '!') {
            analyseRelation(c);
        }
        else if (c == ',' || c == ';') {
            words.push_back(Word(c, lineNum));
        }
        else if (c == '"') {
            analyseCitation();
        }
        else if (c == '\'') {
            analyseChar();
        }
        else if (c == '&' || c == '|') {
            analyseLogic(c);
        }
        else if (isdigit(c)) {
            analyseDigit(c);
        }
        else if (isalpha(c) || c == '_') {
            analyseLetter(c);
        }
    }
}

// 处理注释
void LexicalAnalyser::analyseSlash() {
    char c = getChar();
    if (c == '/') {
        while ((c = getChar()) != '\0' && c != '\n') {}
    }
    else if (c == '*') {
        while ((c = getChar()) != '\0') {
            if (c == '*') {
                c = getChar();
                if (c == '/') {
                    return;
                }
                else {
                    unGetChar();
                }
            }
        }
    }
    else {
        words.push_back(Word("/", lineNum));
        unGetChar();
    }
}

// 处理关系运算符
void LexicalAnalyser::analyseRelation(char c) {
    if (c == '=') {
        c = getChar();
        if (c == '=') {
            words.push_back(Word("==", lineNum));
        }
        else {
            unGetChar();
            words.push_back(Word("=", lineNum));
        }
    }
    else if (c == '<') {
        c = getChar();
        if (c == '=') {
            words.push_back(Word("<=", lineNum));
        }
        else {
            unGetChar();
            words.push_back(Word("<", lineNum));
        }
    }
    else if (c == '>') {
        c = getChar();
        if (c == '=') {
            words.push_back(Word(">=", lineNum));
        }
        else {
            unGetChar();
            words.push_back(Word(">", lineNum));
        }
    }
    else {
        c = getChar();
        if (c == '=') {
            words.push_back(Word("!=", lineNum));
        }
        else {
            unGetChar();
            words.push_back(Word("!", lineNum));
        }
    }
}

// 处理字符串
void LexicalAnalyser::analyseCitation() {
    char c;
    std::string buffer = "";
    int flag = 0;
    while ((c = getChar()) != '\0') {
        if (c == '"') {
            words.push_back(Word("STRCON", "\"" + buffer + "\"", lineNum));
            return;
        }
        else {
            if (c == '\\') {
                flag = 1;
            }
            else {
                if (flag > 0 && c == 'n') {
                    buffer += "\n";
                }
                else {
                    buffer += c;
                }
                flag = 0;
            }
        }
    }
}

// 处理逻辑运算符
void LexicalAnalyser::analyseLogic(char pre) {
    char c;
    if ((c = getChar()) != '\0') {
        if (pre == '&') {
            if (c == '&') {
                words.push_back(Word("&&", lineNum));
            }
            else {
                unGetChar();
                words.push_back(Word("&", lineNum));
            }
        }
        else {
            if (c == '|') {
                words.push_back(Word("||", lineNum));
            }
            else {
                unGetChar();
                words.push_back(Word("|", lineNum));
            }
        }
    }
}

// 处理数字
void LexicalAnalyser::analyseDigit(char pre) {
    std::string builder(1, pre);
    char c;
    while ((c = getChar()) != '\0') {
        if (isdigit(c)) {
            builder += c;
        }
        else {
            unGetChar();
            words.push_back(Word("INTCON", builder, lineNum));
            return;
        }
    }
}

// 处理字符
void LexicalAnalyser::analyseChar() {
    std::string charStr(1, getChar());
    if (charStr == "\\") {
        char c = getChar();
        switch (c) {
        case 'a': c = (char)7; break;
        case 'b': c = (char)8; break;
        case 't': c = (char)9; break;
        case 'n': c = (char)10; break;
        case 'v': c = (char)11; break;
        case 'f': c = (char)12; break;
        case '"': c = (char)34; break;
        case '\'': c = (char)39; break;
        case '\\': c = (char)92; break;
        case '0': c = (char)0; break;
        }
    }
    words.push_back(Word("CHARCON", charStr, lineNum));
    getChar();
}

// 处理字母和标识符
void LexicalAnalyser::analyseLetter(char pre) {
    std::string builder(1, pre);
    char c;
    while ((c = getChar()) != '\0') {
        if (isalnum(c) || c == '_') {
            builder += c;
        }
        else {
            unGetChar();
            if (KeyWordMap().isKey(builder)) {
                words.push_back(Word(builder, lineNum));
            }
            else {
                words.push_back(Word("IDENFR", builder, lineNum));
            }
            return;
        }
    }
}

// 输出词法分析结果到文件
void LexicalAnalyser::printWords(std::ofstream& writer) {
    for (const Word& word : words) {
        writer << word.toString() << "\n";
    }
    writer.flush();
    writer.close();
}

// 获取分析结果
std::vector<Word> LexicalAnalyser::getWords() const {
    return words;
}
