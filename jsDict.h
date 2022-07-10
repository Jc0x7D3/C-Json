#pragma once
#include "cpp-json.h"
#include <map>

class jsDict 
    : public jsBaseClass
{
public:
    jsDict() :jsBaseClass(jsType::Object) {}

    std::map<string, jsClassRef> index;

    void forEach(std::function<void(string, jsClassRef)> func) const;
    const jsClassRef& operator[](const string&) const;
    void set(const string&, jsClassRef);

    virtual string stringify() const final;
    virtual size_t length() const final { return index.size(); }
    virtual ~jsDict() = default;
};
