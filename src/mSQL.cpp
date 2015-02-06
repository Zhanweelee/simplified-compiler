#include <iostream>
#include <algorithm>
#include <stack>
#include <math.h>
#include <sstream>
#include <iomanip>
using namespace std;

#include "mASTNode.h"
#include "mSQL.h"

void toLowerCase (string& str);
string toString (int );

mSQL::mSQL() {
    this->debug = false;
}

void mSQL::setDebug (bool d) {
    this->debug = d;
}

bool mSQL::isDebug () {
    return this->debug;
}

void mSQL::process(mASTNode* tree) {
    mASTNode* stmt = NULL;
    if (tree->type == SSQL_STMT && !tree->childs.empty()) {
        stmt = tree->childs.at(0);
    } else {
        error_msg = "There is no branchs under ssql_stmt.";
        return;
    }

    switch (stmt->type) {
    case CREATE_STMT:
        create (stmt);
        break;
    case INSERT_STMT:
        insert (stmt);
        break;
    case DELETE_STMT:
        _delete (stmt);
        break;
    case QUERY_STMT:
        query (stmt);
        break;

    }
    
    if (error_msg != "") {
        cout <<"ERROR: " <<error_msg <<endl;
		error_msg = "";
    }
}

void mSQL::create (mASTNode* stmt) {
	// 创建数据表中column的列表
    mASTNode* decl_list = NULL;

	// 表名
    string table_name = "";
    for (size_t i = 0; i < stmt->childs.size(); i ++) {
        if (stmt->childs.at(i)->type == G_TABLE) {      // 获取到CREATE TABLE后的的文本
            mToken* nextAfterTable = stmt->childs.at(i+1)->token;
			if (nextAfterTable->type != ID) {          // 禁止使用除了ID之外的token作为表名
				this->error_msg = "Cannot use unvalid name of table.";
				return;
			}
			table_name = stmt->childs.at(i+1)->token->text;
        } else if (stmt->childs.at(i)->type == DECL_LIST) {  // 获取数据表中column的列表声明
            decl_list = stmt->childs.at(i);
            break;
        }
    }
    
    toLowerCase (table_name);
    map<string, field> default_value;   // 列的初始值
    map<string, fieldType> columns;     // 列名
    vector<string> primary_keys;        // 主键

    while (!decl_list->childs.empty()) {
        mASTNode* decl = NULL;
        for (int i = 0; i < decl_list->childs.size(); i ++) {
            if (decl_list->childs.at(i)->type == DECL) {                // column的文法
                decl = decl_list->childs.at(i);
            } else if (decl_list->childs.at(i)->type == DECL_LIST_) {   // 包含下一个column的文法
                decl_list = decl_list->childs.at(i);
                if (decl == NULL) {
                    i = -1;
                } else {
                    break;
                }
            }
        }
        
        string column_name;           // 列名
        fieldType type;               // 列的类型
        int defval = 0;
        bool toSetPrimaryKey = false; // 当前是否设置primary key
        bool toSetDefault = false;
        for (int i = 0; i < decl->childs.size(); i ++) {
            switch (decl->childs.at(i)->type) {
            case G_ID:
                column_name = decl->childs.at(i)->token->text;  // 列名
                break;
            case G_INT:                                         // 列的类型
                type = F_INT;
                break;
            case DEFAULT_SPEC:                                  // 设定列的初始值
                {
                    mASTNode* simple_expr = NULL;
                    mASTNode* default_spec = decl->childs.at(i);
					if (default_spec->childs.empty()) break;    // 若default_spec节点为空，则不设置初始值
                    for (int i = 0; i < default_spec->childs.size(); i ++) {
                        if (default_spec->childs.at(i)->type == SIMPLE_EXPR) {
                            simple_expr = default_spec->childs.at(i);
                        } else if (default_spec->childs.at(i)->type == DEFAULT_SPEC_) {
                            default_spec = default_spec->childs.at(i);
                            i = -1;
                        }
                    }
                    if (simple_expr == NULL) {                  // 如果没有获取到表达式，则提示错误
                        error_msg = "Your default value's expression has error! Please check again.";
                        return;
                    }

                    defval = getValueOfSimpleExpr (simple_expr); // 处理表达式获得结果
                    if (error_msg != "") return;
                    toSetDefault = true;
                }
                break;
            case G_PRIMARY:                                      // 标记设置primary key
                toSetPrimaryKey = true;
                break;
            case COLUMN_LIST:                                    // 主键的列表
                if (toSetPrimaryKey) {
                    mASTNode* column_list = decl->childs.at(i);
                    primary_keys = getColumnList (column_list);  // 获得主键
                }
                break;
            case DECL_:
                decl = decl->childs.at(i);
                i = -1;
                break;
            }
        }
        if (toSetPrimaryKey == false) {
            if (columns.find (column_name) != columns.end()) {
                this->error_msg = "Duplicate column name '" + column_name + "'"; // mark
                return;
            }
            columns[column_name] = type;
            if (toSetDefault) {
                default_value[column_name] = field(defval);
                toSetDefault = false;
            }
        }
    }

    // 检查主键中的column是否都存在
    bool isPrimaryKeysValid = true;
    for (size_t i = 0; i <primary_keys.size(); i ++) {
        if (columns.find (primary_keys.at(i)) == columns.end()) {
            this->error_msg = "Key column '" + primary_keys.at(i) + "' doesn't exist in table";
            isPrimaryKeysValid = false;
        }
    }

    // 如果主键中column非法，则提示错误
    if (isPrimaryKeysValid) {
        this->create (table_name, columns, default_value, primary_keys);
    } else {
        return;
    }

}

