#pragma once
#include "cpp-json.h"

class jsNumber 
    : public jsBaseClass
{
public:
    double data;

    jsNumber(double x = 0.0) :jsBaseClass(jsType::Number), data(x) {}
    virtual string stringify() const final;
    virtual size_t length() const final { return 1; }
    virtual ~jsNumber() = default;
};
