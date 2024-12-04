#include "../include/GrammaticalAnalyser.h"
#include <fstream>  
#include <stdexcept>
#include <algorithm>
#include <map>
#include "../include/Exps.h"


GrammaticalAnalyser::GrammaticalAnalyser(const std::vector<Word>& words){
    this->words = words;
    grammar = std::vector<std::string>();
    analyseCompUnit();
}

void GrammaticalAnalyser::analyseCompUnit() {
    addArea();

    Word word = getNextWord();
    while (word.typeEquals("CONSTTK") || (
        (word.typeEquals("INTTK") || word.typeEquals("CHARTK")) && getNext2Word().typeEquals("IDENFR") && !getNext3Word().typeEquals("LPARENT"))) {
        analyseDecl();
        word = getNextWord();
    }
    while (word.typeEquals("VOIDTK") || (
        ((word.typeEquals("INTTK") || word.typeEquals("CHARTK")) && !getNext2Word().typeEquals("MAINTK")))) {
        analyseFuncDef();
        word = getNextWord();
    }
    if (word.typeEquals("INTTK") && getNext2Word().typeEquals("MAINTK")) {
        analyseMainFuncDef();
    }
    else {
        error();
    }

    removeArea();
    grammar.push_back("<CompUnit>");
}

void GrammaticalAnalyser::analyseDecl() {
    Word word = getNextWord();
    if (word.typeEquals("CONSTTK")) {
        analyseConstDecl();
    }
    else if (word.typeEquals("INTTK") || word.typeEquals("CHARTK")) {
        if (word.typeEquals("CHARTK")) {
            is_char = true;
        }
        analyseVarDecl();
        is_char = false;
    }
    else {
        error();
    }
}

void GrammaticalAnalyser::analyseFuncDef() {
    int startIndex = index;
    
    std::vector<int> paras;
    std::string returnType = analyseFuncType();
    getWord(); // Ident
    if (functions.find(curWord.getContent()) != functions.end()) {
        error("b");
    }
    PCode code(CodeType::FUNC, curWord.getContent(), ValueType::STRING);
    int index = codes.size();
    codes.push_back(code);
    Function function(curWord, returnType);
    addArea();

    getWord(); // (
    Word word = getNextWord();
    if (word.typeEquals("VOIDTK") || word.typeEquals("INTTK") || word.typeEquals("CHARTK")) {
        paras = analyseFuncFParams();
    }
    checkParent(); // )
    function.setParas(paras);
    functions[function.getContent()] = function;
    needReturn = function.getReturnType() == "int";
    bool isReturn = analyseBlock(true);
    if (needReturn && !isReturn) {
        error("g");
    }

    removeArea();
    codes[index].setValue2(transInt(paras.size()), ValueType::INT);
    codes.push_back(PCode(CodeType::RET, transInt(0), ValueType::INT));
    codes.push_back(PCode(CodeType::ENDFUNC));
    grammar.push_back("<FuncDef>");
}


void GrammaticalAnalyser::analyseConstDef() {
    getWord(); // Ident
    Word ident = curWord;
    if (hasSymbolInThisArea(curWord)) {
        error("b");
    }
    codes.push_back(PCode(CodeType::VAR, transInt(areaID)+"_" + curWord.getContent(), ValueType::STRING));
    int intType = 0;
    Word word = getNextWord();
    while (word.typeEquals("LBRACK")) {
        intType++;
        getWord(); // [
        auto exp = getExp();
        analyseConstExp(exp);
        checkBrack(); // ]
        word = getNextWord();
    }
    if (intType > 0) {
        codes.push_back(PCode(CodeType::DIMVAR, transInt(areaID)+"_" + ident.getContent(), ValueType::STRING,
            transInt(intType),ValueType::INT));
    }
    if (is_char) {
        addSymbol(ident, "const_char", intType, areaID);
    }
    else {
        addSymbol(ident, "const_int", intType, areaID);
    }
    getWord(); // =
    initialNum = -1;
    analyseConstInitVal();
    if (intType > 0) {
        codes.push_back(PCode(CodeType::COMPLEMENT, transInt(areaID) + "_" + ident.getContent(), ValueType::STRING,
            transInt(initialNum), ValueType::INT));
    }
    initialNum = -1;
    grammar.push_back("<ConstDef>");
}

void GrammaticalAnalyser::analyseMainFuncDef() {
    getWord(); // int
    getWord(); // main
    if (functions.find(curWord.getContent()) != functions.end()) {
        error("b");
    }
    else {
        Function function(curWord, "int");
        function.setParas({});
        functions["main"] = function;
    }
    codes.push_back(PCode(CodeType::MAIN,curWord.getContent(),ValueType::STRING));
    getWord(); // (
    checkParent(); // )
    needReturn = true;
    bool isReturn = analyseBlock(false);
    if (needReturn && !isReturn) {
        error("g");
    }
    codes.push_back(PCode(CodeType::EXIT));
    grammar.push_back("<MainFuncDef>");
}

