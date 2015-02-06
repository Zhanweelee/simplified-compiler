all: run

run: compile
	./ssql

compile: mToken.h mState.h mLexer.h mGrammar.h mASTNode.h mParser.h mSQL.h mLexer.cpp mParser.cpp mSQL.cpp mLexerInit.cpp mParserInit.cpp main.cpp
	g++ *.h *.cpp -o ssql

