all: run

run: compile
	./ssql

compile: src/mToken.h src/mState.h src/mLexer.h src/mGrammar.h src/mASTNode.h src/mParser.h src/mSQL.h src/mLexer.cpp src/mParser.cpp src/mSQL.cpp src/mLexerInit.cpp src/mParserInit.cpp src/main.cpp
	g++ src/*.h src/*.cpp -o ssql