bool GrammaticalAnalyser::analyseBlock(bool fromFunc) {
    getWord(); // {
    if (!fromFunc) {
        addArea();
    }
    Word word = getNextWord();
    bool isReturn = false;
    while (word.typeEquals("CONSTTK") || word.typeEquals("INTTK") ||
        word.typeEquals("CHARTK") || word.typeSymbolizeStmt()) {
        if (word.typeEquals("CONSTTK") || word.typeEquals("INTTK") || word.typeEquals("CHARTK")) {
            isReturn = analyseBlockItem();
        }
        else {
            isReturn = analyseStmt();
        }
        word = getNextWord();
    }
    getWord(); // }
    if (!fromFunc) {
        removeArea();
    }
    grammar.push_back("<Block>");
    return isReturn;
}

bool GrammaticalAnalyser::analyseBlockItem() {
    Word word = getNextWord();
    bool isReturn = false;
    if (word.typeEquals("CONSTTK") || word.typeEquals("INTTK") || word.typeEquals("CHARTK")) {
        analyseDecl();
    }
    else {
        isReturn = analyseStmt();
    }
    return isReturn;
}

bool GrammaticalAnalyser::analyseStmt() {
    bool isReturn = false;
    Word word = getNextWord();

    if (word.typeEquals("IDENFR")) {
        std::vector<Word> exp = getExp();
        if (getNextWord().typeEquals("ASSIGN")) {
            Word ident = exp[0];
            int intType = analyseLVal(exp);
            codes.emplace_back(PCode(CodeType::ADDRESS,
                transInt(getSymbol(ident).getAreaID()) + "_" + ident.getContent(), ValueType::STRING,
                   transInt(intType),ValueType::INT));
            if (isConst(word)) {
                error("h", word.getLineNum());
            }
            getWord(); // =
            if (getNextWord().typeEquals("GETINTTK")) {
                getWord(); // getint
                getWord(); // (
                checkParent(); // )
                checkSemicn(); // ;
                codes.emplace_back(PCode(CodeType::GETINT));
            }
            else if (getNextWord().typeEquals("GETCHARTK")) {
                getWord(); // getchar
                getWord(); // (
                checkParent(); // )
                checkSemicn(); // ;
                codes.emplace_back(PCode(CodeType::GETCHAR));
            }
            else {
                auto exp = getExp();
                analyseExp(exp);
                checkSemicn(); // ;
                if (getSymbol(ident).getType() == "var_char" || getSymbol(ident).getType() == "const_char") {
                    truncation(true);
                }
            }
            codes.emplace_back(PCode(CodeType::POP,
                transInt(getSymbol(ident).getAreaID()) + "_" + ident.getContent(), ValueType::STRING));
        }
        else {
            analyseExp(exp);
            checkSemicn(); // ;
        }
    }
    else if (word.typeSymbolizeBeginOfExp()) {
        auto exp = getExp();
        analyseExp(exp);
        checkSemicn(); // ;
    }
    else if (word.typeEquals("LBRACE")) {
        analyseBlock(false);
    }
    else if (word.typeEquals("IFTK")) {
        ifLabels.push_back({
            {"if", labelGenerator.getLabel("if")},
            {"else", labelGenerator.getLabel("else")},
            {"if_end", labelGenerator.getLabel("if_end")},
            {"if_block", labelGenerator.getLabel("if_block")}
            });
        
        codes.emplace_back(PCode(CodeType::LABEL, 
            ifLabels.back()["if"],ValueType::STRING));
        getWord(); // if
        getWord(); // (
        analyseCond("IFTK");
        checkParent(); // )
        codes.emplace_back(PCode(CodeType::JZ, 
           ifLabels.back()["else"], ValueType::STRING));
        codes.emplace_back(PCode(CodeType::LABEL, 
            ifLabels.back()["if_block"], ValueType::STRING));
        analyseStmt();
        word = getNextWord();
        codes.emplace_back(PCode(CodeType::JMP, 
            ifLabels.back()["if_end"], ValueType::STRING));
        codes.emplace_back(PCode(CodeType::LABEL,
          ifLabels.back()["else"], ValueType::STRING));
        if (word.typeEquals("ELSETK")) {
            getWord(); // else
            analyseStmt();
        }
        codes.emplace_back(PCode(CodeType::LABEL, 
           ifLabels.back()["if_end"], ValueType::STRING));
        ifLabels.pop_back();
    }
    else if (word.typeEquals("FORTK")) {
        forLabels.emplace_back(std::unordered_map<std::string, std::string>{
            {"for", labelGenerator.getLabel("for")},
            { "for_end", labelGenerator.getLabel("for_end") },
            { "for_init", labelGenerator.getLabel("for_init") },
            { "for_cond", labelGenerator.getLabel("for_cond") },
            { "for_block", labelGenerator.getLabel("for_block") },
            { "for_inc", labelGenerator.getLabel("for_inc") }
        });

        getWord(); // for
        whileFlag++;
        getWord(); // (
        codes.emplace_back(PCode(CodeType::LABEL, 
           forLabels.back()["for_init"],ValueType::STRING));
        if (!getNextWord().typeEquals("SEMICN")) {
            analyseStmt();
        }
        else {
            getWord();
        }
        codes.emplace_back(PCode(CodeType::LABEL, 
           forLabels.back()["for_cond"], ValueType::STRING));
        if (!getNextWord().typeEquals("SEMICN")) {
            analyseCond("FORTK");
            codes.emplace_back(PCode(CodeType::JZ, 
                forLabels.back()["for_end"], ValueType::STRING));
        }
        else {
            getWord();
        }
        checkSemicn();
        codes.emplace_back(PCode(CodeType::JMP,
         forLabels.back()["for_block"], ValueType::STRING));
        codes.emplace_back(PCode(CodeType::LABEL, 
           forLabels.back()["for_inc"], ValueType::STRING));
        if (!getNextWord().typeEquals("RPARENT")) {
            last_for_stmt = true;
            analyseStmt();
            last_for_stmt = false;
        }
        checkParent();
        codes.emplace_back(PCode(CodeType::JMP, 
          forLabels.back()["for_cond"], ValueType::STRING));
        codes.emplace_back(PCode(CodeType::LABEL,
         forLabels.back()["for_block"], ValueType::STRING));
        analyseStmt();
        codes.emplace_back(PCode(CodeType::JMP,
          forLabels.back()["for_inc"], ValueType::STRING));
        codes.emplace_back(PCode(CodeType::LABEL, 
           forLabels.back()["for_end"], ValueType::STRING));
        forLabels.pop_back();
        whileFlag--;
    }
    else if (word.typeEquals("BREAKTK")) {
        getWord(); // break
        codes.emplace_back(PCode(CodeType::JMP, 
         forLabels.back()["for_end"], ValueType::STRING));
        if (whileFlag == 0) {
            error("m");
        }
        checkSemicn(); // ;
    }
    else if (word.typeEquals("CONTINUETK")) {
        getWord(); // continue
        codes.emplace_back(PCode(CodeType::JMP, 
          forLabels.back()["for_inc"], ValueType::STRING));
        if (whileFlag == 0) {
            error("m");
        }
        checkSemicn(); // ;
    }
    else if (word.typeEquals("RETURNTK")) {
        bool flag = false;
        getWord(); // return
        isReturn = true;
        if (getNextWord().typeSymbolizeBeginOfExp()) {
            if (!needReturn) {
                error("f");
            }
            auto expp = getExp();
            analyseExp(expp);
            flag = true;
        }
        checkSemicn(); // ;
        codes.emplace_back(PCode(CodeType::RET, transInt(flag ? 1 : 0),ValueType::INT));
    }
    else if (word.typeEquals("PRINTFTK")) {
        getWord(); // printf
        Word printftk = curWord;
        getWord(); // (
        getWord(); // STRCON
        Word strcon = curWord;
        word = getNextWord();
        int para = 0;
        while (word.typeEquals("COMMA")) {
            getWord(); // ,
            auto expp = getExp();
            analyseExp(expp);
            para++;
            word = getNextWord();
        }
        if (strcon.isFormatIllegal()) {
            error("a", strcon.getLineNum());
        }
        if (para != strcon.getFormatNum()) {
            error("l", printftk.getLineNum());
        }
        checkParent(); // )
        checkSemicn(); // ;
        codes.emplace_back(PCode(CodeType::PRINT,
          strcon.getContent(), ValueType::STRING,
            transInt(para), ValueType::INT));
    }
    else if (word.typeEquals("SEMICN")) {
        getWord(); // ;
    }
    grammar.push_back("<Stmt>");
    return isReturn;
}

