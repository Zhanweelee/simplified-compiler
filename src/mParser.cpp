#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <list>

using namespace std;

#include "mParser.h"

// 初始化语法规则
mParser::mParser(mGrammar* exprSyntax) {
    this->mainGrammar = exprSyntax;
    this->root = NULL;
    this->debug = false;
    this->syntaxError = "Error: You have an error in your SQL syntax; \
check the manual that corresponds to your MySQL server version for the right syntax to user near";
}

// 开启debug模式
void mParser::setDebug (bool d) {
    this->debug = d;
}

bool mParser::isDebug () {
    return this->debug;
}

// 对token流进行语法分析
bool mParser::process (vector<mToken> tokens) {
    this->root = NULL;
	this->lineCount = 0;
    this->tokenNum = tokens.size();
    this->tokenStream = new mToken[tokenNum+1];
    for (size_t i = 0; i < tokenNum; i ++)
        tokenStream[i] = tokens[i];
    nextToken = tokenStream;
    process ();
    if (errPos != -1) {
        cout <<syntaxError <<" '" <<errMsg <<"' at line " <<lineCount;
		int columnCount = 0;
		mToken* p = tokenStream;
        for (p; p < nextToken; ++ p) {
			if (p->type == EN)
				columnCount = 0;
			else
                columnCount += p->text.size();
		}
		cout << ", at column " <<columnCount <<"." <<endl;
		return false;
    }
	return true;
}
    
// 语法分析函数，需要同名函数的初始化再调用
void mParser::process () {
    errPos = -1;
    stack<mGrammar*> pStack;
    pStack.push(mainGrammar);
    list<mASTNode*> nList;
    this->root = new mASTNode (SSQL_STMT);
    nList.push_back(root);
    while (nextToken < tokenStream + tokenNum) {
        int tokenType = nextToken->type;
		
		if (tokenType == EN) {
			++ lineCount;
			++ nextToken;
			continue;
		}

        mASTNode* currnt = nList.front();
        nList.pop_front();
        mGrammar* top = pStack.top();
        pStack.pop();

        if (top->isTerminal == false) {
            if (top->isRaw == false) {
				bool isFoundGrammar = false, hasEpsilon = false;
                for (size_t i = 0; i < top->subGrammars.size(); i ++) {
                    mGrammar* sub = top->subGrammars.at(i);
                    if (First.find(sub) == First.end())
                        computeFirst(sub);
                    if (find(First[sub].begin(), First[sub].end(), tokenType) != First[sub].end()) {
                        pStack.push(sub);
                        mASTNode* node = new mASTNode (sub->type, sub->isTerminal, sub->token);
                        currnt->childs.insert(currnt->childs.begin(), node);;
                        nList.push_front(node);
                        isFoundGrammar = true;
						break;
                    }

					if (find(First[sub].begin(), First[sub].end(), EPSILON) != First[sub].end()) {
						hasEpsilon = true;
					}
                }

				if (isFoundGrammar == false && hasEpsilon == false) {
					errMsg = nextToken->text;
					errPos = nextToken - tokenStream;
					break;
				}
            } else {
                if (First.find(top) == First.end())
                    computeFirst(top);
                if (find(First[top].begin(), First[top].end(), tokenType) != First[top].end()) {
                    for (int i = top->subGrammars.size()-1; i >= 0; i --) {
                        mGrammar* sub = top->subGrammars.at(i);
                        pStack.push(sub);
                        mASTNode* node = new mASTNode (sub->type, sub->isTerminal, sub->token);
                        currnt->childs.insert(currnt->childs.begin(), node);
                        nList.push_front(node);
                    }
                } else {
                    errMsg = nextToken->text;
                    errPos = nextToken - tokenStream;
                    break;
                }
            }
        } else {
            if (top->token->type == tokenType) {
                if (this->debug) {
                    cout <<"Token: " <<tokenType <<endl <<"Position: " <<(int)(nextToken - tokenStream) <<endl <<endl;
                }
                currnt->token = nextToken;
                currnt->isToken = true;
                nextToken ++;
            } else {
                errMsg = nextToken->text;
                errPos = nextToken - tokenStream;
                break;
            }
        }
    }

    if (!pStack.empty()) {
        errPos = nextToken - tokenStream;
    } else if (this->debug) {
        cout <<"Success at generating Parser tree!" <<endl;
    }
}

//// 初始化Sentence文法所有推导式的First集和Follw集
//void mParser::computeParserTable (mGrammar* expr) {
//    for (size_t i = 0; i < mTOKEN_NUM; i ++) {
//
//    }
//}


