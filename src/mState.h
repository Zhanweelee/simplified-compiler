#include <string>
#include <map>
using namespace std;

#ifndef M_STATE
#define M_STATE

// 定义state的类型
enum mSTATE_TYPE {
READY,
INTEGER, IDENTIFIER,
C_REATE, CR_EATE, CRE_ATE, CREA_TE, CREAT_E, CREATE,
T_ABLE, TA_BLE, TAB_LE, TABL_E, TABLE,
P_RIMARY, PR_IMARY, PRI_MARY, PRIM_ARY, PRIMA_RY, PRIMAR_Y, PRIMARY,
K_EY, KE_Y, KEY,
D_EFAULT, DE_FAULT, DEF_AULT, DEFA_ULT, DEFAU_LT, DEFAUL_T, DEFAULT,
I_NSERT, IN_SERT, INS_ERT, INSE_RT, INSER_T, INSERT,
I_NTO, IN_TO, INT_O, INTO,
I_NT, IN_T, INT,
V_ALUES, VA_LUES, VAL_UES, VALU_ES, VALUE_S, VALUES,
D_ELETE, DE_LETE, DEL_ETE, DELE_TE, DELET_E, DELETE,
F_ROM, FR_OM, FRO_M, FROM,
W_HERE, WH_ERE, WHE_RE, WHER_E, WHERE,
S_ELECT, SE_LECT, SEL_ECT, SELE_CT, SELEC_T, SELECT,
LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, EQUAL, NOT_EQUAL,
ASSIGN, ADDITION, SUBTRACTION, MULTIPLY, DIVISION, 
BIT_AND, BIT_OR,
LOG_AND, LOG_OR,
REVERSE,
COMMA, SEMICOLON,
LEFT_PAREM, RIGHT_PAREN,
END, FINISH, ERROR,
ENTER,
mSTATE_NUM};

// 状态转移条件标记
enum CONDITION {IS_LETTER, IS_DIGIT, IS_OPERATOR, IS_WHITESPACE, IS_DELIMITER, UNKNOWN};
struct mState {
    int stateID;
    bool isEnd;
    string currntText;
    multimap<CONDITION, mState*> generalTransfer;
    multimap<char, mState*> specifiedTransfer;
    mState& push(CONDITION condition, mState* state) { generalTransfer.insert(make_pair(condition, state)); return *this; }
    mState& push(char ch, mState* state) { specifiedTransfer.insert(make_pair(ch, state)); return *this; }

    mState (int id = 0, string text = "", bool end = false): stateID(id), currntText(text), isEnd(end) {};
};

#endif