std::vector<int> GrammaticalAnalyser::analyseFuncFParams() {
    std::vector<int> paras;
    int paraType = analyseFuncFParam();
    paras.push_back(paraType);
    Word word = getNextWord();
    while (word.typeEquals("COMMA")) {
        getWord(); // ,
        paraType = analyseFuncFParam();
        paras.push_back(paraType);
        word = getNextWord();
    }
    grammar.push_back("<FuncFParams>");
    return paras;
}

int GrammaticalAnalyser::analyseFuncFParam() {
    int paraType = 0;
    getWord(); // void|int
    getWord(); // Ident
    Word ident = curWord;
    if (hasSymbolInThisArea(curWord)) {
        error("b");
    }
    Word word = getNextWord();
    if (word.typeEquals("LBRACK")) {
        paraType++;
        getWord(); // [
        checkBrack(); // ]
        word = getNextWord();
        while (word.typeEquals("LBRACK")) {
            paraType++;
            getWord(); // [
            auto expp = getExp();
            analyseConstExp(expp);
            checkBrack(); // ]
            word = getNextWord();
        }
    }
    codes.push_back(PCode(CodeType::PARA, 
        transInt(areaID)+"_" + ident.getContent(), ValueType::STRING,
       transInt(paraType),ValueType::INT));
    addSymbol(ident, "para", paraType, areaID);
    grammar.push_back("<FuncFParam>");
    return paraType;
}

std::string GrammaticalAnalyser::analyseFuncType() {
    getWord(); // void|int
    grammar.push_back("<FuncType>");
    return curWord.getContent();
}

