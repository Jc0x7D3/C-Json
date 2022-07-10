#pragma once
#include "cpp-json.h"

class jsString
    : public jsBaseClass
{
public:
    string data;

    jsString(const char* str = "") :jsBaseClass(jsType::String), data(str) {}
    virtual string stringify() const final;
    virtual size_t length() const final { return data.length(); }
    virtual ~jsString() = default;
};