// 计算文法的First集
void mParser::computeFirst (mGrammar* expr) {
    if (expr->isTerminal) {
        First[expr].push_back (expr->token->type);
        return;
    }

        for (size_t j = 0; j < expr->subGrammars.size(); j ++) {
		    mGrammar* sub = expr->subGrammars.at(j);
			if (expr->isRaw == false && expr->type != sub->type) {
                computeFirst (sub);
			    for (size_t k = 0; k < First[sub].size(); k ++)
                    if (find(First[expr].begin(), First[expr].end(), First[sub].at(k)) == First[expr].end())
                        First[expr].push_back(First[sub].at(k));
                continue;
            }

		    if (sub->isTerminal) {
                int tokenType = sub->token->type;
                if (find(First[expr].begin(), First[expr].end(), tokenType) == First[expr].end())
                    First[expr].push_back(tokenType);
            } else {
			    computeFirst (sub);
			    for (size_t k = 0; k < First[sub].size(); k ++)
                    if (find(First[expr].begin(), First[expr].end(), First[sub].at(k)) == First[expr].end())
                        First[expr].push_back(First[sub].at(k));
		    }

            // 例如X=Y1Y2Y3……Yk，假设Yi的First中包含epsilon，则把Yi+1Yi+2……Yk的First放入X的First中
            bool hasEpsilon = false;
            for (size_t k = 0; k < First[sub].size(); k ++) {
                if (First[sub].at(k) == EPSILON) {
                    hasEpsilon = true;
                    break;
                }
            }

            // 如果第一个推导式的First中没有epsilon，则先不继续计算其他。
            if (hasEpsilon == false)
                break;
        }
}

//// 计算文法的Follow集
//void mParser::computeFollow (mGrammar* expr) {
//    if (expr->isRaw == false) {
//        for (size_t i = 0; i < expr->subGrammars.size(); i ++) {
//            mGrammar* sub = expr->subGrammars.at(i);
//            computeFollow (sub);
//		    for (size_t k = 0; k < Follow[sub].size(); k ++) {
//                int tokenType = Follow[sub].at(k);
//                if (find(Follow[expr].begin(), Follow[expr].end(), tokenType) == Follow[expr].end())
//                    Follow[expr].push_back(tokenType);
//            }
//        }
//    } else {
//        // 假设推导式为 A->aBb，a为一串终结符，找出B的位置
//        int j;
//		mGrammar* sub = NULL;
//        for (j = 0; j < expr->subGrammars.size(); j ++) {
//            sub = expr->subGrammars.at(j);
//            if (sub->isTerminal == false) {
//                break;
//            }
//        }
//
//        // First(b)包含于Follow(B)
//        bool hasEpsilon = false;
//        if (j + 1 < expr->subGrammars.size()) {
//            computeFirst(expr->subGrammars.at(j+1));
//		    mGrammar* sub = expr->subGrammars.at(j);
//		    mGrammar* subNext = expr->subGrammars.at(j+1);
//            for (size_t m = 0; m < First[subNext].size(); m ++) {
//                int tokenType = First[subNext].at(m);
//                hasEpsilon = (tokenType == EPSILON ? true : false);
//                if (find(Follow[sub].begin(), Follow[sub].end(), tokenType) == Follow[sub].end() && tokenType != EPSILON)
//                    Follow[sub].push_back(tokenType);
//            }
//
//            // 如果A->aBC, A的Follow应该包含于C
//            int lastPos = expr->subGrammars.size()-1;
//            mGrammar* last = expr->subGrammars.at(lastPos);
//            if (last->isTerminal == false) {
//                for (size_t m = 0; m < Follow[expr].size(); m ++) {
//                    int tokenType = Follow[expr].at(m);
//                    if (find(Follow[last].begin(), Follow[last].end(), tokenType) == Follow[last].end() && tokenType != EPSILON)
//                        Follow[last].push_back(tokenType);
//                }
//            }
//        }
//
//        // 如果A->aB或者A->aBb, 且b包含epsilon
//        if (j + 1 == expr->subGrammars.size() || hasEpsilon == true) {
//		    mGrammar* sub = expr->subGrammars.at(j);
//            for (size_t m = 0; m < Follow[expr].size(); m ++) {
//                int tokenType = Follow[expr].at(m);
//                if (find(Follow[sub].begin(), Follow[sub].end(), tokenType) == Follow[sub].end() && tokenType != EPSILON)
//                    Follow[sub].push_back(tokenType);
//            }
//        }
//    }
//}

// 获取AST根节点
mASTNode* mParser::getASTree() {
    return this->root;
}


void mParser::printTree (mASTNode* root, string prefix) {
    for (size_t i = 0; i < root->childs.size(); i ++) {
        mASTNode* node = root->childs[i];
        int size = root->childs.size();
         if (i + 1 >= size) {
            if (node->isToken == false) {
                cout <<prefix <<"└" <<node->type <<endl;
                printTree(node, prefix + "    ");
            } else {
                cout <<prefix <<"└" <<"Token ('" <<node->token->text <<"')" <<endl;
            }
        } else if (i == 0) {
            if (node->isToken == false) {
                cout <<prefix <<"┌" <<node->type <<endl;
                printTree(node, prefix + "│  ");
            } else {
                cout <<prefix <<"┌" <<"Token ('" <<node->token->text <<"')" <<endl;
            }
        } else {
            if (node->isToken == false) {
                cout <<prefix <<"├" <<node->type <<endl;
                printTree(node, prefix + "│  ");
            } else {
                cout <<prefix <<"├" <<"Token ('" <<node->token->text <<"')" <<endl;
            }
        }
            
    }

}