void GrammaticalAnalyser::analyseConstInitVal() {
    initialNum++;
    Word word = getNextWord();
    if (word.typeEquals("LBRACE")) {
        getWord(); // {
        word = getNextWord();
        if (!word.typeEquals("RBRACE")) {
            analyseConstInitVal();
            Word word1 = getNextWord();
            while (word1.typeEquals("COMMA")) {
                getWord(); // ,
                analyseConstInitVal();
                word1 = getNextWord();
            }
        }
        getWord(); // }
    }
    else if (word.typeEquals("STRCON")) {
        std::string s = word.getContent();
        int length = s.length();
        for (int i = 1; i < length - 1; i++) {
            Word w("CHARCON", std::string(1, s[i]), word.getLineNum());
            std::vector<Word> exp = { w };
            analyseExp(exp);
            truncation(is_char);
        }
        getWord();
    }
    else {
        auto expp = getExp();
        analyseConstExp(expp);
        truncation(is_char);
    }
    grammar.push_back("<ConstInitVal>");
}

void GrammaticalAnalyser::analyseConstDecl() {
    getWord(); // const
    getWord(); // int
    if (curWord.typeEquals("INTTK") || curWord.typeEquals("CHARTK")) {
        if (curWord.typeEquals("CHARTK")) {
            is_char = true;
        }
        analyseBType();
    }
    else {
        error();
    }
    analyseConstDef();
    Word word = getNextWord();
    while (word.typeEquals("COMMA")) {
        getWord(); // ,
        analyseConstDef();
        word = getNextWord();
    }
    if (getNextWord().typeEquals("SEMICN")) {
        getWord(); // ;
    }
    else {
        error("i");
    }
    is_char = false;
    grammar.push_back("<ConstDecl>");
}

void GrammaticalAnalyser::analyseVarDecl() {
    getWord(); // int
    analyseVarDef();
    Word word = getNextWord();
    while (word.typeEquals("COMMA")) {
        getWord(); // ,
        analyseVarDef();
        word = getNextWord();
    }
    if (getNextWord().typeEquals("SEMICN")) {
        getWord(); // ;
    }
    else {
        error("i");
    }
    grammar.push_back("<VarDecl>");
}

void GrammaticalAnalyser::analyseVarDef() {
    getWord(); // Ident
    Word ident = curWord;
    if (hasSymbolInThisArea(curWord)) {
        error("b");
    }
    codes.push_back(PCode(CodeType::VAR, 
        transInt(areaID)+"_" + curWord.getContent(), ValueType::STRING));
    int intType = 0;
    Word word = getNextWord();
    while (word.typeEquals("LBRACK")) {
        intType++;
        getWord(); // [
        auto expp = getExp();
        analyseConstExp(expp);
        checkBrack(); // ]
        word = getNextWord();
    }
    if (intType > 0) {
        codes.push_back(PCode(CodeType::DIMVAR, 
            transInt(areaID)+"_" + ident.getContent(), ValueType::STRING,
            transInt(intType),ValueType::INT));
    }
    if (is_char) {
        addSymbol(ident, "var_char", intType, areaID);
    }
    else {
        addSymbol(ident, "var_int", intType, areaID);
    }
    if (word.typeEquals("ASSIGN")) {
        getWord(); // =
        initialNum = -1;
        analyseInitVal();
        if (intType > 0) {
            codes.push_back(PCode(CodeType::COMPLEMENT, transInt(areaID) + "_" + ident.getContent(), ValueType::STRING,
                transInt(initialNum), ValueType::INT));
        }
        initialNum = -1;
    }
    else {
        codes.push_back(PCode(CodeType::PLACEHOLDER, transInt(areaID)+"_"+ident.getContent(), ValueType::STRING,
            transInt(intType),ValueType::INT));
    }
    grammar.push_back("<VarDef>");
}

void GrammaticalAnalyser::analyseInitVal() {
    initialNum++;
    Word word = getNextWord();
    if (word.typeEquals("LBRACE")) {
        getWord(); // {
        word = getNextWord();
        if (!word.typeEquals("RBRACK")) {
           
            analyseInitVal();
            Word word1 = getNextWord();
            while (word1.typeEquals("COMMA")) {
                getWord(); // ,
                analyseInitVal();
                word1 = getNextWord();
            }
        }
        getWord(); // }
    }
    else if (word.typeEquals("STRCON")) {
        std::string s = word.getContent();
        int length = s.length();
        for (int i = 1; i < length - 1; i++) {
            Word w("CHARCON", std::string(1, s[i]), word.getLineNum());
            std::vector<Word> exp = { w };
            analyseExp(exp);
            truncation(is_char);
        }
        getWord();
    }
    else {
        auto expp = getExp();
        analyseExp(expp);
        truncation(is_char);
    }
    grammar.push_back("<InitVal>");
}

int GrammaticalAnalyser::analyseExp(std::vector<Word>& exp) {
    int intType = analyseAddExp(exp);
    grammar.push_back("<Exp>");
    return intType;
}

void GrammaticalAnalyser::analyseCond(const std::string& from) {
    auto expp = getExp();
    analyseLOrExp(expp, from);
    grammar.push_back("<Cond>");
}

