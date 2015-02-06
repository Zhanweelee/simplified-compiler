#include <string>
#include <map>
using namespace std;

#include "mLexer.h"
#include "mParser.h"
#include "mGrammar.h"

mParser* mParserInit() {
    mGrammar* ssql_stmt = new mGrammar (SSQL_STMT);
    mGrammar* create_stmt = new mGrammar(CREATE_STMT);
    mGrammar* t_create = new mGrammar(G_CREATE, true, new mToken(CR));
    mGrammar* t_table = new mGrammar(G_TABLE, true, new mToken(TAB));
    mGrammar* t_id = new mGrammar(G_ID, true, new mToken(ID));
    mGrammar* t_left_parenthesis = new mGrammar(G_LEFT_PAREM, true, new mToken(LP));
    mGrammar* decl_list = new mGrammar(DECL_LIST);
    mGrammar* decl_list_ = new mGrammar(DECL_LIST_);
    mGrammar* decl_list_1 = new mGrammar(DECL_LIST_);
    mGrammar* decl_list_2 = new mGrammar(DECL_LIST_);
    mGrammar* decl = new mGrammar(DECL);
    mGrammar* decl_1 = new mGrammar(DECL_);
    mGrammar* decl_2 = new mGrammar(DECL_);
    mGrammar* t_right_parenthesis = new mGrammar(G_RIGHT_PAREM, true, new mToken(RP));
    mGrammar* t_endl = new mGrammar(G_ENDL, true, new mToken(SEMI));
    mGrammar* t_epsilon = new mGrammar(G_EPSILON, true, new mToken(EPSILON));
    mGrammar* t_comma = new mGrammar(G_COMMA, true, new mToken(COM));
    mGrammar* t_int = new mGrammar(G_INT, true, new mToken(IN));
    mGrammar* t_primary = new mGrammar(G_PRIMARY, true, new mToken(PR));
    mGrammar* t_key = new mGrammar(G_KEY, true, new mToken(KE));
    mGrammar* t_default = new mGrammar(G_DEFAULT, true, new mToken(DEF));
    mGrammar* t_assign = new mGrammar(G_ASSIGN, true, new mToken(AS));
    mGrammar* t_num = new mGrammar(G_NUM, true, new mToken(NUM));
    
    mGrammar* insert_stmt = new mGrammar(INSERT_STMT);
    mGrammar* t_insert = new mGrammar(G_INSERT, true, new mToken(INS));
    mGrammar* t_into = new mGrammar(G_INTO, true, new mToken(IT));
    mGrammar* t_values = new mGrammar(G_VALUES, true, new mToken(VAL));
    mGrammar* value_list = new mGrammar(VALUE_LIST);
    mGrammar* value_list_ = new mGrammar(VALUE_LIST_);
    mGrammar* value_list_1 = new mGrammar(VALUE_LIST_);
    mGrammar* value_list_2 = new mGrammar(VALUE_LIST_);

	mGrammar* delete_stmt = new mGrammar(DELETE_STMT);
    mGrammar* t_delete = new mGrammar(G_DELETE, true, new mToken(DEL));
	mGrammar* t_from = new mGrammar(G_FROM, true, new mToken(FR));

	mGrammar* query_stmt = new mGrammar(QUERY_STMT);
	mGrammar* t_select = new mGrammar(G_SELECT, true, new mToken(SE));
	mGrammar* select_list = new mGrammar(SELECT_LIST);
	mGrammar* select_list_1 = new mGrammar (SELECT_LIST_);
	mGrammar* select_list_2 = new mGrammar (SELECT_LIST_);

	
    mGrammar* default_spec = new mGrammar(DEFAULT_SPEC);
    mGrammar* default_spec_1 = new mGrammar(DEFAULT_SPEC_);
    mGrammar* default_spec_2 = new mGrammar(DEFAULT_SPEC_);
	mGrammar* t_add = new mGrammar(G_ADD, true, new mToken(ADD));
	mGrammar* t_sub = new mGrammar(G_SUB, true, new mToken(SUB));
	mGrammar* t_mul = new mGrammar(G_MUL, true, new mToken(MUL));
	mGrammar* t_div = new mGrammar(G_DIV, true, new mToken(DIV));


    mGrammar* column_list = new mGrammar(COLUMN_LIST);
    mGrammar* column_list_ = new mGrammar(COLUMN_LIST_);
    mGrammar* column_list_1 = new mGrammar(COLUMN_LIST_);
    mGrammar* column_list_2 = new mGrammar(COLUMN_LIST_);
	
	mGrammar* simple_expr = new mGrammar(SIMPLE_EXPR);
	mGrammar* simple_term = new mGrammar(SIMPLE_TERM);
	mGrammar* simple_unary = new mGrammar(SIMPLE_UNARY);
	
	mGrammar* where_clause = new mGrammar(WHERE_CLAUSE);
	mGrammar* t_where = new mGrammar(G_WHERE, true, new mToken(WH));
	mGrammar* disjunct = new mGrammar(DISJUNCT);
	mGrammar* t_or = new mGrammar(G_OR, true, new mToken(LO));
	mGrammar* t_and = new mGrammar(G_AND, true, new mToken(LA));
	mGrammar* t_not = new mGrammar(G_NOT, true, new mToken(NOT));
    mGrammar* t_not_equal = new mGrammar(G_NOT_EQUAL, true, new mToken(NE));
    mGrammar* t_equal = new mGrammar(G_EQUAL, true, new mToken(EQ));
    mGrammar* t_greater = new mGrammar(G_GREATER, true, new mToken(GR));
    mGrammar* t_less = new mGrammar(G_LESS, true, new mToken(LS));
    mGrammar* t_greater_equal = new mGrammar(G_GREATER_EQUAL, true, new mToken(GE));
    mGrammar* t_less_equal = new mGrammar(G_LESS_EQUAL, true, new mToken(LE));
	mGrammar* conjunct = new mGrammar(CONJUNCT);
	mGrammar* conjunct_ = new mGrammar(CONJUNCT_);
	mGrammar* conjunct_1 = new mGrammar(CONJUNCT_);
	mGrammar* conjunct_2 = new mGrammar(CONJUNCT_);
	mGrammar* bool_expr = new mGrammar(BOOL);
	mGrammar* comp = new mGrammar(COMP);
	mGrammar* expr = new mGrammar(EXPR);
	mGrammar* rop = new mGrammar(ROP);
	mGrammar* term = new mGrammar(TERM);
	mGrammar* unary = new mGrammar(UNARY);




    /*
     * Create Statement
     * create_stmt → create table id (decl_list);
     * decl_list → decl | decl_list, decl
     * decl → id int default_spec | primary key (column_list)
     * default_spec → default = num | ε
     * column_list → id | column_list, id
     */

    /* create_stmt → create table id (decl_list); */
    create_stmt->subGrammars.push_back (t_create);
    create_stmt->subGrammars.push_back (t_table);
    create_stmt->subGrammars.push_back (t_id);
    create_stmt->subGrammars.push_back (t_left_parenthesis);
    create_stmt->subGrammars.push_back (decl_list);
    create_stmt->subGrammars.push_back (t_right_parenthesis);
    create_stmt->subGrammars.push_back (t_endl);
    
    /* decl_list → decl | decl_list, decl */
    decl_list->subGrammars.push_back(decl);
    decl_list->subGrammars.push_back(decl_list_);

    decl_list_1->subGrammars.push_back (t_comma);
    decl_list_1->subGrammars.push_back (decl);
    decl_list_1->subGrammars.push_back (decl_list_);
    decl_list_2->subGrammars.push_back (t_epsilon);

    decl_list_->isRaw = false;
    decl_list_->subGrammars.push_back(decl_list_1);
    decl_list_->subGrammars.push_back(decl_list_2);
    
    /* decl → id int default_spec | primary key (column_list) */
    decl_1->subGrammars.push_back(t_id);
    decl_1->subGrammars.push_back(t_int);
    decl_1->subGrammars.push_back(default_spec);
    
    decl_2->subGrammars.push_back(t_primary);
    decl_2->subGrammars.push_back(t_key);
    decl_2->subGrammars.push_back(t_left_parenthesis);
    decl_2->subGrammars.push_back(column_list);
    decl_2->subGrammars.push_back(t_right_parenthesis);

    decl->isRaw = false;
    decl->subGrammars.push_back(decl_1);
    decl->subGrammars.push_back(decl_2);
    
    /* default_spec → default = num | ε */
    default_spec_1->subGrammars.push_back(t_default);
    default_spec_1->subGrammars.push_back(t_assign);
    default_spec_1->subGrammars.push_back(simple_expr);
    
    default_spec_2->subGrammars.push_back(t_epsilon);

    default_spec->isRaw = false;
    default_spec->subGrammars.push_back(default_spec_1);
    default_spec->subGrammars.push_back(default_spec_2);
    
    /* column_list → id | column_list, id */
    column_list_1->subGrammars.push_back(t_comma);
    column_list_1->subGrammars.push_back(t_id);
    column_list_1->subGrammars.push_back(column_list_);

    column_list_2->subGrammars.push_back(t_epsilon);
    column_list_->isRaw = false;
    column_list_->subGrammars.push_back(column_list_1);
    column_list_->subGrammars.push_back(column_list_2);

    column_list->subGrammars.push_back(t_id);
    column_list->subGrammars.push_back(column_list_);

    /*
     * Insert Statement
     * insert_stmt → insert into id(column_list) values (value_list);
     * value_list → num | value_list, num
     */

    /* insert_stmt → insert into id(column_list) values (value_list); */
    insert_stmt->subGrammars.push_back(t_insert);
    insert_stmt->subGrammars.push_back(t_into);
    insert_stmt->subGrammars.push_back(t_id);
    insert_stmt->subGrammars.push_back(t_left_parenthesis);
    insert_stmt->subGrammars.push_back(column_list);
    insert_stmt->subGrammars.push_back(t_right_parenthesis);
    insert_stmt->subGrammars.push_back(t_values);
    insert_stmt->subGrammars.push_back(t_left_parenthesis);
    insert_stmt->subGrammars.push_back(value_list);
    insert_stmt->subGrammars.push_back(t_right_parenthesis);
    insert_stmt->subGrammars.push_back (t_endl);
    
	
	/*
     * Delete Statement
     * delete_stmt → delete from id where_clause;
	 */
	delete_stmt->subGrammars.push_back (t_delete);
	delete_stmt->subGrammars.push_back (t_from);
	delete_stmt->subGrammars.push_back (t_id);
	delete_stmt->subGrammars.push_back (where_clause);
    delete_stmt->subGrammars.push_back (t_endl);

	/*
     * Query Statement
     * query_stmt → select select_list from id where_clause;
	 * select_list → column_list | *
	 */
	query_stmt->subGrammars.push_back (t_select);
	query_stmt->subGrammars.push_back (select_list);
	query_stmt->subGrammars.push_back (t_from);
	query_stmt->subGrammars.push_back (t_id);
	query_stmt->subGrammars.push_back (where_clause);
    query_stmt->subGrammars.push_back (t_endl);

    
	mGrammar* rop1 = new mGrammar (ROP_);
	mGrammar* rop2 = new mGrammar (ROP_);
	mGrammar* rop3 = new mGrammar (ROP_);
	mGrammar* rop4 = new mGrammar (ROP_);
	mGrammar* rop5 = new mGrammar (ROP_);
	mGrammar* rop6 = new mGrammar (ROP_);
    rop1->subGrammars.push_back (t_not_equal);
    rop2->subGrammars.push_back (t_equal);
    rop3->subGrammars.push_back (t_greater);
    rop4->subGrammars.push_back (t_less);
    rop5->subGrammars.push_back (t_greater_equal);
    rop6->subGrammars.push_back (t_less_equal);
    rop->isRaw = false;
    rop->subGrammars.push_back (rop1);
    rop->subGrammars.push_back (rop2);
    rop->subGrammars.push_back (rop3);
    rop->subGrammars.push_back (rop4);
    rop->subGrammars.push_back (rop5);
    rop->subGrammars.push_back (rop6);


	select_list_1->subGrammars.push_back (column_list);
	select_list->isRaw = false;
	select_list_2->subGrammars.push_back (t_mul);
	select_list->subGrammars.push_back (select_list_1);
	select_list->subGrammars.push_back (select_list_2);
	
    /* value_list → num | value_list, num */
	value_list->subGrammars.push_back (simple_expr);
	value_list->subGrammars.push_back (value_list_);
	
	value_list_1->subGrammars.push_back (t_comma);
	value_list_1->subGrammars.push_back (simple_expr);
	value_list_1->subGrammars.push_back (value_list_);
	
	value_list_2->subGrammars.push_back (t_epsilon);
	
	value_list_->isRaw = false;
	value_list_->subGrammars.push_back (value_list_1);
	value_list_->subGrammars.push_back (value_list_2);
	
	
	mGrammar* simple_expr_ = new mGrammar (SIMPLE_EXPR_);
	simple_expr->subGrammars.push_back (simple_term);
	simple_expr->subGrammars.push_back (simple_expr_);

	mGrammar* simple_expr_1 = new mGrammar (SIMPLE_EXPR_);
	mGrammar* simple_expr_2 = new mGrammar (SIMPLE_EXPR_);
	mGrammar* simple_expr_3 = new mGrammar (SIMPLE_EXPR_);
	simple_expr_1->subGrammars.push_back (t_epsilon);
	
	simple_expr_2->subGrammars.push_back (t_add);
	simple_expr_2->subGrammars.push_back (simple_term);
	simple_expr_2->subGrammars.push_back (simple_expr_);
	
	simple_expr_3->subGrammars.push_back (t_sub);
	simple_expr_3->subGrammars.push_back (simple_term);
	simple_expr_3->subGrammars.push_back (simple_expr_);
	
	simple_expr_->isRaw = false;
	simple_expr_->subGrammars.push_back (simple_expr_1);
	simple_expr_->subGrammars.push_back (simple_expr_2);
	simple_expr_->subGrammars.push_back (simple_expr_3);
	
	mGrammar* simple_term_ = new mGrammar (SIMPLE_TERM_);
	simple_term->subGrammars.push_back (simple_unary);
	simple_term->subGrammars.push_back (simple_term_);

	mGrammar* simple_term_1 = new mGrammar (SIMPLE_TERM_);
	mGrammar* simple_term_2 = new mGrammar (SIMPLE_TERM_);
	mGrammar* simple_term_3 = new mGrammar (SIMPLE_TERM_);
	simple_term_1->subGrammars.push_back (t_epsilon);
	
	simple_term_2->subGrammars.push_back (t_mul);
	simple_term_2->subGrammars.push_back (simple_unary);
	simple_term_2->subGrammars.push_back (simple_term_);
	
	simple_term_3->subGrammars.push_back (t_div);
	simple_term_3->subGrammars.push_back (simple_unary);
	simple_term_3->subGrammars.push_back (simple_term_);
	
	simple_term_->isRaw = false;
	simple_term_->subGrammars.push_back (simple_term_1);
	simple_term_->subGrammars.push_back (simple_term_2);
	simple_term_->subGrammars.push_back (simple_term_3);
	
	mGrammar* simple_unary1 = new mGrammar (SIMPLE_UNARY_);
	mGrammar* simple_unary2 = new mGrammar (SIMPLE_UNARY_);
	mGrammar* simple_unary3 = new mGrammar (SIMPLE_UNARY_);
	mGrammar* simple_unary4 = new mGrammar (SIMPLE_UNARY_);
	simple_unary1->subGrammars.push_back (t_left_parenthesis);
	simple_unary1->subGrammars.push_back (simple_expr);
	simple_unary1->subGrammars.push_back (t_left_parenthesis);
	
	simple_unary2->subGrammars.push_back (t_sub);
	simple_unary2->subGrammars.push_back (simple_unary);
	
	simple_unary3->subGrammars.push_back (t_add);
	simple_unary3->subGrammars.push_back (simple_unary);
	
	simple_unary4->subGrammars.push_back (t_num);

	simple_unary->isRaw = false;
	simple_unary->subGrammars.push_back (simple_unary1);
	simple_unary->subGrammars.push_back (simple_unary2);
	simple_unary->subGrammars.push_back (simple_unary3);
	simple_unary->subGrammars.push_back (simple_unary4);
	
	/*
	 * where_clause
	 */
	
	mGrammar* where_clause1 = new mGrammar (WHERE_CLAUSE_);
	mGrammar* where_clause2 = new mGrammar (WHERE_CLAUSE_);
	where_clause1->subGrammars.push_back (t_where);
	where_clause1->subGrammars.push_back (disjunct);
	
	where_clause2->subGrammars.push_back (t_epsilon);

	where_clause->isRaw = false;
	where_clause->subGrammars.push_back (where_clause1);
	where_clause->subGrammars.push_back (where_clause2);
	
	mGrammar* disjunct_ = new mGrammar (DISJUNCT_);
	mGrammar* disjunct_1 = new mGrammar (DISJUNCT_);
	mGrammar* disjunct_2 = new mGrammar (DISJUNCT_);
	disjunct->subGrammars.push_back (conjunct);
	disjunct->subGrammars.push_back (disjunct_);
	
	disjunct_1->subGrammars.push_back (t_or);
	disjunct_1->subGrammars.push_back (conjunct);
	disjunct_1->subGrammars.push_back (disjunct_);
	
	disjunct_2->subGrammars.push_back (t_epsilon);

	disjunct_->isRaw = false;
	disjunct_->subGrammars.push_back (disjunct_1);
	disjunct_->subGrammars.push_back (disjunct_2);

	conjunct->subGrammars.push_back (bool_expr);
	conjunct->subGrammars.push_back (conjunct_);
	
	conjunct_1->subGrammars.push_back (t_and);
	conjunct_1->subGrammars.push_back (bool_expr);
	conjunct_1->subGrammars.push_back (conjunct_);
	
	conjunct_2->subGrammars.push_back (t_epsilon);

	conjunct_->isRaw = false;
	conjunct_->subGrammars.push_back (conjunct_1);
	conjunct_->subGrammars.push_back (conjunct_2);
	
	mGrammar* bool_expr1 = new mGrammar (BOOL_);
	mGrammar* bool_expr2 = new mGrammar (BOOL_);
	mGrammar* bool_expr3 = new mGrammar (BOOL_);
	bool_expr1->subGrammars.push_back (t_left_parenthesis);
	bool_expr1->subGrammars.push_back (disjunct);
	bool_expr1->subGrammars.push_back (t_right_parenthesis);
	
	bool_expr2->subGrammars.push_back (t_not);
	bool_expr2->subGrammars.push_back (bool_expr);
	
	bool_expr3->subGrammars.push_back (comp);

	bool_expr->isRaw = false;
	bool_expr->subGrammars.push_back (bool_expr1);
	bool_expr->subGrammars.push_back (bool_expr2);
	bool_expr->subGrammars.push_back (bool_expr3);
	
	comp->subGrammars.push_back (expr);
	comp->subGrammars.push_back (rop);
	comp->subGrammars.push_back (expr);
	
	mGrammar* expr_ = new mGrammar (EXPR_);
	expr->subGrammars.push_back (term);
	expr->subGrammars.push_back (expr_);

	mGrammar* expr_1 = new mGrammar (EXPR_);
	mGrammar* expr_2 = new mGrammar (EXPR_);
	mGrammar* expr_3 = new mGrammar (EXPR_);
	expr_1->subGrammars.push_back (t_epsilon);

	expr_2->subGrammars.push_back (t_add);
	expr_2->subGrammars.push_back (term);
	expr_2->subGrammars.push_back (expr_);
	
	expr_3->subGrammars.push_back (t_sub);
	expr_3->subGrammars.push_back (term);
	expr_3->subGrammars.push_back (expr_);

	expr_->isRaw = false;
	expr_->subGrammars.push_back (expr_1);
	expr_->subGrammars.push_back (expr_2);
	expr_->subGrammars.push_back (expr_3);
	
	mGrammar* term_ = new mGrammar (TERM_);
	term->subGrammars.push_back (unary);
	term->subGrammars.push_back (term_);

	mGrammar* term_1 = new mGrammar (TERM_);
	mGrammar* term_2 = new mGrammar (TERM_);
	mGrammar* term_3 = new mGrammar (TERM_);
	term_1->subGrammars.push_back (t_epsilon);

	term_2->subGrammars.push_back (t_mul);
	term_2->subGrammars.push_back (unary);
	term_2->subGrammars.push_back (term_);
	
	term_3->subGrammars.push_back (t_div);
	term_3->subGrammars.push_back (unary);
	term_3->subGrammars.push_back (term_);

	term_->isRaw = false;
	term_->subGrammars.push_back (term_1);
	term_->subGrammars.push_back (term_2);
	term_->subGrammars.push_back (term_3);
	
	mGrammar* unary1 = new mGrammar (UNARY_);
	mGrammar* unary2 = new mGrammar (UNARY_);
	mGrammar* unary3 = new mGrammar (UNARY_);
	mGrammar* unary4 = new mGrammar (UNARY_);
	mGrammar* unary5 = new mGrammar (UNARY_);
	unary1->subGrammars.push_back (t_left_parenthesis);
	unary1->subGrammars.push_back (expr);
	unary1->subGrammars.push_back (t_right_parenthesis);
	
	unary2->subGrammars.push_back (t_sub);
	unary2->subGrammars.push_back (unary);
	
	unary3->subGrammars.push_back (t_add);
	unary3->subGrammars.push_back (unary);

	unary4->subGrammars.push_back (t_id);
	
	unary5->subGrammars.push_back (t_num);

	unary->isRaw = false;
	unary->subGrammars.push_back (unary1);
	unary->subGrammars.push_back (unary2);
	unary->subGrammars.push_back (unary3);
	unary->subGrammars.push_back (unary4);
	unary->subGrammars.push_back (unary5);


	ssql_stmt->isRaw = false;
	ssql_stmt->subGrammars.push_back (create_stmt);
	ssql_stmt->subGrammars.push_back (delete_stmt);
	ssql_stmt->subGrammars.push_back (insert_stmt);
	ssql_stmt->subGrammars.push_back (query_stmt);

    // 传递文法到Parser中
    mParser* myParser = new mParser(ssql_stmt);
    return myParser;
}