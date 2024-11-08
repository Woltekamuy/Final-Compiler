#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "Word.h"  
#include "CodeGeneration/Pcode.h"  
#include "CodeGeneration/LabelGenerator.h"  
#include "Function.h"  
#include"Symbols.h"
#include"Error.h"
#include "Exps.h"

class GrammaticalAnalyser {
private:
    std::vector<Word> words;
    int index = 0;
    Word curWord;
    std::vector<std::string> grammar;

    std::unordered_map<int, Symbols> symbols;  // Assuming Symbols class exists
    std::unordered_map<std::string, Function> functions;
    std::vector<Error> errors;  // Assuming Error class exists

    int area = -1;
    bool needReturn = false;
    int whileFlag = 0;
    bool is_char = false;
    bool last_for_stmt = false;
    int initialNum = -1;

    std::vector<PCode> codes;
    LabelGenerator labelGenerator;

    std::vector<std::unordered_map<std::string, std::string>> ifLabels;
    std::vector<std::unordered_map<std::string, std::string>> whileLabels;
    std::vector<std::unordered_map<std::string, std::string>> forLabels;
    std::vector<std::unordered_map<int, std::string>> condLabels;
    int areaID = -1;

    void getWord();
    void getWordWithoutAddToGrammar();
    Word getNextWord();
    Word getNext2Word();
    Word getNext3Word();
    void error(std::string type);
    void error(std::string type, int lineNum);
    void error();
    void printWords(std::ofstream& writer);
    void printErrors(std::ofstream& writer);
    void addArea();
    void removeArea();
    bool isConst(Word word);
    bool hasSymbol(Word word);
    bool hasSymbolInThisArea(Word word);
    bool hasFunction(Word word);
    void addSymbol(Word word, std::string type, int intType, int areaID);
    Symbol getSymbol(Word word);
    Function getFunction(Word word);
    void truncation(bool cut);
    void checkBrack();
    void checkParent();
    void checkSemicn();
    std::string transInt(int n);


    void analyseCompUnit();
    void analyseDecl();
    void analyseConstDef();
    void analyseFuncDef();
    void analyseMainFuncDef();
    bool analyseBlock(bool fromFunc);
    bool analyseBlockItem();
    bool analyseStmt();
    std::vector<int> analyseFuncFParams();
    int analyseFuncFParam();
    std::string analyseFuncType();
    void analyseConstInitVal();
    void analyseBType();
    void analyseConstDecl();
    void analyseVarDecl();
    void analyseVarDef();
    void analyseInitVal();
    int analyseExp(std::vector<Word>& exp);
    void analyseCond(const std::string& from);
    void analyseFuncRParams(Word ident, std::vector<Word>& exp, std::vector<int>& paras);
    void checkParasMatchRParas(Word ident, const std::vector<int>& paras, const std::vector<int>& rparas);
    void analyseRelExp(std::vector<Word>& exp);
    void analyseEqExp(std::vector<Word>& exp);
    void analyseLAndExp(std::vector<Word>& exp, const std::string& from, const std::string& label);
    void analyseLOrExp(std::vector<Word>& exp, const std::string& from);
    int analyseLVal(std::vector<Word>& exp);
    void analyseNumber(Word word);
    void analyseCharacter(Word word);
    int analysePrimaryExp(std::vector<Word>& exp);
    int analyseUnaryExp(std::vector<Word>& exp);
    void analyseUnaryOp(Word word);
    Exps divideExp(std::vector<Word>& exp, std::vector<std::string>& symbol);
    int analyseMulExp(std::vector<Word>& exp);
    int analyseAddExp(std::vector<Word>& exp);
    void analyseConstExp(std::vector<Word>& exp);
    std::vector<Word> getExp();


public:
    GrammaticalAnalyser(const std::vector<Word>& words);
    void printPCode();
    std::vector<PCode> getCodes();
};