void GrammaticalAnalyser::analyseFuncRParams(Word ident, std::vector<Word>& exp, std::vector<int>& paras) {
    std::vector<int> rparas;
    std::vector<std::string> symbol = { "COMMA" };
    Exps exps = divideExp(exp, symbol);
    int j = 0;
    for (auto& exp1 : exps.getWords()) {
        int intType = analyseExp(exp1);
        rparas.push_back(intType);
        codes.push_back(PCode(CodeType::RPARA,
            transInt(intType),ValueType::INT));
        if (j < exps.getSymbols().size()) {
            grammar.push_back(exps.getSymbols()[j++].toString());
        }
    }
    if (!paras.empty()) {
        checkParasMatchRParas(ident, paras, rparas);
    }
    grammar.push_back("<FuncRParams>");
}

void GrammaticalAnalyser::checkParasMatchRParas(Word ident, const std::vector<int>& paras, const std::vector<int>& rparas) {
    if (paras.size() != rparas.size()) {
        error("d", ident.getLineNum());
    }
    else {
        for (size_t i = 0; i < paras.size(); i++) {
            if (paras[i] != rparas[i]) {
                error("e", ident.getLineNum());
            }
        }
    }
}

void GrammaticalAnalyser::analyseRelExp(std::vector<Word>& exp) {
    std::vector<std::string> symbol = { "LSS", "LEQ", "GRE", "GEQ" };
    Exps exps = divideExp(exp, symbol);
    int j = 0;
    for (auto& exp1 : exps.getWords()) {
        analyseAddExp(exp1);
        if (j > 0) {
            CodeType type;
            if (exps.getSymbols()[j - 1].typeEquals("LSS")) {
                type = CodeType::CMPLT;
            }
            else if (exps.getSymbols()[j - 1].typeEquals("LEQ")) {
                type = CodeType::CMPLE;
            }
            else if (exps.getSymbols()[j - 1].typeEquals("GRE")) {
                type = CodeType::CMPGT;
            }
            else {
                type = CodeType::CMPGE;
            }
            codes.push_back(PCode(type));
        }
        grammar.push_back("<RelExp>");
        if (j < exps.getSymbols().size()) {
            grammar.push_back(exps.getSymbols()[j++].toString());
        }
    }
}

void GrammaticalAnalyser::analyseEqExp(std::vector<Word>& exp) {
    std::vector<std::string> symbol = { "EQL", "NEQ" };
    Exps exps = divideExp(exp,symbol);
    int j = 0;
    for (auto& exp1 : exps.getWords()) {
        analyseRelExp(exp1);
        if (j > 0) {
            CodeType type;
            if (exps.getSymbols()[j - 1].typeEquals("EQL")) {
                type = CodeType::CMPEQ;
            }
            else {
                type = CodeType::CMPNE;
            }
            codes.push_back(PCode(type));
        }
        grammar.push_back("<EqExp>");
        if (j < exps.getSymbols().size()) {
            grammar.push_back(exps.getSymbols()[j++].toString());
        }
    }
}

void GrammaticalAnalyser::analyseLAndExp(std::vector<Word>& exp, const std::string& from, const std::string& label) {
    std::vector<std::string> symbol = { "AND" };
    Exps exps = divideExp(exp, symbol);
    int j = 0;
    for (size_t i = 0; i < exps.getWords().size(); i++) {
        std::vector<Word> exp1 = exps.getWords()[i];
        analyseEqExp(exp1);
        if (j > 0) {
            codes.push_back(PCode(CodeType::AND));
        }
        if (exps.getWords().size() > 1 && i != exps.getWords().size() - 1) {
            if (from == "IFTK") {
                codes.push_back(PCode(CodeType::JZ, 
                   label,ValueType::STRING));
            }
            else {
                codes.push_back(PCode(CodeType::JZ,
                   label,ValueType::STRING));
            }
        }
        grammar.push_back("<LAndExp>");
        if (j < exps.getSymbols().size()) {
            grammar.push_back(exps.getSymbols()[j++].toString());
        }
    }
}

void GrammaticalAnalyser::analyseLOrExp(std::vector<Word>& exp, const std::string& from) {
    std::vector<std::string> sym = { "OR" };
    Exps exps = divideExp(exp,sym);
    int j = 0;
    for (size_t i = 0; i < exps.getWords().size(); i++) {
        std::vector<Word> exp1 = exps.getWords()[i];
        std::string label = labelGenerator.getLabel("cond_" + std::to_string(i));
        analyseLAndExp(exp1, from, label);
        codes.push_back(PCode(CodeType::LABEL, 
            label,ValueType::STRING));
        if (j > 0) {
            codes.push_back(PCode(CodeType::OR));
        }
        if (exps.getWords().size() > 1 && i != exps.getWords().size() - 1) {
            if (from == "IFTK") {
                codes.push_back(PCode(CodeType::JNZ,
                  ifLabels.back()["if_block"],ValueType::STRING));
            }
            else {
                codes.push_back(PCode(CodeType::JNZ, 
                   forLabels.back()["for_block"],ValueType::STRING));
            }
        }
        grammar.push_back("<LOrExp>");
        if (j < exps.getSymbols().size()) {
            grammar.push_back(exps.getSymbols()[j++].toString());
        }
    }
}

