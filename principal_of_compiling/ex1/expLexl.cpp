#include <iostream>
#include <string>
#include <math.h>
#include <map>
using namespace std;

// 单词种类的枚举
enum TokenID
{
    // keyword
    ALIGNAS,
    ALIGNOF,
    ANDB,
    AND_EQB,
    ASMA,
    AUTO,
    BITANDB,
    BITORB,
    BOOL,
    BREAK,
    CASE,
    CATCH,
    CHAR,
    CHAR8_T,
    CHAR16_T,
    CHAR32_T,
    CLASS,
    COMPLB,
    CONCEPTC,
    CONST,
    CONST_CAST,
    CONSTEVALC,
    CONSTEXPR,
    CONSTINITC,
    CONTINUE,
    CO_AWAITC,
    CO_RETURNC,
    CO_YIELDC,
    DECLTYPE,
    DEFAULT,
    DELETE,
    DO,
    DOUBLE,
    DYNAMIC_CAST,
    ELSE,
    ENUM,
    EXPLICIT,
    EXPORTC,
    EXTERN,
    FALSE,
    FLOAT,
    FOR,
    FRIEND,
    GOTO,
    IF,
    INLINE,
    INT,
    LONG,
    MUTABLE,
    NAMESPACE,
    NEW,
    NOEXCEPT,
    NOTB,
    NOT_EQB,
    NULLPTR,
    OPERATOR,
    ORB,
    OR_EQB,
    PRIVATE,
    PROTECTED,
    PUBLIC,
    REGISTER,
    REINTERPRET_CAST,
    REQUIRESC,
    RETURN,
    SHORT,
    SIGNED,
    SIZEOF,
    STATIC,
    STATIC_ASSERT,
    STATIC_CAST,
    STRUCT,
    SWITCH,
    TEMPLATE,
    THIS,
    THREAD_LOCAL,
    THROW,
    TRUE,
    TRY,
    TYPEDEF,
    TYPEID,
    TYPENAME,
    UNION,
    UNSIGNED,
    USING,
    VIRTUAL,
    VOID,
    VOLATILE,
    WCHAR_T,
    WHILE,
    XORB,
    XOR_EQB,

    SCOPE_RESOLUTION, // ::
    MSELECT_OBJ,      // member selection(objects) .
    PTM_OBJ,          // pointer-to-menber(objects) .*
    LSBRACKET,        // left square bracket [
    RSBRACKET,        // right square bracket ]
    LBRACKET,         // left bracket (
    RBRACKET,         // right bracket )
    COMPLEMENT,       // ~
    LNOT,             // logical not !
    NOTEQ,            // !=
    ADDRESSOF,        // &
    LAND,             // &&
    BAND_ASSIGN,      // bitwise and assignment &=
    MODULUS,          // %
    MODULUS_ASSIGN,   // %=
    BEXOR,            // bitwise exclusive or ^
    BEXOR_ASSIGN,     // bitwise exclusive or assign ^=
    BINOR,            // Bitwise inclusive or |
    BINOR_ASSIGN,     // Bitwise inclusive or assignment |=
    LOR,              // logical or ||
                      // 条件运算符?:存疑
    COMMA,            //,
    ADD,              // +
    INCREMENT,        //++
    ADD_ASSIGN,       //+=
    SUB,              // -
    DECREMENT,        //--
    SUB_ASSIGN,       //-=
    MSELECT_PT,       // member selection(pointers) ->
    PTM_PT,           // pointer-to-menber(pointers) .*
    MUL,              // *
    MUL_ASSIGN,       // *=
    DIV,              // /
    DIV_ASSIGN,       // /=
    ASSIGN,           // =
    EQ,               // ==
    LT,               // <
    LSHIFT,           // left shift <<
    LTE,              // <=
    LSHIFT_ASSIGN,    // left shift assignment
    GT,               // >
    RSHIFT,           // right shift
    GTE,              // >=
    RSHIFT_ASSIGN,    // right shift assignment

    NUMBER,
    ID,        // 标识符identifier
    SEMICOLON, // ；
    ENDINPUT
};

map<string, TokenID> keywords;

struct TokenStru
{
    TokenID ID;
    double val;
    char op[3];
    char word[20];
};

TokenStru token;
char buffer[255];
int pos = 0;

