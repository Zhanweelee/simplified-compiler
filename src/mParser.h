#include <vector>

using std::vector;

#include "mGrammar.h"
#include "mASTNode.h"

#ifndef M_PARSER
#define M_PARSER

class mParser {
private:
    // token流、下一个将匹配的token位置、token数目
    mToken* tokenStream;
    mToken* nextToken;
    int tokenNum;

    // 错误提示信息
    string syntaxError;
    string errMsg;
    int errPos;
	int lineCount;

    // 记录Abstrate Syntax Tree的根节点
    mASTNode* root;

    map<mGrammar*, vector<int> > First;

    // 语法规则
    mGrammar* mainGrammar;

    // 记录当前匹配到的文法
    mGrammar* next;

    // 语法分析函数，需要同名函数的初始化再调用
    void process();

    // 是否开启debug模式
    bool debug;

public:
	// 计算文法的First集
	void computeFirst (mGrammar* expr);
	// 开启debug模式
	void setDebug (bool);
    // 返回debug状态
    bool isDebug ();
public:
    // 初始化语法规则
    mParser(mGrammar* grammar);
    // 对token流进行语法分析
    bool process (vector<mToken> token_stream);
    // 获取AST根节点
    mASTNode* getASTree();
    void printTree (mASTNode* root, string prefix);
    void printError();
};

#endif