int GrammaticalAnalyser::analyseLVal(std::vector<Word>& exp) {
    int intType = 0;
    Word ident = exp[0]; // Ident
    if (!hasSymbol(ident)) {
        error("c", ident.getLineNum());
    }
    codes.push_back(PCode(CodeType::PUSH, 
        transInt(getSymbol(ident).getAreaID()) + "_" + ident.getContent(), ValueType::STRING));
    grammar.push_back(ident.toString());
    if (exp.size() > 1) {
        std::vector<Word> exp1;
        int flag = 0;
        for (size_t i = 1; i < exp.size(); i++) {
            Word word = exp[i];
            if (word.typeEquals("LBRACK")) {
                if (flag == 0) {
                    intType++;
                }
                flag++;
                if (flag == 1) {
                    grammar.push_back(word.toString());
                    exp1.clear();
                }
                else {
                    exp1.push_back(word);
                }
            }
            else if (word.typeEquals("RBRACK")) {
                flag--;
                if (flag == 0) {
                    analyseExp(exp1);
                    grammar.push_back(word.toString());
                }
                else {
                    exp1.push_back(word);
                }
            }
            else {
                exp1.push_back(word);
            }
        }
        if (flag > 0) {
            analyseExp(exp1);
            error("k", exp[exp.size() - 1].getLineNum()); // not sure
        }
    }
    grammar.push_back("<LVal>");
    if (hasSymbol(ident)) {
        return getSymbol(ident).getIntType() - intType;
    }
    else {
        return 0;
    }
}

void GrammaticalAnalyser::analyseNumber(Word word) {
    codes.push_back(PCode(CodeType::PUSH, 
        transInt(std::stoi(word.getContent())),ValueType::INT));
    grammar.push_back(word.toString());
    grammar.push_back("<Number>");
}

void GrammaticalAnalyser::analyseCharacter(Word word) {
    codes.push_back(PCode(CodeType::PUSH, 
        transInt(static_cast<int>(word.getContent()[0])),ValueType::INT));
    grammar.push_back(word.toString());
    grammar.push_back("<Character>");
}

int GrammaticalAnalyser::analysePrimaryExp(std::vector<Word>& exp) {
    int intType = 0;
    Word word = exp[0];
    if (word.typeEquals("LPARENT")) {
        // remove ( )
        grammar.push_back(exp[0].toString());
        std::vector<Word> subVec(exp.begin() + 1, exp.end() - 1);
        analyseExp(subVec);
        grammar.push_back(exp[exp.size() - 1].toString());
    }
    else if (word.typeEquals("IDENFR")) {
        intType = analyseLVal(exp);
        Word ident = exp[0];
        if (intType == 0) {
            codes.push_back(PCode(CodeType::VALUE, 
                transInt(getSymbol(ident).getAreaID()) + "_" + ident.getContent(), ValueType::STRING,
                transInt(intType),ValueType::INT));
        }
        else {
            codes.push_back(PCode(CodeType::ADDRESS,
                transInt(getSymbol(ident).getAreaID()) + "_" + ident.getContent(),ValueType::STRING,
                transInt(intType),ValueType::INT));
        }
    }
    else if (word.typeEquals("INTCON")) {
        analyseNumber(exp[0]);
    }
    else if (word.typeEquals("CHARCON")) {
        analyseCharacter(exp[0]);
    }
    else {
        error();
    }
    grammar.push_back("<PrimaryExp>");
    return intType;
}

int GrammaticalAnalyser::analyseUnaryExp(std::vector<Word>& exp) {
    int intType = 0;
    Word word = exp[0];
    if (word.typeEquals("PLUS") || word.typeEquals("MINU") || word.typeEquals("NOT")) {
        // UnaryOp UnaryExp
        analyseUnaryOp(exp[0]); // remove UnaryOp
        auto wexp = std::vector<Word>(exp.begin() + 1, exp.end());
        analyseUnaryExp(wexp);
        CodeType type;
        if (word.typeEquals("PLUS")) {
            type = CodeType::POS;
        }
        else if (word.typeEquals("MINU")) {
            type = CodeType::NEG;
        }
        else {
            type = CodeType::NOT;
        }
        codes.push_back(PCode(type));
    }
    else if (exp.size() == 1) {
        // PrimaryExp
        intType = analysePrimaryExp(exp);
    }
    else {
        if (exp[0].typeEquals("IDENFR") && exp[1].typeEquals("LPARENT")) {
            // Ident '(' [FuncRParams] ')'
            Word ident = exp[0];
            std::vector<int> paras = {};
            if (!hasFunction(ident)) {
                error("c", ident.getLineNum());
            }
            else {
                paras = getFunction(ident).getParas();
            }
            if (!exp[exp.size() - 1].typeEquals("RPARENT")) {
                exp.push_back(Word(")", curWord.getLineNum()));
                error("j");
            }
            // Ident ( )
            grammar.push_back(exp[0].toString());
            grammar.push_back(exp[1].toString());
            if (exp.size() > 3) {
                auto wexp = std::vector<Word>(exp.begin() + 2, exp.end() - 1);
                analyseFuncRParams(ident,wexp, paras);
            }
            else {
                if (!paras.empty()) {
                    error("d", ident.getLineNum());
                }
            }
            grammar.push_back(exp[exp.size() - 1].toString());
            codes.push_back(PCode(CodeType::CALL, 
               ident.getContent(),ValueType::STRING));
            if (hasFunction(ident)) {
                if (getFunction(ident).getReturnType() == "void") {
                    intType = -1;
                }
            }
        }
        else {
            // PrimaryExp
            intType = analysePrimaryExp(exp);
        }
    }
    grammar.push_back("<UnaryExp>");
    return intType;
}

