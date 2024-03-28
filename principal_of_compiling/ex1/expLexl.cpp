#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include <fstream>
#include <vector>
#include <string.h>
#include <iomanip>
using namespace std;
using std::right;
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
    INCLUDE,
    IOSTREAM,
    STD,
    COUT,
    CIN,
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
    ID,       // 标识符identifier
    COMMENTS, // 注释
    STRING,   // 字符串
    CHAR_TYPE,
    ENDINPUT
};

map<string, TokenID> keywords;

struct TokenStru
{
    TokenID ID;
    double val;      // 数字的值存放于此
    string numStr;   // 数字的字符串形式存放于此
    char op[4];      // 符号、字符存放于此
    char word[20];   // 关键字、标识符存放于此
    string comments; // 注释、字符串存放于此
};

TokenStru token;
char buffer[4092];
int pos = 0;

void init()
{
    keywords["alignas"] = ALIGNAS;
    keywords["alignof"] = ALIGNOF;
    keywords["andb"] = ANDB;
    keywords["and_eqb"] = AND_EQB;
    keywords["asma"] = ASMA;
    keywords["auto"] = AUTO;
    keywords["bitandb"] = BITANDB;
    keywords["bitorb"] = BITORB;
    keywords["bool"] = BOOL;
    keywords["break"] = BREAK;
    keywords["case"] = CASE;
    keywords["catch"] = CATCH;
    keywords["char"] = CHAR;
    keywords["char8_t"] = CHAR8_T;
    keywords["char16_t"] = CHAR16_T;
    keywords["char32_t"] = CHAR32_T;
    keywords["class"] = CLASS;
    keywords["complb"] = COMPLB;
    keywords["conceptc"] = CONCEPTC;
    keywords["const"] = CONST;
    keywords["const_cast"] = CONST_CAST;
    keywords["constevalc"] = CONSTEVALC;
    keywords["constexpr"] = CONSTEXPR;
    keywords["constinitc"] = CONSTINITC;
    keywords["continue"] = CONTINUE;
    keywords["co_awaitc"] = CO_AWAITC;
    keywords["co_returnc"] = CO_RETURNC;
    keywords["co_yieldc"] = CO_YIELDC;
    keywords["decltype"] = DECLTYPE;
    keywords["default"] = DEFAULT;
    keywords["delete"] = DELETE;
    keywords["do"] = DO;
    keywords["double"] = DOUBLE;
    keywords["dynamic_cast"] = DYNAMIC_CAST;
    keywords["else"] = ELSE;
    keywords["enum"] = ENUM;
    keywords["explicit"] = EXPLICIT;
    keywords["exportc"] = EXPORTC;
    keywords["extern"] = EXTERN;
    keywords["false"] = FALSE;
    keywords["float"] = FLOAT;
    keywords["for"] = FOR;
    keywords["friend"] = FRIEND;
    keywords["goto"] = GOTO;
    keywords["if"] = IF;
    keywords["inline"] = INLINE;
    keywords["int"] = INT;
    keywords["long"] = LONG;
    keywords["mutable"] = MUTABLE;
    keywords["namespace"] = NAMESPACE;
    keywords["new"] = NEW;
    keywords["noexcept"] = NOEXCEPT;
    keywords["notb"] = NOTB;
    keywords["not_eqb"] = NOT_EQB;
    keywords["nullptr"] = NULLPTR;
    keywords["operator"] = OPERATOR;
    keywords["orb"] = ORB;
    keywords["or_eqb"] = OR_EQB;
    keywords["private"] = PRIVATE;
    keywords["protected"] = PROTECTED;
    keywords["public"] = PUBLIC;
    keywords["register"] = REGISTER;
    keywords["reinterpret_cast"] = REINTERPRET_CAST;
    keywords["requirese"] = REQUIRESC;
    keywords["return"] = RETURN;
    keywords["short"] = SHORT;
    keywords["signed"] = SIGNED;
    keywords["sizeof"] = SIZEOF;
    keywords["static"] = STATIC;
    keywords["static_assert"] = STATIC_ASSERT;
    keywords["static_cast"] = STATIC_CAST;
    keywords["struct"] = STRUCT;
    keywords["switch"] = SWITCH;
    keywords["template"] = TEMPLATE;
    keywords["this"] = THIS;
    keywords["thread_local"] = THREAD_LOCAL;
    keywords["throw"] = THROW;
    keywords["true"] = TRUE;
    keywords["try"] = TRY;
    keywords["typedef"] = TYPEDEF;
    keywords["typeid"] = TYPEID;
    keywords["typename"] = TYPENAME;
    keywords["union"] = UNION;
    keywords["unsigned"] = UNSIGNED;
    keywords["using"] = USING;
    keywords["virtual"] = VIRTUAL;
    keywords["void"] = VOID;
    keywords["volatile"] = VOLATILE;
    keywords["wchar_t"] = WCHAR_T;
    keywords["while"] = WHILE;
    keywords["xorb"] = XORB;
    keywords["xor_eqb"] = XOR_EQB;
    keywords["include"] = INCLUDE;
    keywords["iostream"] = IOSTREAM;
    keywords["std"] = STD;
    keywords["cout"] = COUT;
    keywords["cin"] = CIN;
}