bool mSQL::create (const string& name, map<string, fieldType> columns, map<string, field> default_value, vector<string> primary_key) {
    // 判断是否已经存在该数据表
	if (database.find(name) != database.end()) {
		this->error_msg = "Table '" + name + "' exists.";
        return false;
    }

	// 把该表新建到数据库中
    table* new_table = new table(columns, default_value, primary_key);
    database[name] = new_table;
    return true;
}

void mSQL::insert (mASTNode* stmt) {
	// 插入数据表中的column列表
    mASTNode* column_list = NULL;
	// column列表对应的数据
    mASTNode* value_list = NULL;
    string table_name = "";

    for (size_t i = 0; i < stmt->childs.size(); i ++) {
        if (stmt->childs.at(i)->type == G_INTO) {
            table_name = stmt->childs.at(i+1)->token->text;      // 数据表名
        } else if (stmt->childs.at(i)->type == COLUMN_LIST) {
            column_list = stmt->childs.at(i);                    // column列表
        } else if (stmt->childs.at(i)->type == VALUE_LIST) {
            value_list = stmt->childs.at(i);                     // column列表对应的数据
        }
    }
    
    vector<string> columns;
    map<string, field> data;
    columns = getColumnList (column_list);                       // 获取要插入数据的column列表
    vector<field> value = getValueList (value_list);
    if (columns.size() != value.size()) {
        error_msg = "Number of value and columns doesn't match!";
        return;
    }

    for (size_t i = 0; i < columns.size(); i ++) {
        data[columns.at(i)] = value.at(i);
    }

    toLowerCase (table_name);
    insert (table_name, columns, data);
}