void GrammaticalAnalyser::analyseUnaryOp(Word word) {
    grammar.push_back(word.toString());
    grammar.push_back("<UnaryOp>");
}

Exps GrammaticalAnalyser::divideExp(std::vector<Word>& exp, std::vector<std::string>& symbol) {
    std::vector<std::vector<Word>> exps;
    std::vector<Word> exp1;
    std::vector<Word> symbols;
    bool unaryFlag = false;
    int flag1 = 0;
    int flag2 = 0;
    for (int i = 0; i < exp.size(); i++) {
        Word word = exp[i];
        if (word.typeEquals("LPARENT")) {
            flag1++;
        }
        if (word.typeEquals("RPARENT")) {
            flag1--;
        }
        if (word.typeEquals("LBRACK")) {
            flag2++;
        }
        if (word.typeEquals("RBRACK")) {
            flag2--;
        }
        if (std::find(symbol.begin(), symbol.end(), word.getType()) != symbol.end() && flag1 == 0 && flag2 == 0) {
            // UnaryOp
            if (word.typeOfUnary()) {
                if (!unaryFlag) {
                    exp1.push_back(word);
                    continue;
                }
            }
            exps.push_back(exp1);
            symbols.push_back(word);
            exp1.clear();
        }
        else {
            exp1.push_back(word);
        }
        unaryFlag = word.typeEquals("IDENFR") || word.typeEquals("RPARENT") || word.typeEquals("INTCON") ||word.typeEquals("CHARCON") || word.typeEquals("RBRACK");
    }
    exps.push_back(exp1);
    return Exps(exps, symbols);
}

int GrammaticalAnalyser::analyseMulExp(std::vector<Word>& exp) {
    int intType = 0;
    std::vector<std::string> symbol = { "MULT", "DIV", "MOD" };
    Exps exps = divideExp(exp, symbol);
    int j = 0;
    for (std::vector<Word>& exp1 : exps.getWords()) {
        intType = analyseUnaryExp(exp1);
        if (j > 0) {
            if (exps.getSymbols()[j - 1].typeEquals("MULT")) {
                codes.push_back(PCode(CodeType::MUL));
            }
            else if (exps.getSymbols()[j - 1].typeEquals("DIV")) {
                codes.push_back(PCode(CodeType::DIV));
            }
            else {
                codes.push_back(PCode(CodeType::MOD));
            }
        }
        grammar.push_back("<MulExp>");
        if (j < exps.getSymbols().size()) {
            grammar.push_back(exps.getSymbols()[j++].toString());
        }
    }
    return intType;
}

int GrammaticalAnalyser::analyseAddExp(std::vector<Word>& exp) {
    int intType = 0;
    std::vector<std::string> symbol = { "PLUS", "MINU" };
    Exps exps = divideExp(exp, symbol);
    int j = 0;
    for (std::vector<Word>& exp1 : exps.getWords()) {
        intType = analyseMulExp(exp1);
        if (j > 0) {
            if (exps.getSymbols()[j - 1].typeEquals("PLUS")) {
                codes.push_back(PCode(CodeType::ADD));
            }
            else {
                codes.push_back(PCode(CodeType::SUB));
            }
        }
        grammar.push_back("<AddExp>");
        if (j < exps.getSymbols().size()) {
            grammar.push_back(exps.getSymbols()[j++].toString());
        }
    }
    return intType;
}

void GrammaticalAnalyser::analyseConstExp(std::vector<Word>& exp) {
    analyseAddExp(exp);
    grammar.push_back("<ConstExp>");
}

std::vector<Word> GrammaticalAnalyser::getExp() {
    std::vector<Word> exp;
    bool inFunc = false;
    int funcFlag = 0;
    int flag1 = 0;
    int flag2 = 0;
    Word preWord = Word();
    Word word = getNextWord();
    while (true) {
        if (word.typeEquals("SEMICN") || word.typeEquals("ASSIGN") || word.typeEquals("RBRACE") ||
            word.typeSymbolizeValidateStmt()) {
            break;
        }
        if (word.typeEquals("COMMA") && !inFunc) {
            break;
        }
        if (preWord.getType()!="unknown") {
            if ((preWord.typeEquals("INTCON") || preWord.typeEquals("IDENFR")) && (word.typeEquals("INTCON") || word.typeEquals("IDENFR"))) {
                break;
            }
            if ((preWord.typeEquals("RPARENT") || preWord.typeEquals("RBRACK")) && (word.typeEquals("INTCON") || word.typeEquals("IDENFR"))) {
                break;
            }
            if (flag1 == 0 && flag2 == 0) {
                if (preWord.typeEquals("INTCON") && word.typeEquals("LBRACK")) {
                    break;
                }
                if (preWord.typeEquals("INTCON") && word.typeEquals("LBRACE")) {
                    break;
                }
            }
        }
        if (word.typeOfNotInExp()) {
            break;
        }
        if (word.typeEquals("IDENFR")) {
            if (getNext2Word().typeEquals("LPARENT")) {
                inFunc = true;
            }
        }
        if (word.typeEquals("LPARENT")) {
            flag1++;
            if (inFunc) {
                funcFlag++;
            }
        }
        if (word.typeEquals("RPARENT")) {
            flag1--;
            if (inFunc) {
                funcFlag--;
                if (funcFlag == 0) {
                    inFunc = false;
                }
            }
        }
        if (word.typeEquals("LBRACK")) {
            flag2++;
        }
        if (word.typeEquals("RBRACK")) {
            flag2--;
        }
        if (flag1 < 0) {
            break;
        }
        if (flag2 < 0) {
            break;
        }
        getWordWithoutAddToGrammar();
        exp.push_back(curWord);
        preWord = word;
        word = getNextWord();
    }
    return exp;
}