void GetToken()
{
    int s;

    while (buffer[pos] == 32 || buffer[pos] == 9) // spacebar or tab
        pos++;

    if ((buffer[pos] >= '0') && (buffer[pos] <= '9')) // digit
    {
        token.numStr = ""; // 清空字符串
        s = 0;
        while (buffer[pos] >= '0' && buffer[pos] <= '9')
        {
            s = s * 10 + buffer[pos] - '0';
            token.numStr += buffer[pos]; // 将当前字符添加到数字字符串中
            pos++;
            if (buffer[pos] == 39) // separator '
            {
                pos++;
            }
        }
        token.ID = NUMBER; // 设置 token ID 为数字类型
        token.val = s;

        if (buffer[pos] == '.') // float
        {
            token.numStr += '.';
            int len = 0;
            s = 0;
            pos++;
            while (buffer[pos] >= '0' && buffer[pos] <= '9')
            {
                s = s * 10 + buffer[pos] - '0';
                len++;
                token.numStr += buffer[pos]; // 将当前字符添加到数字字符串中
                pos++;
            }
            token.val = token.val + s / pow(10, len);
        }

        if (buffer[pos] == 'e' || buffer[pos] == 'E') // exponent
        {
            token.numStr += 'e';
            bool ispositive = true;
            s = 0;
            pos++;
            if (buffer[pos] == '-')
            {
                ispositive = false;
                pos++;
                while (buffer[pos] >= '0' && buffer[pos] <= '9')
                {
                    s = s * 10 + buffer[pos] - '0';
                    token.numStr += buffer[pos]; // 将当前字符添加到数字字符串中
                    pos++;
                }
            }
            else if (buffer[pos] == '+')
            {
                token.numStr += '+';
                ispositive = true;
                pos++;
                while (buffer[pos] >= '0' && buffer[pos] <= '9')
                {
                    s = s * 10 + buffer[pos] - '0';
                    token.numStr += buffer[pos]; // 将当前字符添加到数字字符串中
                    pos++;
                }
            }
            else if (buffer[pos] >= '0' && buffer[pos] <= '9')
            {
                ispositive = true;
                while (buffer[pos] >= '0' && buffer[pos] <= '9')
                {
                    s = s * 10 + buffer[pos] - '0';
                    token.numStr += buffer[pos]; // 将当前字符添加到数字字符串中
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
                token.numStr += buffer[pos]; // 将当前字符添加到数字字符串中
                pos++;
            }
            token.val = s;

            if (buffer[pos] == 'b' || buffer[pos] == 'B') // binary
            {
                token.numStr += 'b';
                pos++; // 跳过 'b' 或 'B'
                s = 0;
                while (buffer[pos] == '0' || buffer[pos] == '1')
                {
                    s = s * 2 + buffer[pos] - '0'; // 将二进制转换为十进制
                    token.numStr += buffer[pos];   // 将当前字符添加到数字字符串中
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
                    token.numStr += buffer[pos]; // 将当前字符添加到数字字符串中
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
            if (buffer[pos + 1] == ':')
            {
                pos++;
                token.ID = SCOPE_RESOLUTION; // ::
                token.op[1] = ':';
                token.op[2] = '\0';
                break;
            }
            else
            {
                token.op[1] = '\0';
                token.ID = COLON; // :
                break;
            }

        case '.':
            token.op[0] = buffer[pos];
            if (buffer[pos + 1] == '*')
            {
                pos++;
                token.ID = MSELECT_OBJ; // .
                token.op[1] = '*';
                token.op[2] = '\0';
                break;
            }
            else
            {
                token.op[1] = '\0';
                token.ID = PTM_OBJ; // .*
                break;
            }

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

        case '{':
            token.ID = LCBRACKET; // {
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;

        case '}':
            token.ID = RCBRACKET; // }
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
            else
            {
                token.op[1] = '\0';
                cout << " !  ERROR";
                break;
            }

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
            else
            {
                token.op[1] = '\0';
                token.ID = ADDRESSOF; // &
                break;
            }

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
            else
            {
                token.op[1] = '\0';
                token.ID = MODULUS; // %
                break;
            }

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
            else
            {
                token.op[1] = '\0';
                token.ID = BEXOR; // ^
                break;
            }

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
            else
            {
                token.op[1] = '\0';
                token.ID = BINOR; // |
                break;
            }

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
            else
            {
                token.ID = ADD; // +
                token.op[0] = buffer[pos];
                token.op[1] = '\0';
                break;
            }

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
            else if (buffer[pos + 1] == '>')
            {
                if (buffer[pos + 1] == '*')
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
            else
            {
                token.ID = SUB; // -
                token.op[0] = buffer[pos];
                token.op[1] = '\0';
                break;
            }

        case '*':
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = MUL_ASSIGN; // *=
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            else
            {
                token.ID = MUL; // *
                token.op[0] = buffer[pos];
                token.op[1] = '\0';
                break;
            }

        case '/':
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = MUL_ASSIGN; // /=
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            else if (buffer[pos + 1] == '/') // 单行注释
            {
                // pos += 2; // 不再跳过 '//'，使得输出可以包括注释符号
                int commentStart = pos;
                // 寻找注释结束符（换行符）'\n' 或文件结束符 '\0'
                while (!(buffer[pos] == '\r' || buffer[pos] == '\0'))
                {
                    pos++;
                    cout << buffer[pos] << endl;
                }

                // 将注释内容存储到 token 中
                token.ID = COMMENTS;
                token.comments = std::string(buffer + commentStart, pos - commentStart);
                // 如果下一行还是注释，继续跳过
                if (buffer[pos] == '\n')
                {
                    pos++;
                    cout << "rr" << endl;
                }
                break;
            }
            else if (buffer[pos + 1] == '*') // 多行注释
            {
                // pos += 2; // 跳过 '/*'
                int commentStart = pos;
                // 寻找注释结束标记 '*/'
                while (!(buffer[pos] == '*' && buffer[pos + 1] == '/'))
                {
                    if (buffer[pos] == '\0')
                    {
                        break;
                    }
                    pos++;
                }
                // 将注释内容存储到 token 中
                token.ID = COMMENTS;
                token.comments = std::string(buffer + commentStart, pos - commentStart);
                token.comments.append("*/");
                pos += 2; // 跳过 '*/'
                break;
            }
            else
            {
                token.ID = DIV; // /
                token.op[0] = buffer[pos];
                token.op[1] = '\0';
                break;
            }

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
            else if (buffer[pos + 1] == '>')
            {
                if (buffer[pos + 1] == '=')
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
            else
            {
                token.op[1] = '\0';
                token.ID = GT;
                break;
            }

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
            else if (buffer[pos + 1] == '<')
            {
                if (buffer[pos + 1] == '=')
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
            else
            {
                token.op[1] = '\0';
                token.ID = LT;
                break;
            }

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
            else
            {
                token.op[1] = '\0';
                token.ID = ASSIGN;
                break;
            }

        case 39:
            if (buffer[pos + 2] == 39)
            {
                token.ID = CHAR_TYPE; // char
                token.op[0] = 39;
                token.op[1] = buffer[pos + 1];
                token.op[2] = 39;
                token.op[3] = '\0';
                pos += 2;
            }
            break;

        case 34:
        {
            pos++;
            // create a new variable in a case, need to reduce the scope.
            int stringStart = pos;
            while (!(buffer[pos] == 34))
            {
                if (buffer[pos] == '\0')
                    break;
                pos++;
            }
            token.ID = STRING;
            token.comments = string(buffer + stringStart, pos - stringStart);
            break;
        }

        case '#':
            token.ID = PSIGN;
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;

        case '?':
            token.ID = QMARK;
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
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
    string fileName = "..//test//test.cpp";
    ifstream file;
    file.open(fileName, ios::in);

    if (!file.is_open())
    {
        cerr << "error" << endl;
        return 1;
    }

    vector<string> lines; // 用于存储读取到的每一行内容

    string line; // 用于临时存储每一行的内容
    while (getline(file, line))
    {                          // 逐行读取文件内容
        lines.push_back(line); // 将读取到的行存储到vector中
    }

    file.close(); // 关闭文件流

    // 将vector中的内容存储到名为buffer的char数组中
    int index = 0;
    for (const string &str : lines)
    {
        strcpy(buffer + index, str.c_str()); // 将string转换为char数组并拷贝到目标数组
        index += str.size();                 // 更新目标数组的索引位置
    }

    // for (long long int i = 0; i < sizeof(buffer) / sizeof(char); i++)
    // {
    //     cout << buffer[i];
    // }

    init();

    // cin.getline(buffer, 255); // 读入一行的算术表达式
    pos = 0;

    GetToken(); // 获取第一单词
    cout << "The result is : " << endl;
    while (token.ID != ENDINPUT)
    {
        if (token.ID < 97)
        {
            // cout << token.ID << " "
            //      << "Keyword" << endl;
            int i = 0;
            while (token.word[i] != '\0')
            {
                cout << token.word[i];
                i++;
            }
            cout << right << setw(16) << "keyword" << endl;
        }

        else if (token.ID == NUMBER)
        {
            cout << token.numStr;
            cout << right << setw(16) << "number" << endl;
        }
        else if (token.ID == ID)
        {
            cout << token.word;
            cout << right << setw(16) << "identifier" << endl;
        }

        else if (token.ID == COMMENTS)
        {
            cout << token.comments;
            cout << right << setw(16) << "comments" << endl;
        }
        else if (token.ID == CHAR_TYPE)
        {
            cout << token.op[1];
            cout << right << setw(16) << "char" << endl;
        }
        else if (token.ID == STRING)
        {
            cout << token.comments;
            cout << right << setw(16) << "string" << endl;
        }
        else
        {
            cout << token.op;
            cout << right << setw(16) << "operator" << endl;
        }
        GetToken(); // 获取下一个单词
    }

    system("pause");

    return 0;
}