bool mSQL::insert(const string& name, vector<string> columns, map<string, field> data) {
    if (database.find(name) == database.end()) {
        error_msg = "Table doesn't exist.";
        return false;
    }

	// 检验column是否具有重复项
    for (size_t i = 0; i < columns.size(); i ++) {
        for (size_t j = i+1; j < columns.size(); j ++) {
            if (columns.at(i) == columns.at(j)) {
                error_msg = "Column '" + columns.at(i) + "' specified twice";
                return false;
            }
        }
    }

    table* toInsertTable = database.find(name)->second;

	// 检验需要插入数据的column是否合法
    map<string, fieldType> existColumns = toInsertTable->columns;             // 数据表的column
    for (size_t i = 0; i < columns.size(); i ++) {
        string column_name = columns.at(i);
        if (existColumns.find(column_name) == existColumns.end()) {           // 如果即将插入的column不存在数据表中，则非法column
            error_msg = "Unknown column '" + column_name + "' in 'field list'";
            return false;
        }
    }
	
    map<string, field> validDefault = toInsertTable->default_value;           // column的初始值
    for (map<string, fieldType>::iterator itr = existColumns.begin(); itr != existColumns.end(); itr ++) {
        string column_name = itr->first;
        if (find(columns.begin(), columns.end(), column_name) == columns.end()) {  // 如果插入的column没有设定值
            if (validDefault.find(column_name) == validDefault.end()) {            // 同时该列没有初始值则报错
                error_msg = "No default value for " + column_name + " column!";
                return false;
            } else {
                data[column_name] = validDefault[column_name];                     // 该列有初始值，则设置为初始值
            }
        }
    }
    
	// 检验主键的唯一性
	vector<string> primaryKeys = toInsertTable->primary_key; 
	for (size_t i = 0; i < primaryKeys.size(); i ++) {
		string key = primaryKeys.at(i);
		vector<field> datas = toInsertTable->data[key];
		for (size_t j = 0; j < datas.size(); j ++) {
			if (data[key].val == datas.at(j).val) {
                string tmp;
                tmp = toString (data[key].val);
                this->error_msg = "Duplicate entry '" + tmp +  "' for key 'PRIMARY'";
				return false;
			}
		}
    }
	
	// 插入数据		
    for (map<string, field>::iterator itr = data.begin(); itr != data.end(); itr ++) {
            string name = itr->first;
            field value = itr->second;
            toInsertTable->data[name].push_back (value);
    }

    cout <<"Query OK, 1 row affected (0.00 sec)" <<endl;
    return true;
}

void mSQL::_delete (mASTNode* stmt) {
    mASTNode* where_clause = NULL;
    string table_name = "";

    for (size_t i = 0; i < stmt->childs.size(); i ++) {
        if (stmt->childs.at(i)->type == G_FROM) {
            table_name = stmt->childs.at(i+1)->token->text;
        } else if (stmt->childs.at(i)->type == WHERE_CLAUSE) {
            where_clause = stmt->childs.at(i);
        }
    }

    toLowerCase (table_name);

    if (database.find(table_name) == database.end()) {
        error_msg =  "Table '" + table_name + "' doesn't exist";
        return;
    }

    table* toQueryTable = database[table_name];
    map<string, field>  single_data;
    size_t data_size = toQueryTable->data[toQueryTable->columns.begin()->first].size();
    vector<int> toDeleteSet;
    for (size_t i = 0; i < data_size; i ++) {
        for (map<string, fieldType>::iterator itr = toQueryTable->columns.begin(); itr != toQueryTable->columns.end(); itr++) {
            single_data[itr->first] = toQueryTable->data[itr->first][i];
        }

        if (getWhereClause (where_clause, single_data)) {
            toDeleteSet.push_back (i);
        }

        if (error_msg != "") return;

    }
    
    for (int i = toDeleteSet.size()-1; i >= 0; i --) {
        for (map<string, fieldType>::iterator itr = toQueryTable->columns.begin(); itr != toQueryTable->columns.end(); itr++) {
            toQueryTable->data[itr->first].erase (toQueryTable->data[itr->first].begin() + toDeleteSet.at(i));
        }
    }

    cout <<"Query OK, " <<toDeleteSet.size() <<" rows affected" <<endl;
}

