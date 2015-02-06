#include <string>

using std::string;

#ifndef M_TOKEN
#define M_TOKEN

// 定义token的类型
/* Integer, Identifier,
 * Create, Table, Primary, Default, Insert, Into, Values, Delete, From, Where, Select,
 * Equal, Less, Greater, Less Equal, Greater Equal, Not Equal
 * Comma, Semicolon, Unknown
 */
enum mTOKEN_TYPE {
NUM, ID,
CR, TAB, PR, KE, DEF, INS, IT, VAL, DEL, FR, WH, SE, IN,
EQ, LE, GE, LS, GR, NE,
AS, ADD, SUB, MUL, DIV,
BA, BO, LA, LO, NOT,
LP, RP,
COM, SEMI,
EN,
EPSILON, ENDL, UN,
mTOKEN_NUM};

// 定义token结构体
struct mToken {
    int type;
    string text;
    double value;
    mToken (int tp = UN, const string& t = "", double v = 0): type(tp), text(t), value(v) {};
};

#endif