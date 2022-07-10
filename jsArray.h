#pragma once
#include "cpp-json.h"
#include <vector>

class jsArray :
    public jsBaseClass
{
public:
    jsArray() :jsBaseClass(jsType::Array) {}

    std::vector<jsClassRef> index;

    void forEach(std::function<void(size_t, jsClassRef)> func) const;
    const jsClassRef& operator[](size_t) const;
    void set(size_t, jsClassRef);
    void push(jsClassRef);

    virtual string stringify() const final;
    virtual size_t length() const final { return index.size(); }
    virtual ~jsArray() = default;
};

