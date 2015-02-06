#include <iostream>
#include <fstream>
using namespace std;

#include "mLexer.h"
#include "mParser.h"
#include "mSQL.h"

extern mLexer* mLexerInit();
extern mParser* mParserInit();
int main(int argc, char* argv[]) {
    // 初始化Lexer
    mLexer* myLexer = mLexerInit();
    // 初始化Parser
    mParser* myParser = mParserInit();
    // 初始化SSQL
    mSQL* mySQL = new mSQL();

	mASTNode* mytree = NULL;
	
    // 如果输入文件
    if (argc >= 2) {
        fstream input(argv[1], fstream::in);
        if (input.fail()) {
            cout <<"Cannot open file!" <<endl;
            return 0;
        }
        string query;
        while (!input.eof()) {
            char line[256];
            // 获取一行命令
            input.getline(line, 256);
            string str (line);
            // 如果查找到分号作为结束
            int pos = str.find(';');
            if (pos == -1) {
                query += str + "\n";
            } else {
                query += str.substr (0, pos+1);
                // 词法分析
                myLexer->process (query);
                bool parseResult = myParser->process(myLexer->getTokenStream());    // 构建语法树
                if (parseResult) {
                    // 如果语法分析成功
					mytree = myParser->getASTree();
					if (mySQL->isDebug()) {
						myParser->printTree (mytree, "");    // 如果开启debug模式，则打印语法树
					}
					mySQL->process (mytree);	
                }
                // 截取下一部分语句继续执行
				query = str.substr (pos+1);
            }
        }
    } else {

        string str, query;
        cout <<"myssql> ";
        while (getline(cin, str) && str != "exit" && str != "exit;") {
            int pos = str.find(';');
            if (pos == -1) {
                query += str + "\n";
			    cout <<"      > ";
            } else {
                query += str.substr (0, pos+1);

                if (query == "debug" || query == "debug;") {
                    myParser->setDebug (true);
                    mySQL->setDebug (true);
				    query.clear();
                } else {
                    myLexer->process(query);
                    bool parseResult = myParser->process(myLexer->getTokenStream());    // 构建语法树

                    if (parseResult) {
					    mytree = myParser->getASTree();

					    if (mySQL->isDebug()) {
						    myParser->printTree (mytree, "");    // 如果开启debug模式，则打印语法树
					    }

					    mySQL->process (mytree);	
                    }
				    query = str.substr (pos+1);
			    }
			
			    cout <<"myssql> ";
            }
        };
    }
    return 0;
}