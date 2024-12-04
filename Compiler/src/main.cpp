#include"../include/CodeGeneration/PCodeExecutor.h"
#include"../include/LexicalAnalyser.h"
#include"../include/GrammaticalAnalyser.h"
#include"../include/FileProcessor.h"

int main() {
	FileProcessor fileProcessor;
	LexicalAnalyser lexicalAnalyser;
	GrammaticalAnalyser grammaticalAnalyser(lexicalAnalyser.getWords());
	grammaticalAnalyser.printPCode();
	PCodeExecutor pCodeExecutor(grammaticalAnalyser.getCodes(),fileProcessor.getWriter());
	pCodeExecutor.run();
	pCodeExecutor.print();
	return 0;
}