#include "jsArray.h"
#include "jsException.h"

//为了防止 arr[0xFFFFFFFF] = 1jn 这种可以将内存快速耗尽的操作带来的危害
//特将数组长度限定在0xFFF0（65520）以下，根据具体需求可以修改
const size_t arrMaxSize = 0xFFF0;


void jsArray::forEach(std::function<void(size_t, jsClassRef)> func) const {
    for (size_t i = 0; i < index.size(); i++) func(i, index[i]);
}

const jsClassRef& jsArray::operator[](size_t i) const {
    if (i >= index.size()) return JS::undefined;
    else return index[i];
}

void jsArray::set(size_t i, jsClassRef obj) {

    if (obj.isUndefined()) {
        if (i < index.size()) {
            index[i] = JS::undefined;

            if (i == index.size() - 1) {
                for (; index[i].isUndefined(); i--);
                index.resize(i + 1);
            }
        }
    }
    else {
        if (i >= 0xFFF0) throw jsArrayTooLong();

        if (i < index.size()) {
            index[i] = obj;
        }
        else {
            for (size_t count = i - index.size(); count; count--)
                index.push_back(JS::undefined);
            index.push_back(obj);
        }
    }
}

void jsArray::push(jsClassRef obj) {
    if (!obj.isUndefined()) {
        if (index.size() >= 0xFFF0) throw jsArrayTooLong();
        index.push_back(obj);
    }
}

string jsArray::stringify() const {
    string str;
    forEach(
        [&](size_t i, jsClassRef val) {
            str += ',';
            str += val.stringify();
        }
    );

    if (str.size()) str[0] = '[';
    else str += '[';
    str += ']';

    return str;
}



jsClassRef::jsClassRef(const std::initializer_list<jsClassRef>& list)
    :jsClassRef(jsType::Array)
{
    size_t i = 0;
    for (auto& x : list) {
        set(i, x);
        i++;
    }
}


void jsClassRef::set(size_t i, jsClassRef obj) const {
    jsTypeDetection::check(this, jsType::Array);
    ((jsArray*)ref.get())->set(i, obj);
}

void jsClassRef::push(jsClassRef obj) const {
    jsTypeDetection::check(this, jsType::Array);
    ((jsArray*)ref.get())->push(obj);
}

void jsClassRef::forEach(std::function<void(size_t, jsClassRef)> func) const {
    jsTypeDetection::check(this, jsType::Array);
    ((jsArray*)ref.get())->forEach(func);
}

const jsClassRef& jsClassRef::operator[](size_t i) const {
    jsTypeDetection::check(this, jsType::Array);
    return (*(jsArray*)ref.get())[i];
}