void mSQL::query (mASTNode* stmt) {
    mASTNode* select_list = NULL;
    mASTNode* where_clause = NULL;
    string table_name = "";

    for (size_t i = 0; i < stmt->childs.size(); i ++) {
        if (stmt->childs.at(i)->type == G_FROM) {
            table_name = stmt->childs.at(i+1)->token->text;      // 需要查询的数据表名
        } else if (stmt->childs.at(i)->type == SELECT_LIST) {
            select_list = stmt->childs.at(i);                    // 需要查询的列
        } else if (stmt->childs.at(i)->type == WHERE_CLAUSE) {
            where_clause = stmt->childs.at(i);                   // 查询条件
        }
    }

    toLowerCase (table_name);

    if (database.find(table_name) == database.end()) {           // 数据表不存在，则提示错误
        error_msg =  "Table '" + table_name + "' doesn't exist";
        return;
    }

    table* toQueryTable = database[table_name];

    vector<string> columns;
    mASTNode* column_list = NULL;
    while (!select_list->childs.empty()) {
        if (select_list->childs.at(0)->type == COLUMN_LIST) {
            column_list = select_list->childs.at(0);             // 处理需要查询的column列表
            columns = getColumnList (column_list);
            break;
        } else if (select_list->childs.at(0)->type == G_MUL) {
            for (map<string, fieldType>::iterator itr = toQueryTable->columns.begin(); itr != toQueryTable->columns.end(); itr ++) {
                columns.push_back (itr->first);                  // 如果列表为*，则返回所有列表的数据
            }
            break;
        } else if (select_list->childs.at(0)->type == SELECT_LIST_) {
            select_list = select_list->childs.at(0);             // 重复处理colunn列表
        }
    }

    // 检查column是否合法
    for (size_t i = 0; i < columns.size(); i ++) {
        if (toQueryTable->columns.find (columns.at(i)) == toQueryTable->columns.end()) {
            this->error_msg = "Unknown column '" + columns.at(i) + "' in 'field list'";
            return;
        }
    }
    
	// 用于符合条件的结果存储起来，进行格式化输出
	int resultCount = 0;
	map<string, vector<field> > result;
	map<string, int> maxLength;           // 格式化输出时候，每个column的最大宽度

    map<string, field>  single_data;
    size_t data_size = toQueryTable->data[toQueryTable->columns.begin()->first].size();  // 数据表中所有记录的数目
    for (size_t i = 0; i < data_size; i ++) {
        for (map<string, fieldType>::iterator itr = toQueryTable->columns.begin(); itr != toQueryTable->columns.end(); itr++) {
            single_data[itr->first] = toQueryTable->data[itr->first][i];
        }

        if (getWhereClause (where_clause, single_data)) {                               // 进行条件判断，返回true则表示数据符合要求
			++ resultCount;
            for (size_t i = 0; i < columns.size(); i ++) {
				string col = columns.at(i);
				int val = single_data[col].val;
				result[col].push_back(val);
				maxLength[col] = maxLength[col] <= 0 ? 12 : maxLength[col];
				while (val/pow(10.0, maxLength[col]) > 1) {
					maxLength[col] ++;
				}
            }
        }

        if (error_msg != "") return;
    }
	
    if (result.empty() || result.begin()->second.empty()) { // mark
        cout <<"Emtpy set " <<endl;
        return;
    }
	// 考虑column名的长度，重新计算每一列所需的宽度
	int sum = 0;
    for (size_t i = 0; i < columns.size(); i ++) {
		string col = columns.at(i);
		if (col.size() > maxLength[col]) {
			maxLength[col] = col.size();
		}
		sum += maxLength[col];
	}
	 
    // mark
	// 输出间隔行
    for (size_t i = 0; i < columns.size(); i ++) {
		string col = columns.at(i);
		cout <<(i == 0 ? "|" : "") <<setw(maxLength[col]) <<setfill('-') <<"-" <<"|";
	}
	cout <<endl;

	// 输出column名
    for (size_t i = 0; i < columns.size(); i ++) {
		string col = columns.at(i);
        // 居中
        int pre_spaces = ceil((maxLength[col] - col.size())/2);
        int pos_spaces = maxLength[col] - pre_spaces - col.size();
		cout <<(i == 0 ? "|" : "") <<setw(pre_spaces) <<setfill(' ') <<' ' <<col <<setw(pos_spaces) <<setfill(' ') <<' ' <<"|";
	}
	cout <<endl;

	// 输出每一条查询的结果
	for (size_t i = 0; i < resultCount; i ++) {
		// 输出间隔行
		for (size_t j = 0; j < columns.size(); j ++) {
			string col = columns.at(j);
			cout <<(j == 0 ? "|" : "") <<setw(maxLength[col]) <<setfill('-') <<"-" <<"|";
		}
		cout <<endl;

		for (size_t j = 0; j < columns.size(); j ++) {
			string col = columns.at(j);
            int lenOfVal = 1;
            while (result[col].at(i).val/pow(10.0, lenOfVal) >= 1) {
				lenOfVal ++;
			}
            // 居中
            int pre_spaces = ceil((maxLength[col] - lenOfVal)/2);
            int pos_spaces = maxLength[col] - pre_spaces - lenOfVal;
		    cout <<(j == 0 ? "|" : "") <<setw(pre_spaces) <<setfill(' ') <<' ' <<result[col].at(i).val <<setw(pos_spaces) <<setfill(' ') <<' ' <<"|";
		}
		cout <<endl;
	}
	
	// 输出间隔行
    for (size_t i = 0; i < columns.size(); i ++) {
		string col = columns.at(i);
		cout <<(i == 0 ? "|" : "") <<setw(maxLength[col]) <<setfill('-') <<"-" <<"|";
	}
	cout <<endl;
}

