#include "jsException.h"

bool isBlankChar(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

#define THROW_P_ERR(x) throw jsParseError(str, ptr, #x)

class jsStringParser 
{
    string str;
    size_t ptr;

    char cur() const {
        if (ptr >= str.size()) THROW_P_ERR(out of range);
        else return str[ptr];
    }
    char read() {
        if (ptr > str.size()) THROW_P_ERR(out of range);
        else return str[ptr++];
    }

    unsigned short Base16Parse(char c) const;
    string getStr();

    jsClassRef parseNum();
    jsClassRef parseStr();
    jsClassRef parseArr();
    jsClassRef parseDict();

    jsClassRef parseAny();

public:
    jsStringParser(const string&);
    jsClassRef parse();
};


unsigned short jsStringParser::Base16Parse(char c) const {
    if (c >= '0' && c <= '9') return c - '0';
    else if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    else if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    else THROW_P_ERR( not base16 at all !!! );
}

string jsStringParser::getStr() {
    //\b 8  \t 9  \n 10  \f 12  \r 13 \" \\ \u00xx \uxxxx

    if (read() != '\"') THROW_P_ERR(where is the "" ? );

    string str;
    char c;
    while ((c = read()) != '\"') {
        if (c == '\\') {
            switch (read())
            {
            case 'b': str += '\b'; break;
            case 't': str += '\t'; break;
            case 'n': str += '\n'; break;
            case 'f': str += '\f'; break;
            case 'r': str += '\r'; break;
            case '\"': str += '\"'; break;
            case '\\': str += '\\'; break;
            case 'u': {
                char byte;
                byte = Base16Parse(read()) * 16 + Base16Parse(read());
                if (byte != 0) str += byte;
                byte = Base16Parse(read()) * 16 + Base16Parse(read());
                 str += byte;
                break;
            }
            default:
                THROW_P_ERR(? whats this ? );
                break;
            }
        }
        else {
            if (c < 0x20) THROW_P_ERR(not escaped control char detected);
            if (c >= 'A' && c <= 'Z') str += c - 'A' + 'a';
            else str += c;
        }
    }

    return str;
}

jsStringParser::jsStringParser(const string& s):ptr(0) {
    //È¥¿Õ×Ö·û
    int state = 0;

    //test:  
    //"a: \n\t\"a \n\t\\\"b\", a: \n\t\"a \n\t\\\"b\""
    //-->
    //a:"a \n\t"
    for (char c : s) {
        if (state == 0) {
            if (c == '\"') state = 1;
            if (!isBlankChar(c)) str += c;
        }
        else {
            str += c;

            if (c == '\"' && state == 1) state = 0;
            else if (state == 2) state = 1;
            else if (c == '\\') state = 2;
        }
    }
}

jsClassRef jsStringParser::parseNum() {
    size_t used;
    double res = std::stod(str.substr(ptr), &used);
    ptr += used;
    return jsClassRef(res);
}

jsClassRef jsStringParser::parseStr() {
    return jsClassRef(getStr());
}

jsClassRef jsStringParser::parseArr() {
    //[1,2,3] []
    ptr++;  //ignore '['

    auto res = jsClassRef(jsType::Array);
    if (cur() == ']') {
        ptr++;
        return res;
    }
    else {
        res.push(parseAny());
    }

    while (cur() == ',') {
        ptr++;  //ignore ','
        res.push(parseAny());
    }

    if (read() != ']') THROW_P_ERR(where is the ] ? );
    return res;
}

jsClassRef jsStringParser::parseDict() {
    //{"a":1,"b":2,"c":3} {}
    ptr++;  //ignore '{'

    string key;

    auto res = jsClassRef(jsType::Object);
    if (cur() == '}') {
        ptr++;
        return res;
    }
    else {
        key = getStr();
        if (read() != ':') THROW_P_ERR(where is the : ? );
        res.set(key, parseAny());
    }

    while (cur() == ',') {
        ptr++;  //ignore ','

        key = getStr();
        if (read() != ':') THROW_P_ERR(where is the : ? );
        res.set(key, parseAny());
    }
    
    if (read() != '}') THROW_P_ERR(where is the } ? );
    return res;
}


jsClassRef jsStringParser::parseAny() {
    switch (cur())
    {
    case 'f': {
        if (read() == 'f' && read() == 'a' && read() == 'l' && read() == 's' && read() == 'e') return JS::null;
        else THROW_P_ERR(start with 'n' but not null);
    }
    case 't': {
        if (read() == 't' && read() == 'r' && read() == 'u' && read() == 'e') return jsClassRef(true);
        else THROW_P_ERR(start with 'n' but not null);
    }
    case 'n': {
        if (read() == 'n' && read() == 'u' && read() == 'l' && read() == 'l') return JS::null;
        else THROW_P_ERR(start with 'n' but not null);
    }
    case '\"':
        return parseStr();
    case '[':
        return parseArr();
    case '{':
        return parseDict();
    default:
        return parseNum();
    }
}

jsClassRef jsStringParser::parse() {
    jsClassRef res = parseAny();
    if (ptr != str.size()) THROW_P_ERR(too long);
    else return res;
}

jsClassRef JS::parse(const string& str) {
    return jsClassRef(jsStringParser(str).parse());
}



class jsCircularReferenceChecker 
{
    bool& _vis;
public:
    jsCircularReferenceChecker(jsBaseClass* obj);
    ~jsCircularReferenceChecker();
};

jsCircularReferenceChecker::jsCircularReferenceChecker(jsBaseClass* obj)
    :_vis(((bool*)(&(obj->_type)))[3]) 
{
    if (_vis) throw jsCircularReference(obj);
    else _vis = 1;
}
   
jsCircularReferenceChecker::~jsCircularReferenceChecker() {
    _vis = 0;
}

string jsClassRef::stringify() const {
	if (ref == nullptr) return "null";
    jsCircularReferenceChecker guard(ref.get());
    return ref->stringify();
}