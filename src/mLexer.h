#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

#include "mToken.h"
#include "mState.h"

#ifndef M_LEXER
#define M_LEXER

// 词法分析器
class mLexer {
private:
    vector<mToken> tokenStream;
    string errorMsg;
    string input;
    mState *readyState;
	vector<mState*> currntStates;
    map<int, int> stateIDToTokenType;
    int position;
    mToken getNext ();
    void process ();
public:
    mLexer(mState* ready, map<int, int> stateIDToTokenType);
    void process (const string& input);
    vector<mToken> getTokenStream ();
};

#endif