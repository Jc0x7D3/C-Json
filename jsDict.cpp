#include "jsDict.h"
#include "jsException.h"


void jsDict::forEach(std::function<void(string, jsClassRef)> func) const {
    for (auto &ptr : index) {
        func(ptr.first, ptr.second);
    }
}

const jsClassRef& jsDict::operator[](const string& key) const {
    auto ptr = index.find(key);
    if (ptr == index.end()) return JS::undefined;
    else return ptr->second;
}

void jsDict::set(const string& key, jsClassRef obj) {
    if (obj.isUndefined()) {
        auto ptr = index.find(key);
        if (ptr != index.end()) {
            index.erase(ptr);
        }
    }
    else {
        index[key] = obj;
    }
}

extern string strClarify(const string&);
string jsDict::stringify() const {
    string str;
    forEach(
        [&](string key, jsClassRef val) {
            str += ',', str += strClarify(key), str += ':';
            str += val.stringify();
        }
    );

    if (str.size()) str[0] = '{';
    else str += '{';
    str += '}';

    return str;
}



jsClassRef::jsClassRef(const std::initializer_list<key_val_pair>& list)
    :jsClassRef(jsType::Object)
{
    for (auto& x : list) {
        set(x.first, x.second);
    }
}

void jsClassRef::forEach(std::function<void(string, jsClassRef)> func) const {
    jsTypeDetection::check(this, jsType::Object);
    ((jsDict*)ref.get())->forEach(func);
}

const jsClassRef& jsClassRef::operator[](const string& key) const {
    jsTypeDetection::check(this, jsType::Object);
    return (*(jsDict*)ref.get())[key];
}

void jsClassRef::set(const string& key, jsClassRef obj) const {
    jsTypeDetection::check(this, jsType::Object);
    return ((jsDict*)ref.get())->set(key, obj);
}