#include <stdlib.h>
#include <stdio.h>
#include "mLexer.h"

int getCharType (char ch);
void parseTextByType (mToken* token);
// extern enum mSTATE_TYPE;

// 构造函数: 初始化状态转移图和token类型映射字典
mLexer::mLexer(mState* ready, map<int, int> stateIDToTokenType) {
    this->position = 0;
    this->readyState = ready;
	this->currntStates.push_back(ready);
	this->stateIDToTokenType = stateIDToTokenType;
}

// 对input文本进行词法分析
void mLexer::process (const string& input) {
    this->tokenStream.clear();
    this->currntStates.clear();
    this->errorMsg.clear();
    this->currntStates.push_back(readyState);
    this->position = 0;
    this->input = input;
    process();
}

// 进行词法分析
void mLexer::process () {
    while (position < (int)input.size()) {
        mToken next = getNext();

        // 错误处理
        if (errorMsg != "") {
            printf("%s\n", errorMsg.c_str());
            return;
        }

        // 根据类型赋上token的值
		parseTextByType (&next);
        tokenStream.push_back(next);
    }
}

// 返回token流
vector<mToken> mLexer::getTokenStream () {
    return this->tokenStream;
}

// 产生文本流中下一个token
mToken mLexer::getNext() {
    vector<mToken> availableTokens;
    while (!currntStates.empty()) {
        if (position >= (int)input.size()) {
            int lastStateID = currntStates[0]->stateID;
            if (stateIDToTokenType.find(lastStateID) != stateIDToTokenType.end()) {
                return mToken (stateIDToTokenType[currntStates[0]->stateID], currntStates[0]->currntText);
            } else {
                errorMsg = "Error: at " + currntStates[0]->currntText + " can't transfer!\n";
                break;
            }
		}
        char nextChar = input[position++];
        int charType = getCharType(nextChar);
		vector<mState*> matchedStates;
		for (size_t count = 0; count < currntStates.size(); ++count) {
			multimap<CONDITION, mState*> generalAdjacents = currntStates[count]->generalTransfer;
			multimap<char, mState*> specifiedAdjacents = currntStates[count]->specifiedTransfer;

            vector<mState*> tempStates;

			for (multimap<CONDITION, mState*>::iterator itr = generalAdjacents.begin(); itr != generalAdjacents.end(); itr ++)
				if (itr->first == charType)
                    tempStates.push_back(itr->second);
			
			for (multimap<char, mState*>::iterator itr = specifiedAdjacents.begin(); itr != specifiedAdjacents.end(); itr ++)
				if (itr->first == nextChar)
                    tempStates.push_back(itr->second);

            for (size_t i = 0; i < tempStates.size(); i ++)
                if (tempStates.at(i)->stateID == FINISH) {
						mState* save = currntStates[count];
                        availableTokens.push_back(mToken (stateIDToTokenType[save->stateID], save->currntText));
					} else if (tempStates.at(i)->stateID != END) {
						mState* nextState = tempStates.at(i);
						nextState->currntText = currntStates[count]->currntText;
						nextState->currntText.push_back(nextChar);
						matchedStates.push_back(nextState);
					}
        }
        currntStates = matchedStates;
    }
    currntStates.push_back(readyState);
    if (availableTokens.empty()) {
        return getNext();
    } else {
        position --;
        return availableTokens.back();
    }
}

/* 其他处理函数 */
bool isLetter (char ch);
bool isDigit (char ch);
bool isOperator (char ch);
bool isWhitespace (char ch);
bool isDelimiter (char ch);

// 根据字符来判断符合的条件
// 如'a'符合IS_LETTER, '1'符合IS_DIGIT, 用于通用判断的转移规则
int getCharType (char ch) {
    if (isLetter(ch)) {
        return IS_LETTER;
    } else if (isDigit(ch)) {
        return IS_DIGIT;
    } else if (isOperator(ch)) {
        return IS_OPERATOR;
    } else if (isWhitespace(ch)) {
        return IS_WHITESPACE;
    } else if (isDelimiter(ch)) {
        return IS_DELIMITER;
    }
    return UNKNOWN;
}

// 根据token的类型，对文本进行解析，得到Value
void parseTextByType (mToken* token) {
	if (token->type == INT) {
		char buffer[256];
		for (size_t i = 0; i < token->text.size(); i ++)
			buffer[i] = token->text[i];
		token->value = atoi(buffer);
	}
}

bool isLetter (char ch) {
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        return true;
    return false;
}

bool isDigit (char ch) {
    if (ch >= '0' && ch <= '9')
        return true;
    return false;
}

bool isOperator (char ch) {
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=')
        return true;
    return false;
}

bool isWhitespace (char ch) {
    if (ch == ' ' || ch == '\t' || ch == '\n')
        return true;
    return false;
}

bool isDelimiter (char ch) {
    if (ch == ',' || ch == ';' || ch == '(' || ch == ')' || ch == '\n')
        return true;
    return false;
}