vector<string> mSQL::getColumnList (mASTNode* column_list) {
    vector<string> keys;
    
    while (!column_list->childs.empty()) {
        for (size_t i = 0; i < column_list->childs.size(); i ++) {
            if (column_list->childs.at(i)->type == G_ID) {
                keys.push_back (column_list->childs.at(i)->token->text);
            } else if (column_list->childs.at(i)->type == COLUMN_LIST_) {
                column_list = column_list->childs.at(i);
                break;
            }
        }
    }

    return keys;
}

vector<field> mSQL::getValueList (mASTNode* value_list) {
    vector<field> data;
    
    while (!value_list->childs.empty()) {
        for (size_t i = 0; i < value_list->childs.size(); i ++) {
            if (value_list->childs.at(i)->type == SIMPLE_EXPR) {
                int result = getValueOfSimpleExpr (value_list->childs.at(i));
                if (error_msg != "") return vector<field>();
                data.push_back (field(result));
            } else if (value_list->childs.at(i)->type == VALUE_LIST_) {
                value_list = value_list->childs.at(i);
                break;
            }
        }
    }

    return data;
}

bool mSQL::getWhereClause (mASTNode* where_clause, map<string, field>& data) {
    mASTNode* disjunct = NULL;
    for (int i = 0; i < where_clause->childs.size(); i ++) {
        switch (where_clause->childs.at(i)->type) {
        case DISJUNCT:
            disjunct = where_clause->childs.at(i);
            return getValueOfDisjunct (disjunct, data);
            break;
        case EPSILON:
            return true;
            break;
        case WHERE_CLAUSE_:
            where_clause = where_clause->childs.at(i);
            i = -1;
            break;
        }
    }
    return true;
}

int mSQL::getValueOfSimpleExpr (mASTNode* simple_expr) {
    stack<int> operand;
    stack<char> operation;
    while (!simple_expr->childs.empty()) {
        mASTNode* simple_expr_ = simple_expr;
        for (size_t i = 0; i < simple_expr_->childs.size(); i ++) {
            switch (simple_expr_->childs.at(i)->type) {
            case G_ADD:
                {
                    if (operation.size() >= 1) {
                        int val2 = operand.top();
                        operand.pop();
                        int val1 = operand.top();
                        operand.pop();
                        int result = getValueOfFomula (val1, val2, operation.top());
                        if (error_msg != "") return false;
                        operand.push (result);
                        operation.pop();
                    }
                }
                operation.push ('+');
                break;
            case G_SUB:
                {
                    if (operation.size() >= 1) {
                        int val2 = operand.top();
                        operand.pop();
                        int val1 = operand.top();
                        operand.pop();
                        int result = getValueOfFomula (val1, val2, operation.top());
                        if (error_msg != "") return false;
                        operand.push (result);
                        operation.pop();
                    }
                }
                operation.push ('-');
                break;
            case SIMPLE_TERM:
                {
                    int val = getValueOfSimpleTerm (simple_expr_->childs.at(i));
                    if (error_msg != "") return false;
                    operand.push (val);
                    break;
                }
            case SIMPLE_EXPR_:
                simple_expr = simple_expr_->childs.at(i);
                break;
            }
        }

    }
    
    while (!operation.empty()) {
        int val2 = operand.top();
        operand.pop();
        int val1 = operand.top();
        operand.pop();
        int result = getValueOfFomula (val1, val2, operation.top());
        operation.pop();
        operand.push (result);
    }

    return operand.top();
}

