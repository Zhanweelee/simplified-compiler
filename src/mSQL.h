#include <string>
#include <vector>
#include <map>

using namespace std;

#include "mASTNode.h"

#ifndef M_SQL
#define M_SQL

enum fieldType {F_INT, F_CHAR, F_STR};
struct field {
    fieldType type;
    int val;
    char ch;
    string str;
    field (int val = 0, fieldType type = F_INT): type(type), val(val), ch(val) {};
    field (const string& str, fieldType type): type(type), str(str) {};
};

struct Operand {
    bool isColumn;
    string name;
    int value;
    Operand (const string& name): name(name) { isColumn = true; };
    Operand (int val = 0): value(val) { isColumn = false; };
};

struct condition {
    Operand operand[2];
    int operation;
    condition (Operand oper1, int operation, Operand oper2): operation(operation) { operand[0] = oper1; operand[1] = oper2;};
};

struct table {
    vector<string> primary_key;
    map<string, field> default_value;
    map<string, fieldType> columns;
    map<string, vector<field> > data;
    table (map<string, fieldType> cols, map<string, field> defval, vector<string> pri):columns(cols), default_value(defval), primary_key(pri) {};
};

class mSQL {
public:
    mSQL();
    void process(mASTNode* tree);
    void setDebug (bool);
    bool isDebug ();

private:
    map<string, table*> database;
    string error_msg;
    bool debug;

    // SQL接口
    bool create (const string& name, map<string, fieldType> columns, map<string, field> default_value, vector<string> primary_key);
    bool insert (const string& name, vector<string> columns, map<string, field>  data);

    // 语义分析部分函数（部分包含SQL函数，这里不做明确区分）
    void create (mASTNode* stmt);
    void insert (mASTNode* stmt);
    void _delete (mASTNode* stmt);
    void query (mASTNode* stmt);

    // 对具体不同的节点进行语义分析
    vector<string> getColumnList (mASTNode* column_list);
    vector<field> getValueList (mASTNode* value_list);
    bool getWhereClause (mASTNode* where_clause, map<string, field>& data);
    bool getValueOfDisjunct (mASTNode* disjunct, map<string, field>& data);
    bool getValueOfConjunct (mASTNode* conjunct, map<string, field>& data);
    bool getValueOfBoolExpr (mASTNode* bool_expr, map<string, field>& data);
    bool getValueOfComp (mASTNode* bool_expr, map<string, field>& data);
    int getValueOfCompExpr (mASTNode* bool_expr, map<string, field>& data);

    int getValueOfSimpleExpr (mASTNode* simple_expr);
    int getValueOfSimpleUnary (mASTNode* simple_unary);
    int getValueOfFomula (int val1, int val2, char operation);
    int getValueOfSimpleTerm (mASTNode* simple_term);
    int getValueOfTerm (mASTNode* term, map<string, field>& data);
    int getValueOfUnary (mASTNode* unary, map<string, field>& data);
};

#endif