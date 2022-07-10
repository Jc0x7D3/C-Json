#pragma once
#include <exception>
#include <string>

#include "cpp-json.h"

class jsCircularReference
	:public std::exception
{
public:
	const jsBaseClass* address;
	jsCircularReference(const jsBaseClass* Add) :address(Add) {}

	virtual const char* what() const final{
		return "Circular reference detected when stringifying jsObject.";
	}
};

class jsNullPointerDereference
	:public std::exception
{
public:
	const jsClassRef* address;
	jsNullPointerDereference(const jsClassRef* Add) :address(Add) {}

	virtual const char* what() const final {
		return "Dereference of Null Pointer.";
	}

	static void check(const jsClassRef* obj) {
		if (obj->isNull()) throw jsNullPointerDereference(obj);
	}
};

class jsTypeDetection
	:public std::exception
{
public:
	const jsClassRef* address;
	jsType expect;
	jsTypeDetection(const jsClassRef* Add, jsType expectType) :address(Add), expect(expectType){}

	virtual const char* what() const final {
		return "Type incompatible.";
	}

	static void check(const jsClassRef* obj, jsType expectType) {
		jsNullPointerDereference::check(obj);
		if (obj->type() != expectType) throw jsTypeDetection(obj, expectType);
	}
};

class jsArrayTooLong
	:public std::exception
{
public:
	virtual const char* what() const final {
		return "Array Too Long (>0xFFF0).";
	}
};

class jsParseError
	:public std::exception
{
public:
	string base;
	size_t pos;
	string info;

	jsParseError(string _base, size_t _pos, const char* _info) 
		:base(_base), pos(_pos), info(_info) {}

	virtual const char* what() const final {
		return info.c_str();
	}

};