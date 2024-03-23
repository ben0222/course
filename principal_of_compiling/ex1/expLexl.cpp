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

    LCBRACKET, // }
    RCBRACKET, // {
    COLON,     // :
    QMARK,     // ?
    PSIGN,     // #
    BACKSLASH, /* \ */
    SEMICOLON, // ；
    NUMBER,
    ID, // 标识符identifier

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

void GetToken()
{
    int s;

    while (buffer[pos] == 32 || buffer[pos] == 9) // spacebar or tab
        pos++;

    if ((buffer[pos] >= '0') && (buffer[pos] <= '9')) // digit
    {
        s = 0;
        while (buffer[pos] >= '0' && buffer[pos] <= '9')
        {
            s = s * 10 + buffer[pos] - '0';
            pos++;
            if (buffer[pos] == 39) // separator '
                pos++;
        }
        token.ID = NUMBER; // 此处一设置tokenID为NUMBER，下面所有都不用再设置？
        token.val = s;

        if (buffer[pos] == '.') // float
        {
            int len = 0;
            s = 0;
            pos++;
            while (buffer[pos] >= '0' && buffer[pos] <= '9')
            {
                s = s * 10 + buffer[pos] - '0';
                len++;
                pos++;
            }
            token.val = token.val + s / pow(10, len);
        }

        if (buffer[pos] == 'e' || buffer[pos] == 'E') // exponent
        {
            bool ispositive = true;
            s = 0;
            pos++;
            if (buffer[pos] == '-')
            {
                ispositive = false;
                while (buffer[pos] >= '0' && buffer[pos] <= '9')
                {
                    s = s * 10 + buffer[pos] - '0';
                    pos++;
                }
            }
            else if (buffer[pos] == '+')
            {
                ispositive = true;
                while (buffer[pos] >= '0' && buffer[pos] <= '9')
                {
                    s = s * 10 + buffer[pos] - '0';
                    pos++;
                }
            }
            else if (buffer[pos] >= '0' && buffer[pos] <= '9')
            {
                ispositive = true;
                while (buffer[pos] >= '0' && buffer[pos] <= '9')
                {
                    s = s * 10 + buffer[pos] - '0';
                    pos++;
                }
            }
            if (ispositive)
                token.val = token.val * pow(10, s);
            else
                token.val = token.val * pow(10, -s);
        }

        if (s == 0) // binary, octal, and hexa number(start with 0)
        {
            while (buffer[pos] >= '0' && buffer[pos] <= '7') // octal
            {
                s = s * 8 + buffer[pos] - '0';
                pos++;
            }
            token.val = s;

            if (buffer[pos] == 'b' || buffer[pos] == 'B') // binary
            {
                pos++; // 跳过 'b' 或 'B'
                s = 0;
                while (buffer[pos] == '0' || buffer[pos] == '1')
                {
                    s = s * 2 + buffer[pos] - '0'; // 将二进制转换为十进制
                    pos++;
                }
                token.val = s;
            }

            if (buffer[pos] == 'x' || buffer[pos] == 'X') // hexa
            {
                pos++; // 跳过 'x' 或 'X'
                s = 0;
                while ((buffer[pos] >= '0' && buffer[pos] <= '9') ||
                       (buffer[pos] >= 'a' && buffer[pos] <= 'f') ||
                       (buffer[pos] >= 'A' && buffer[pos] <= 'F'))
                {
                    if (buffer[pos] >= '0' && buffer[pos] <= '9')
                        s = s * 16 + buffer[pos] - '0';
                    else if (buffer[pos] >= 'a' && buffer[pos] <= 'f')
                        s = s * 16 + buffer[pos] - 'a' + 10;
                    else // (buffer[pos] >= 'A' && buffer[pos] <= 'F')
                        s = s * 16 + buffer[pos] - 'A' + 10;
                    pos++;
                }
                token.val = s;
            }
        }
    }

    else if (isalpha(buffer[pos])) // Identifier and keyword
    {
        int len = 0;
        while (isalpha(buffer[pos]) || isdigit(buffer[pos]) || buffer[pos] == '_')
        {
            token.word[len] = buffer[pos];
            len++;
            pos++;
        }
        token.word[len] = '\0'; // end input?

        map<string, TokenID>::iterator it;
        it = keywords.find(token.word); // key word
        if (it != keywords.end())
            // If the iterator do not reach the end of the map, it means the token is a keyword(found in the map).
            // Then token.ID will be set to the index of the keyword in the map.
            token.ID = keywords[token.word];
        else
            token.ID = ID; // a normal Identifier
    }

    else if (buffer[pos] != '\0')
    {
        switch (buffer[pos])
        {
        case ':':
            token.op[0] = buffer[pos];
            if (buffer[pos + 1] = ':')
            {
                pos++;
                token.ID = SCOPE_RESOLUTION; // ::
                token.op[1] = ':';
                token.op[2] = '\0';
            }
            token.op[1] = '\0';
            token.ID = COLON; // :
            break;

        case '.':
            token.op[0] = buffer[pos];
            if (buffer[pos + 1] = '*')
            {
                pos++;
                token.ID = MSELECT_OBJ; // .
                token.op[1] = '*';
                token.op[2] = '\0';
            }
            token.op[1] = '\0';
            token.ID = PTM_OBJ; // .*
            break;

        case '[':
            token.ID = LSBRACKET; // [
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;

        case ']':
            token.ID = RSBRACKET; // ]
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;

        case '(':
            token.ID = LBRACKET; // (
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;

        case ')':
            token.ID = RBRACKET; // )
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;

        case '~':
            token.ID = COMPLEMENT; // ~
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;

        case '!':
            token.op[0] = '!'; // !
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = NOTEQ; // !=
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            token.op[1] = '\0';
            cout << " !  ERROR";
            break;

        case '&':
            token.op[0] = '&';
            if (buffer[pos + 1] == '&')
            {
                pos++;
                token.ID = LAND; // &&
                token.op[1] = '&';
                token.op[2] = '\0';
                break;
            }
            else if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = BAND_ASSIGN; // &=
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            token.op[1] = '\0';
            token.ID = ADDRESSOF; // &
            break;

        case '%':
            token.op[0] = '%';
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = MODULUS_ASSIGN; // %=
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            token.op[1] = '\0';
            token.ID = MODULUS; // %
            break;

        case '^':
            token.op[0] = '^';
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = BEXOR_ASSIGN; // ^=
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            token.op[1] = '\0';
            token.ID = BEXOR; // ^
            break;

        case '|':
            token.op[0] = '|';
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = BINOR_ASSIGN; // |=
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            else if (buffer[pos + 1] == '|')
            {
                pos++;
                token.ID = LOR; // ||
                token.op[1] = '|';
                token.op[2] = '\0';
                break;
            }
            token.op[1] = '\0';
            token.ID = BINOR; // |
            break;

        case ',':
            token.ID = COMMA; // ,
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;

        case '+':
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = ADD_ASSIGN; // +=
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            else if (buffer[pos + 1] == '+')
            {
                pos++;
                token.ID = INCREMENT; // ++
                token.op[1] = '+';
                token.op[2] = '\0';
                break;
            }
            token.ID = ADD; // +
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;

        case '-':
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = SUB_ASSIGN; // -=
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            else if (buffer[pos + 1] == '-')
            {
                pos++;
                token.ID = DECREMENT; // --
                token.op[1] = '-';
                token.op[2] = '\0';
                break;
            }
            else if (buffer[pos + 1] = '>')
            {
                if (buffer[pos + 1] = '*')
                {
                    pos++;
                    token.ID = PTM_PT; // ->*
                    token.op[1] = '>';
                    token.op[2] = '*';
                    token.op[3] = '\0';
                    break;
                }
                pos++;
                token.ID = MSELECT_PT; // ->
                token.op[1] = '-';
                token.op[2] = '\0';
                break;
            }
            token.ID = SUB; // -
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;

        case '*':
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = MUL_ASSIGN; // *=
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            token.ID = MUL; // *
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;

        case '/':
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = MUL_ASSIGN; // /=
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            // 注释的识别需要在此处完成
            token.ID = DIV; // /
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;

        case ';':
            token.ID = SEMICOLON;
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;
        case '>':
            token.op[0] = '>';
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = GTE; // >=
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            else if (buffer[pos + 1] = '>')
            {
                if (buffer[pos + 1] = '=')
                {
                    pos++;
                    token.ID = RSHIFT_ASSIGN; // >>=
                    token.op[1] = '>';
                    token.op[2] = '=';
                    token.op[3] = '\0';
                    break;
                }
                pos++;
                token.ID = RSHIFT; // >>
                token.op[1] = '>';
                token.op[2] = '\0';
                break;
            }
            token.op[1] = '\0';
            token.ID = GT;
            break;
        case '<':
            token.op[0] = '<';
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = LTE;
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            else if (buffer[pos + 1] = '<')
            {
                if (buffer[pos + 1] = '=')
                {
                    pos++;
                    token.ID = LSHIFT_ASSIGN; // <<=
                    token.op[1] = '<';
                    token.op[2] = '=';
                    token.op[3] = '\0';
                    break;
                }
                pos++;
                token.ID = LSHIFT; // <<
                token.op[1] = '<';
                token.op[2] = '\0';
                break;
            }
            token.op[1] = '\0';
            token.ID = LT;
            break;
        case '=':
            token.op[0] = '=';
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = EQ;
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            token.op[1] = '\0';
            token.ID = ASSIGN;
            break;

        default:
            cout << " Error Input at: " << pos + 1;
            exit(1);
        }
        pos++;
    }

    else
        token.ID = ENDINPUT;
}
int main()
{

    init();

    cin.getline(buffer, 255); // 读入一行的算术表达式
    pos = 0;

    GetToken(); // 获取第一单词
    cout << "The result is : " << endl;
    while (token.ID != ENDINPUT)
    {
        if (token.ID < 4)
            cout << token.ID << " Keyword" << endl;
        else if (token.ID == NUMBER)
            cout << token.ID << " " << token.val << endl;
        else if (token.ID == ID)
            cout << token.ID << " " << token.word << endl;
        else
            cout << token.ID << " " << token.op << endl;
        GetToken(); // 获取下一个单词
    }

    system("pause");

    return 0;
}