int mSQL::getValueOfFomula (int val1, int val2, char operation) {
        int result = 0;
        switch (operation) {
        case '+':
            result = val1 + val2;
            break;
        case '-':
            result = val1 - val2;
            break;
        case '*':
            result = val1 * val2;
            break;
        case '/':
            if (val2 == 0) {
                error_msg = "Cannot div zero!";
                return 0;
            }
            result = val1 / val2;
            break;
        }
        return result;
}

int mSQL::getValueOfSimpleTerm (mASTNode* simple_term) {
    stack<int> operand;
    stack<char> operation;
    while (!simple_term->childs.empty()) {
        mASTNode* simple_term_ = simple_term;
        for (size_t i = 0; i < simple_term_->childs.size(); i ++) {
            switch (simple_term_->childs.at(i)->type) {
            case G_MUL:
                if (operation.size() >= 1 && (operation.top() == '*' || operation.top() == '/')) {
                    int val2 = operand.top();
                    operand.pop();
                    int val1 = operand.top();
                    operand.pop();
                    int result = getValueOfFomula (val1, val2, operation.top());
                    if (error_msg != "") return false;
                    operand.push (result);
                    operation.pop();
                }
                operation.push ('*');
                break;
            case G_DIV:
                if (operation.size() >= 1 && (operation.top() == '*' || operation.top() == '/')) {
                    int val2 = operand.top();
                    operand.pop();
                    int val1 = operand.top();
                    operand.pop();
                    int result = getValueOfFomula (val1, val2, operation.top());
                    if (error_msg != "") return false;
                    operand.push (result);
                    operation.pop();
                }
                operation.push ('/');
                break;
            case SIMPLE_UNARY:
                {
                    int val = getValueOfSimpleUnary (simple_term_->childs.at(i));
                    operand.push (val);
                    break;
                }
            case SIMPLE_TERM_:
                simple_term = simple_term_->childs.at(i);
                break;
            }
        }

    }

    while (!operation.empty()) {
        int val2 = operand.top();
        operand.pop();
        int val1 = operand.top();
        operand.pop();
        int result = getValueOfFomula (val1, val2, operation.top());
        if (error_msg != "") return false;
        operation.pop();
        operand.push (result);
    }

    return operand.top();
}

int mSQL::getValueOfSimpleUnary (mASTNode* simple_unary) {
    int num = 0;
    while (simple_unary != NULL) {
        int factor = 1;
        size_t length = simple_unary->childs.size();
        for (size_t i = 0; i < length; i ++) {
            switch (simple_unary->childs.at(i)->type) {
            case G_ADD:
                factor *= 1;
                break;
            case G_SUB:
                factor *= -1;
                break;
            case G_NUM:
                num = factor*atoi(simple_unary->childs.at(i)->token->text.c_str());
                i = length;
                simple_unary = NULL;
                break;
            case SIMPLE_UNARY_:
                num = factor*getValueOfSimpleUnary(simple_unary->childs.at(i));
                i = length;
                simple_unary = NULL;
                break;
            }
        }
    }
    return num;
}

bool mSQL::getValueOfDisjunct (mASTNode* disjunct, map<string, field>& data) {
    bool value, ready, finish;
    value = ready = finish = false;
    while (!finish && (!disjunct->childs.empty())) {
        mASTNode* disjunct_ = disjunct;
        for (size_t i = 0; i < disjunct_->childs.size(); i ++) {
            switch (disjunct_->childs.at(i)->type) {
            case G_OR:
                ready = true;
                break;
            case CONJUNCT:
                {
                    bool flag = getValueOfConjunct (disjunct_->childs.at(i), data);
                    if (error_msg != "") return false;
                    if (ready) {
                        value = value || flag;
                        ready = false;
                    } else {
                        value = flag;
                        finish = flag;
                    }
                }
                break;
            case DISJUNCT_:
                disjunct = disjunct_->childs.at(i);
                break;
            }
        }
    }

    return value;
}

