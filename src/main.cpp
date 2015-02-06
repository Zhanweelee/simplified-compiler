#include <iostream>
#include <fstream>
using namespace std;

#include "mLexer.h"
#include "mParser.h"
#include "mSQL.h"

extern mLexer* mLexerInit();
extern mParser* mParserInit();
int main(int argc, char* argv[]) {
    // ��ʼ��Lexer
    mLexer* myLexer = mLexerInit();
    // ��ʼ��Parser
    mParser* myParser = mParserInit();
    // ��ʼ��SSQL
    mSQL* mySQL = new mSQL();

	mASTNode* mytree = NULL;
	
    // ��������ļ�
    if (argc >= 2) {
        fstream input(argv[1], fstream::in);
        if (input.fail()) {
            cout <<"Cannot open file!" <<endl;
            return 0;
        }
        string query;
        while (!input.eof()) {
            char line[256];
            // ��ȡһ������
            input.getline(line, 256);
            string str (line);
            // ������ҵ��ֺ���Ϊ����
            int pos = str.find(';');
            if (pos == -1) {
                query += str + "\n";
            } else {
                query += str.substr (0, pos+1);
                // �ʷ�����
                myLexer->process (query);
                bool parseResult = myParser->process(myLexer->getTokenStream());    // �����﷨��
                if (parseResult) {
                    // ����﷨�����ɹ�
					mytree = myParser->getASTree();
					if (mySQL->isDebug()) {
						myParser->printTree (mytree, "");    // �������debugģʽ�����ӡ�﷨��
					}
					mySQL->process (mytree);	
                }
                // ��ȡ��һ����������ִ��
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
                    bool parseResult = myParser->process(myLexer->getTokenStream());    // �����﷨��

                    if (parseResult) {
					    mytree = myParser->getASTree();

					    if (mySQL->isDebug()) {
						    myParser->printTree (mytree, "");    // �������debugģʽ�����ӡ�﷨��
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