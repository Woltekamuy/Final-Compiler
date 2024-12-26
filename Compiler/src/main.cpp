#include"../include/CodeGeneration/PCodeExecutor.h"
#include"../include/LexicalAnalyser.h"
#include"../include/GrammaticalAnalyser.h"
#include"../include/FileProcessor.h"

int main() {
	FileProcessor fileProcessor;
	LexicalAnalyser lexicalAnalyser;
	lexicalAnalyser.getWords();	
	//lexicalAnalyser.printWords();
	
	GrammaticalAnalyser grammaticalAnalyser(lexicalAnalyser.getWords());
	grammaticalAnalyser.printPCode();
	grammaticalAnalyser.printErrors();
	
	if(!Errors::hasError())
	{
		PCodeExecutor pCodeExecutor(grammaticalAnalyser.getCodes(), fileProcessor.getWriter());
		pCodeExecutor.run();
		pCodeExecutor.print();
	}
	
	
	return 0;
}