bool mSQL::getValueOfConjunct (mASTNode* conjunct, map<string, field>& data) {
    bool value, ready, finish;
    value = ready = finish = false;
    while (!finish && (!conjunct->childs.empty())) {
        mASTNode* conjunct_ = conjunct;
        for (size_t i = 0; i < conjunct_->childs.size(); i ++) {
            switch (conjunct_->childs.at(i)->type) {
            case G_AND:
                ready = true;
                break;
            case BOOL:
                {
                    bool flag = getValueOfBoolExpr (conjunct_->childs.at(i), data);
                    if (error_msg != "") return false;
                    if (ready) {
                        value = value && flag;
                        ready = false;
                    } else {
                        value = flag;
                        finish = !flag;
                    }
                }
                break;
            case CONJUNCT_:
                conjunct = conjunct_->childs.at(i);
                break;
            }
        }
    }

    return value;
}

bool mSQL::getValueOfBoolExpr (mASTNode* bool_expr, map<string, field>& data) {
    bool value = false;
    bool isMinus = false;
    for (int i = 0; i < bool_expr->childs.size(); i ++) {
        switch (bool_expr->childs.at(i)->type) {
        case G_LEFT_PAREM:
            break;
        case G_RIGHT_PAREM:
            break;
        case DISJUNCT:
            value = getValueOfDisjunct (bool_expr->childs.at(i), data);
            if (error_msg != "") return false;
            break;
        case BOOL:
            value = getValueOfBoolExpr (bool_expr->childs.at(i), data);
            if (error_msg != "") return false;
            value = (!value && isMinus) || (value && !isMinus);
            break;
        case G_NOT:
            isMinus = true;
            break;
        case COMP:
            value = getValueOfComp (bool_expr->childs.at(i), data);
            if (error_msg != "") return false;
            break;
        case BOOL_:
            bool_expr = bool_expr->childs.at(i);
            i = -1;
            break;
        }
    }

    return value;
}

bool mSQL::getValueOfComp (mASTNode* bool_expr, map<string, field>& data) {
    bool value = false, ready = false;
    int val1(0), val2(0);
    int operation = 0;
    for (size_t i = 0; i < bool_expr->childs.size(); i ++) {
        switch (bool_expr->childs.at(i)->type) {
        case EXPR:
            if (!ready) {
                val1 = getValueOfCompExpr (bool_expr->childs.at(i), data);
                if (error_msg != "") return false;
            } else {
                val2 = getValueOfCompExpr (bool_expr->childs.at(i), data);
                if (error_msg != "") return false;
                switch (operation) {
                    case 0:
                        value = (val1 != val2);
                        break;
                    case 1:
                        value = (val1 == val2);
                        break;
                    case 2:
                        value = (val1 > val2);
                        break;
                    case 3:
                        value = (val1 < val2);
                        break;
                    case 4:
                        value = (val1 >= val2);
                        break;
                    case 5:
                        value = (val1 <= val2);
                        break;
                    }
            }
            break;
        case ROP:
            {
                mASTNode* rop = bool_expr->childs.at(i);
                mASTNode* rop_ = rop->childs.at(0);
                switch (rop_->childs.at(0)->type) {
                case G_NOT_EQUAL:
                    operation = 0;
                    break;
                case G_EQUAL:
                    operation = 1;
                    break;
                case G_GREATER:
                    operation = 2;
                    break;
                case G_LESS:
                    operation = 3;
                    break;
                case G_GREATER_EQUAL:
                    operation = 4;
                    break;
                case G_LESS_EQUAL:
                    operation = 5;
                    break;
                }
                ready = true;
            }
            break;
        }
    }

    return value;
}

