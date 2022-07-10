#pragma once
#include "cpp-json.h"

class jsBool 
    : public jsBaseClass
{
public:
    bool data;

    jsBool(bool x = 0) :jsBaseClass(jsType::Boolean), data(x) {}
    virtual string stringify() const final;
    virtual size_t length() const final { return 1; }
    virtual ~jsBool() = default;
};