void GrammaticalAnalyser::analyseBType() {
    // Implementation as required
}

void GrammaticalAnalyser::getWord() {
    curWord = words[index];
    grammar.push_back(curWord.toString());
    index++;
}

void GrammaticalAnalyser::getWordWithoutAddToGrammar() {
    curWord = words[index];
    index++;
}

Word GrammaticalAnalyser::getNextWord() {
    return words[index];
}

Word GrammaticalAnalyser::getNext2Word() {
    return words[index + 1];
}

Word GrammaticalAnalyser::getNext3Word() {
    return words[index + 2];
}

void GrammaticalAnalyser::error(std::string type) {
    errors.push_back(Error(curWord.getLineNum(), type));
}

void GrammaticalAnalyser::error(std::string type, int lineNum) {
    errors.push_back(Error(lineNum, type));
}

void GrammaticalAnalyser::error() {
    // ¿ÕÊµÏÖ
}

void GrammaticalAnalyser::printWords(std::ofstream& writer) {
    for (const std::string& str : grammar) {
        writer << str << "\n";
    }
    writer.flush();
}

void GrammaticalAnalyser::printErrors(std::ofstream& writer) {
    std::sort(errors.begin(), errors.end(), [](const Error& e1, const Error& e2) {
        return e1.getN() < e2.getN();
        });
    for (const Error& error : errors) {
        writer << error << "\n";
    }
    writer.flush();
}

void GrammaticalAnalyser::addArea() {
    areaID++;
    area++;
    symbols[area] = Symbols();
}

void GrammaticalAnalyser::removeArea() {
    symbols.erase(area);
    area--;
}

bool GrammaticalAnalyser::isConst(Word word) {
    for (const auto& s : symbols) {
        if (s.second.hasSymbol(word) && s.second.isConst(word)) {
            return true;
        }
    }
    return false;
}

bool GrammaticalAnalyser::hasSymbol(Word word) {
    for (const auto& s : symbols) {
        if (s.second.hasSymbol(word)) {
            return true;
        }
    }
    return false;
}

bool GrammaticalAnalyser::hasSymbolInThisArea(Word word) {
    return symbols[area].hasSymbol(word);
}

bool GrammaticalAnalyser::hasFunction(Word word) {
    return functions.find(word.getContent()) != functions.end();
}

void GrammaticalAnalyser::addSymbol(Word word, std::string type, int intType, int areaID) {
    symbols[area].addSymbol(type, intType, word, areaID);
}

Symbol GrammaticalAnalyser::getSymbol(Word word) {
    Symbol symbol;
    for (const auto& s : symbols) {
        if (s.second.hasSymbol(word)) {
            symbol = s.second.getSymbol(word);
            break;
        }
    }
    return symbol;
}

Function GrammaticalAnalyser::getFunction(Word word) {
    auto it = functions.find(word.getContent());
    return it->second;
}

void GrammaticalAnalyser::printPCode() {
    std::ofstream codeWriter("pcode.txt");
    for (const PCode& code : codes) {
        codeWriter  <<code.toString() << std::endl;
    }
}

std::vector<PCode> GrammaticalAnalyser::getCodes() {
    return codes;
}

void GrammaticalAnalyser::truncation(bool cut) {
    if (cut) {
        codes.push_back(PCode(CodeType::PUSH, transInt(256), ValueType::INT));
        codes.push_back(PCode(CodeType::MOD));
    }
}

void GrammaticalAnalyser::checkBrack() {
    if (getNextWord().typeEquals("RBRACK")) {
        getWord(); // ]
    }
    else {
        error("k");
    }
}

void GrammaticalAnalyser::checkParent() {
    if (getNextWord().typeEquals("RPARENT")) {
        getWord(); // )
    }
    else {
        error("j");
    }
}

void GrammaticalAnalyser::checkSemicn() {
    if (last_for_stmt) {
        return;
    }
    if (getNextWord().typeEquals("SEMICN")) {
        getWord(); // ;
    }
    else {
        error("i");
    }
}

std::string GrammaticalAnalyser::transInt(int n)
{
    return  std::to_string(n);
}