int mSQL::getValueOfCompExpr (mASTNode* comp_expr, map<string, field>& data) {
    stack<int> operand;
    stack<char> operation;
    while (!comp_expr->childs.empty()) {
        mASTNode* comp_expr_ = comp_expr;
        for (size_t i = 0; i < comp_expr_->childs.size(); i ++) {
            switch (comp_expr_->childs.at(i)->type) {
            case G_ADD:
                {
                    if (operation.size() >= 1) {
                        int val2 = operand.top();
                        operand.pop();
                        int val1 = operand.top();
                        operand.pop();
                    int result = getValueOfFomula (val1, val2, operation.top());
                        if (error_msg != "") return false;
                        operand.push (result);
                        operation.pop();
                    }
                }
                operation.push ('+');
                break;
            case G_SUB:
                {
                    if (operation.size() >= 1) {
                        int val2 = operand.top();
                        operand.pop();
                        int val1 = operand.top();
                        operand.pop();
                        int result = getValueOfFomula (val1, val2, operation.top());
                        if (error_msg != "") return false;
                        operand.push (result);
                        operation.pop();
                    }
                }
                operation.push ('-');
                break;
            case TERM:
                {
                    int val = getValueOfTerm (comp_expr_->childs.at(i), data);
                    operand.push (val);
                    break;
                }
            case EXPR_:
                comp_expr = comp_expr_->childs.at(i);
                break;
            }
        }

    }
    
    while (operand.size() >= 2) {
        int val2 = operand.top();
        operand.pop();
        int val1 = operand.top();
        operand.pop();
        int result = getValueOfFomula (val1, val2, operation.top());
        operation.pop();
        operand.push (result);
    }

    return operand.top();
}

int mSQL::getValueOfTerm (mASTNode* term, map<string, field>& data) {
    stack<int> operand;
    stack<char> operation;
    while (!term->childs.empty()) {
        mASTNode* term_ = term;
        for (size_t i = 0; i < term_->childs.size(); i ++) {
            switch (term_->childs.at(i)->type) {
            case G_MUL:
                if (operation.size() >= 1 && (operation.top() == '*' || operation.top() == '/')) {
                    int val2 = operand.top();
                    operand.pop();
                    int val1 = operand.top();
                    operand.pop();
                    int result = getValueOfFomula (val1, val2, operation.top());
                    if (error_msg != "") return false;
                    operand.push (result);
                    operation.pop();
                }
                operation.push ('*');
                break;
            case G_DIV:
                if (operation.size() >= 1 && (operation.top() == '*' || operation.top() == '/')) {
                    int val2 = operand.top();
                    operand.pop();
                    int val1 = operand.top();
                    operand.pop();
                    int result = getValueOfFomula (val1, val2, operation.top());
                    if (error_msg != "") return false;
                    operand.push (result);
                    operation.pop();
                }
                operation.push ('/');
                break;
            case UNARY:
                {
                    int val = getValueOfUnary (term_->childs.at(i), data);
                    if (error_msg != "") return false;
                    operand.push (val);
                    break;
                }
            case TERM_:
                term = term_->childs.at(i);
                break;
            }
        }

    }

    while (operand.size() >= 2) {
        int val2 = operand.top();
        operand.pop();
        int val1 = operand.top();
        operand.pop();
        int result = getValueOfFomula (val1, val2, operation.top());
        if (error_msg != "") return false;
        operation.pop();
        operand.push (result);
    }

    return operand.top();
}

int mSQL::getValueOfUnary (mASTNode* unary, map<string, field>& data) {
    int num = 0;
    while (unary != NULL) {
        int factor = 1;
        size_t length = unary->childs.size();
        for (size_t i = 0; i < length; i ++) {
            switch (unary->childs.at(i)->type) {
            case G_ADD:
                factor *= 1;
                break;
            case G_SUB:
                factor *= -1;
                break;
            case G_NUM:
                num = factor*atoi(unary->childs.at(i)->token->text.c_str());
                i = length;
                unary = NULL;
                break;
            case G_ID:
                if (data.find(unary->childs.at(i)->token->text) == data.end()) {
                    error_msg = "There is no column name " + unary->childs.at(i)->token->text;
                    return 0;
                }
                num = factor*data[unary->childs.at(i)->token->text].val;
                i = length;
                unary = NULL;
                break;
            case UNARY_:
                num = factor*getValueOfUnary(unary->childs.at(i), data);
                if (error_msg != "") return false;
                i = length;
                unary = NULL;
                break;
            }
        }
    }
    return num;
}

void toLowerCase (string& str) {
    for (size_t i = 0; i < str.size(); i ++) {
        if (str.at(i) >= 'A' && str.at(i) <= 'Z')
            str[i] = str.at(i) - 'A' + 'a';
    }
}

string toString (int n) {
	ostringstream os; 
    os << n; 
    return os.str(); 
}
