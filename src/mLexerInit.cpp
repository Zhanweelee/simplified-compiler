#include <string>
#include <map>
using namespace std;

#include "mLexer.h"

mLexer* mLexerInit() {
    mState* states = new mState[mSTATE_NUM];
    for (int i = 0; i < mSTATE_NUM; i ++)
		states[i].stateID = i;

	// ×´Ì¬READY
    states[READY].push(IS_DIGIT, &states[INTEGER])
        .push(IS_LETTER, &states[IDENTIFIER])
        .push('c', &states[C_REATE]).push('C', &states[C_REATE])
        .push('t', &states[T_ABLE]).push('T', &states[T_ABLE])
        .push('p', &states[P_RIMARY]).push('P', &states[P_RIMARY])
        .push('k', &states[K_EY]).push('K', &states[K_EY])
        .push('d', &states[D_EFAULT]).push('D', &states[D_EFAULT])
        .push('i', &states[I_NSERT]).push('I', &states[I_NSERT])
        .push('i', &states[I_NTO]).push('I', &states[I_NTO])
        .push('i', &states[I_NT]).push('I', &states[I_NT])
        .push('v', &states[V_ALUES]).push('V', &states[V_ALUES])
        .push('d', &states[D_ELETE]).push('D', &states[D_ELETE])
        .push('d', &states[D_EFAULT]).push('D', &states[D_EFAULT])
        .push('f', &states[F_ROM]).push('F', &states[F_ROM])
        .push('w', &states[W_HERE]).push('W', &states[W_HERE])
        .push('s', &states[S_ELECT]).push('S', &states[S_ELECT])
		.push('<', &states[LESS]).push('>', &states[GREATER])
        .push('=', &states[ASSIGN])
        .push('+', &states[ADDITION]).push('-', &states[SUBTRACTION])
        .push('*', &states[MULTIPLY]).push('/', &states[DIVISION])
		.push('&', &states[BIT_AND]).push('|', &states[BIT_OR])
		.push(',', &states[COMMA]).push(';', &states[SEMICOLON])
        .push('(', &states[LEFT_PAREM])
		.push(')', &states[RIGHT_PAREN])
		.push('!', &states[REVERSE])
		.push('\n', &states[ENTER])
		.push(IS_WHITESPACE, &states[END]);
	
	// ×´Ì¬READY->(INTEGER)->FINISH
	states[INTEGER].push(IS_DIGIT, &states[INTEGER])
		.push(IS_LETTER, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	// ×´Ì¬READY->(IDENTIFIER)->FINISH
	states[IDENTIFIER].push(IS_LETTER, &states[IDENTIFIER])
		.push(IS_DIGIT, &states[IDENTIFIER])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	// ×´Ì¬READY->C_REATE->...->CREATE->FINISH
	states[C_REATE].push('r', &states[CR_EATE]).push('R', &states[CR_EATE]);
	states[CR_EATE].push('e', &states[CRE_ATE]).push('E', &states[CRE_ATE]);
	states[CRE_ATE].push('a', &states[CREA_TE]).push('A', &states[CREA_TE]);
	states[CREA_TE].push('t', &states[CREAT_E]).push('T', &states[CREAT_E]);
	states[CREAT_E].push('e', &states[CREATE]).push('E', &states[CREATE]);
	states[CREATE].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);



	// ×´Ì¬READY->T_ABLE->...->TABLE->FINISH
	states[T_ABLE].push('a', &states[TA_BLE]).push('A', &states[TA_BLE]);
	states[TA_BLE].push('b', &states[TAB_LE]).push('B', &states[TAB_LE]);
	states[TAB_LE].push('l', &states[TABL_E]).push('L', &states[TABL_E]);
	states[TABL_E].push('e', &states[TABLE]).push('E', &states[TABLE]);
	states[TABLE].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
	
	// ×´Ì¬READY->I_NT->...->INT->FINISH
	states[I_NT].push('n', &states[IN_T]).push('N', &states[IN_T]);
	states[IN_T].push('t', &states[INT]).push('T', &states[INT]);
	states[INT].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	// ×´Ì¬READY->P_RIMARY->...->PRIMARY->FINISH
	states[P_RIMARY].push('r', &states[PR_IMARY]).push('R', &states[PR_IMARY]);
	states[PR_IMARY].push('i', &states[PRI_MARY]).push('I', &states[PRI_MARY]);
	states[PRI_MARY].push('m', &states[PRIM_ARY]).push('M', &states[PRIM_ARY]);
	states[PRIM_ARY].push('a', &states[PRIMA_RY]).push('A', &states[PRIMA_RY]);
	states[PRIMA_RY].push('r', &states[PRIMAR_Y]).push('R', &states[PRIMAR_Y]);
	states[PRIMAR_Y].push('y', &states[PRIMARY]).push('Y', &states[PRIMARY]);
	states[PRIMARY].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	// ×´Ì¬READY->K_EY->...->KEY->FINISH
	states[K_EY].push('e', &states[KE_Y]).push('E', &states[KE_Y]);
	states[KE_Y].push('y', &states[KEY]).push('Y', &states[KEY]);
	states[KEY].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	// ×´Ì¬READY->D_EFAULT->...->DEFAULT->FINISH
	states[D_EFAULT].push('e', &states[DE_FAULT]).push('E', &states[DE_FAULT]);
	states[DE_FAULT].push('f', &states[DEF_AULT]).push('F', &states[DEF_AULT]);
	states[DEF_AULT].push('a', &states[DEFA_ULT]).push('A', &states[DEFA_ULT]);
	states[DEFA_ULT].push('u', &states[DEFAU_LT]).push('U', &states[DEFAU_LT]);
	states[DEFAU_LT].push('l', &states[DEFAUL_T]).push('L', &states[DEFAUL_T]);
	states[DEFAUL_T].push('t', &states[DEFAULT]).push('T', &states[DEFAULT]);
	states[DEFAULT].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	// ×´Ì¬READY->D_ELETE->...->DELETE->FINISH
	states[D_ELETE].push('e', &states[DE_LETE]).push('E', &states[DE_LETE]);
	states[DE_LETE].push('l', &states[DEL_ETE]).push('L', &states[DEL_ETE]);
	states[DEL_ETE].push('e', &states[DELE_TE]).push('E', &states[DELE_TE]);
	states[DELE_TE].push('t', &states[DELET_E]).push('T', &states[DELET_E]);
	states[DELET_E].push('e', &states[DELETE]).push('E', &states[DELETE]);
	states[DELETE].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	// ×´Ì¬READY->I_NSERT->...->INSERT->FINISH
	states[I_NSERT].push('n', &states[IN_SERT]).push('N', &states[IN_SERT]);
	states[IN_SERT].push('s', &states[INS_ERT]).push('S', &states[INS_ERT]);
	states[INS_ERT].push('e', &states[INSE_RT]).push('E', &states[INSE_RT]);
	states[INSE_RT].push('r', &states[INSER_T]).push('R', &states[INSER_T]);
	states[INSER_T].push('t', &states[INSERT]).push('T', &states[INSERT]);
	states[INSERT].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	// ×´Ì¬READY->I_NTO->...->INTO->FINISH
	states[I_NTO].push('n', &states[IN_TO]).push('N', &states[IN_TO]);
	states[IN_TO].push('t', &states[INT_O]).push('T', &states[INT_O]);
	states[INT_O].push('o', &states[INTO]).push('O', &states[INTO]);
	states[INTO].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	// ×´Ì¬READY->V_ALUES->...->VALUES->FINISH
	states[V_ALUES].push('a', &states[VA_LUES]).push('A', &states[VA_LUES]);
	states[VA_LUES].push('l', &states[VAL_UES]).push('L', &states[VAL_UES]);
	states[VAL_UES].push('u', &states[VALU_ES]).push('U', &states[VALU_ES]);
	states[VALU_ES].push('e', &states[VALUE_S]).push('E', &states[VALUE_S]);
	states[VALUE_S].push('s', &states[VALUES]).push('S', &states[VALUES]);
	states[VALUES].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	// ×´Ì¬READY->F_ROM->...->FROM->FINISH
	states[F_ROM].push('r', &states[FR_OM]).push('R', &states[FR_OM]);
	states[FR_OM].push('o', &states[FRO_M]).push('O', &states[FRO_M]);
	states[FRO_M].push('m', &states[FROM]).push('M', &states[FROM]);
	states[FROM].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	// ×´Ì¬READY->W_HERE->...->WHERE->FINISH
	states[W_HERE].push('h', &states[WH_ERE]).push('H', &states[WH_ERE]);
	states[WH_ERE].push('e', &states[WHE_RE]).push('E', &states[WHE_RE]);
	states[WHE_RE].push('r', &states[WHER_E]).push('R', &states[WHER_E]);
	states[WHER_E].push('e', &states[WHERE]).push('E', &states[WHERE]);
	states[WHERE].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	// ×´Ì¬READY->S_ELECT->...->SELECT->FINISH
	states[S_ELECT].push('e', &states[SE_LECT]).push('E', &states[SE_LECT]);
	states[SE_LECT].push('l', &states[SEL_ECT]).push('L', &states[SEL_ECT]);
	states[SEL_ECT].push('e', &states[SELE_CT]).push('E', &states[SELE_CT]);
	states[SELE_CT].push('c', &states[SELEC_T]).push('C', &states[SELEC_T]);
	states[SELEC_T].push('t', &states[SELECT]).push('T', &states[SELECT]);
	states[SELECT].push(IS_LETTER, &states[END])
		.push(IS_DIGIT, &states[END])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
	
	// ×´Ì¬READY->(OPERATOR)->FINISH
	/* ×´Ì¬READY->LESS->FINISH
	 * ×´Ì¬READY->LESS->LESS_EQUAL->FINISH
	 * ×´Ì¬READY->LESS->NOT_EQUAL->FINISH
	 */
	states[LESS].push('=', &states[LESS_EQUAL])
		.push('>', &states[NOT_EQUAL])
		.push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
    
	states[LESS_EQUAL].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	states[NOT_EQUAL].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	/* ×´Ì¬READY->GREATER->FINISH
	 * ×´Ì¬READY->GREATER->GREATER_EQUAL->FINISH
	 */
	states[GREATER].push('=', &states[GREATER_EQUAL])
		.push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	states[GREATER_EQUAL].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	/* ×´Ì¬READY->ASSIGN->FINISH
	 * ×´Ì¬READY->ASSIGN->EQUAL->FINISH
	 */
	states[ASSIGN].push('=', &states[EQUAL])
		.push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
	
	states[EQUAL].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	/* ×´Ì¬READY->BIT_AND->FINISH
	 * ×´Ì¬READY->BIT_AND->LOG_AND->FINISH
	 */
	states[BIT_AND].push('&', &states[LOG_AND]);
	states[LOG_AND].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
	
	/* ×´Ì¬READY->BIT_OR->FINISH
	 * ×´Ì¬READY->BIT_OR->LOG_OR->FINISH
	 */
	states[BIT_OR].push('|', &states[LOG_OR]);
	states[LOG_OR].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	/* ×´Ì¬READY->COMMA->FINISH
	 */
	states[COMMA].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
	
	/* ×´Ì¬READY->COMMA->FINISH
	 */
	states[SEMICOLON].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
    
    /* ×´Ì¬LEFT_PARENºÍRIGHT_PARENµÄ×ªÒÆ(×óÀ¨ºÅ£¬ÓÒÀ¨ºÅ)
     */
    states[LEFT_PAREM].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
    
	states[RIGHT_PAREN].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

    
    /* ×´Ì¬REVERSEµÄ×ªÒÆ('!')
     */
    states[REVERSE].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

    /* ×´Ì¬ÔËËã·ûµÄ×ªÒÆ(¼Ó¼õ³Ë³ý)
     */
    states[ADDITION].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

    states[SUBTRACTION].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

    states[MULTIPLY].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

    states[DIVISION].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
	
    states[ENTER].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	/* ÖÕÖ¹×´Ì¬ºÍTokenÀàÐÍ¶ÔÓ¦ */
	/* Integer, Identifier,
	 * Create, Table, Primary, Default, Insert, Into, Values, Delete, From, Where, Select,
	 * Equal, Assign, Less, Greater, Less Equal, Greater Equal, Not Equal
	 * Comma, Semicolon, Unknown
	 */
	map<int, int> stateIDToTokenType;
	stateIDToTokenType[INTEGER] = NUM;
	stateIDToTokenType[IDENTIFIER] = ID;

	stateIDToTokenType[CREATE] = CR;
	stateIDToTokenType[TABLE] = TAB;
	stateIDToTokenType[PRIMARY] = PR;
	stateIDToTokenType[KEY] = KE;
	stateIDToTokenType[DEFAULT] = DEF;
	stateIDToTokenType[INSERT] = INS;
	stateIDToTokenType[INTO] = IT;
	stateIDToTokenType[INT] = IN;
	stateIDToTokenType[VALUES] = VAL;
	stateIDToTokenType[DELETE] = DEL;
	stateIDToTokenType[FROM] = FR;
	stateIDToTokenType[WHERE] = WH;
	stateIDToTokenType[SELECT] = SE;

	stateIDToTokenType[EQUAL] = EQ;
	stateIDToTokenType[LESS] = LS;
	stateIDToTokenType[GREATER] = GR;
	stateIDToTokenType[LESS_EQUAL] = LE;
	stateIDToTokenType[GREATER_EQUAL] = GE;
	stateIDToTokenType[NOT_EQUAL] = NE;
    
	stateIDToTokenType[ASSIGN] = AS;
	stateIDToTokenType[ADDITION] = ADD;
    stateIDToTokenType[SUBTRACTION] = SUB;
	stateIDToTokenType[MULTIPLY] = MUL;
    stateIDToTokenType[DIVISION] = DIV;
	
	stateIDToTokenType[BIT_AND] = BA;
	stateIDToTokenType[BIT_OR] = BO;
	stateIDToTokenType[LOG_AND] = LA;
	stateIDToTokenType[LOG_OR] = LO;

    stateIDToTokenType[LEFT_PAREM] = LP;
    stateIDToTokenType[RIGHT_PAREN] = RP;
	stateIDToTokenType[COMMA] = COM;
	stateIDToTokenType[SEMICOLON] = SEMI;

	stateIDToTokenType[ENTER] = EN;
	stateIDToTokenType[REVERSE] = NOT;

    /* ²âÊÔÊý¾Ý
     * SELECT sid, age FROM Student WHERE age > 15 && age < 18;
     * select selecta, wherea FROm table where selecta > 0 && wher < 10;
     * select selecta, wherea FROm table where selecta > 0 && wher < 10
     * ...
     */
    mLexer* lexer = new mLexer(states, stateIDToTokenType);
    /*
    lexer->process("SELECT sid, age FROM Student WHERE age <>==,, 15 && (age < 18);");
    lexer->process("SELECT sid, age FROM Student DEFAULT WHERE age <>==,, 15 || (age < 18);");
    lexer->process("SELECT sid, age FROM Student DELETE WHERE age <>==,, 15 && (age < 18);");
    lexer->process("select selecta, wherea FROm table where selecta > 0 && wher < 10;");
    lexer->process("select selecta, wherea FROm table where selecta > 0 && wher < 10");
    */
    return lexer;
}