void init()
{
    keywords["ALIGNAS"] = ALIGNAS;
    keywords["ALIGNOF"] = ALIGNOF;
    keywords["ANDB"] = ANDB;
    keywords["AND_EQB"] = AND_EQB;
    keywords["ASMA"] = ASMA;
    keywords["AUTO"] = AUTO;
    keywords["BITANDB"] = BITANDB;
    keywords["BITORB"] = BITORB;
    keywords["BOOL"] = BOOL;
    keywords["BREAK"] = BREAK;
    keywords["CASE"] = CASE;
    keywords["CATCH"] = CATCH;
    keywords["CHAR"] = CHAR;
    keywords["CHAR8_T"] = CHAR8_T;
    keywords["CHAR16_T"] = CHAR16_T;
    keywords["CHAR32_T"] = CHAR32_T;
    keywords["CLASS"] = CLASS;
    keywords["COMPLB"] = COMPLB;
    keywords["CONCEPTC"] = CONCEPTC;
    keywords["CONST"] = CONST;
    keywords["CONST_CAST"] = CONST_CAST;
    keywords["CONSTEVALC"] = CONSTEVALC;
    keywords["CONSTEXPR"] = CONSTEXPR;
    keywords["CONSTINITC"] = CONSTINITC;
    keywords["CONTINUE"] = CONTINUE;
    keywords["CO_AWAITC"] = CO_AWAITC;
    keywords["CO_RETURNC"] = CO_RETURNC;
    keywords["CO_YIELDC"] = CO_YIELDC;
    keywords["DECLTYPE"] = DECLTYPE;
    keywords["DEFAULT"] = DEFAULT;
    keywords["DELETE"] = DELETE;
    keywords["DO"] = DO;
    keywords["DOUBLE"] = DOUBLE;
    keywords["DYNAMIC_CAST"] = DYNAMIC_CAST;
    keywords["ELSE"] = ELSE;
    keywords["ENUM"] = ENUM;
    keywords["EXPLICIT"] = EXPLICIT;
    keywords["EXPORTC"] = EXPORTC;
    keywords["EXTERN"] = EXTERN;
    keywords["FALSE"] = FALSE;
    keywords["FLOAT"] = FLOAT;
    keywords["FOR"] = FOR;
    keywords["FRIEND"] = FRIEND;
    keywords["GOTO"] = GOTO;
    keywords["IF"] = IF;
    keywords["INLINE"] = INLINE;
    keywords["INT"] = INT;
    keywords["LONG"] = LONG;
    keywords["MUTABLE"] = MUTABLE;
    keywords["NAMESPACE"] = NAMESPACE;
    keywords["NEW"] = NEW;
    keywords["NOEXCEPT"] = NOEXCEPT;
    keywords["NOTB"] = NOTB;
    keywords["NOT_EQB"] = NOT_EQB;
    keywords["NULLPTR"] = NULLPTR;
    keywords["OPERATOR"] = OPERATOR;
    keywords["ORB"] = ORB;
    keywords["OR_EQB"] = OR_EQB;
    keywords["PRIVATE"] = PRIVATE;
    keywords["PROTECTED"] = PROTECTED;
    keywords["PUBLIC"] = PUBLIC;
    keywords["REGISTER"] = REGISTER;
    keywords["REINTERPRET_CAST"] = REINTERPRET_CAST;
    keywords["REQUIRESC"] = REQUIRESC;
    keywords["RETURN"] = RETURN;
    keywords["SHORT"] = SHORT;
    keywords["SIGNED"] = SIGNED;
    keywords["SIZEOF"] = SIZEOF;
    keywords["STATIC"] = STATIC;
    keywords["STATIC_ASSERT"] = STATIC_ASSERT;
    keywords["STATIC_CAST"] = STATIC_CAST;
    keywords["STRUCT"] = STRUCT;
    keywords["SWITCH"] = SWITCH;
    keywords["TEMPLATE"] = TEMPLATE;
    keywords["THIS"] = THIS;
    keywords["THREAD_LOCAL"] = THREAD_LOCAL;
    keywords["THROW"] = THROW;
    keywords["TRUE"] = TRUE;
    keywords["TRY"] = TRY;
    keywords["TYPEDEF"] = TYPEDEF;
    keywords["TYPEID"] = TYPEID;
    keywords["TYPENAME"] = TYPENAME;
    keywords["UNION"] = UNION;
    keywords["UNSIGNED"] = UNSIGNED;
    keywords["USING"] = USING;
    keywords["VIRTUAL"] = VIRTUAL;
    keywords["VOID"] = VOID;
    keywords["VOLATILE"] = VOLATILE;
    keywords["WCHAR_T"] = WCHAR_T;
    keywords["WHILE"] = WHILE;
    keywords["XORB"] = XORB;
    keywords["XOR_EQB"] = XOR_EQB;
}