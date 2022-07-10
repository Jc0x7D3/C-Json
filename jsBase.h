#pragma once

#include <string>
#include <memory>
#include <initializer_list>
#include <functional>

using std::string;

enum class jsType {
	Undefined	= 0,
	Boolean		= 1,
	Number		= 2,
	String		= 3,
	Array		= 4,
	Object		= 5,
};

class jsCircularReferenceChecker;
class jsClassRef;

class jsBaseClass {
	friend class jsCircularReferenceChecker;

	jsType _type;
public:
	inline jsType type() const { return _type; }

	jsBaseClass(jsType Type = jsType::Undefined) :_type(Type) {}

	virtual string stringify() const { return "null"; }
	virtual size_t length() const { return 0; }
	virtual ~jsBaseClass() = default;
};


class jsClassRef
{
	using jsBaseRef = std::shared_ptr<jsBaseClass>;
	jsBaseRef ref;
	jsClassRef(jsBaseClass* ptr) :ref(ptr) {}

	using key_val_pair = std::pair<string, jsClassRef>;

public:

	enum class jsBoolean { False = 0, True = 1 };

	/*******
	* 构造 *
	*******/
	
	//null
	jsClassRef() :ref(nullptr) {}
	//空类
	jsClassRef(jsType T);

	//	Boolean
	//!!特别注意!!: 此处不能用c-bool构造!!!!!!
	jsClassRef(jsBoolean);
	//jsClassRef& operator=(bool x);

	//	Number
	jsClassRef(double);
	//jsClassRef& operator=(double x);

	//	String
	jsClassRef(const string&);
	jsClassRef& operator=(const string& x) { return (*this) = jsClassRef(x); }
	//jsClassRef(const char* str) :jsClassRef(string(str)) {}

	//	Array
	jsClassRef(const std::initializer_list<jsClassRef>&);
	//	Array: 用undefined赋值即为删除。
	//		注意：索引有最大值
	void set(size_t, jsClassRef) const;
	//	Array: push undefined无效，占位可以push null
	void push(jsClassRef) const;

	//	Object
	jsClassRef(const std::initializer_list<key_val_pair>&);
	//	Object: 用undefined赋值即为删除
	void set(const string&, jsClassRef) const;


	/*******
	* 读取 *
	*******/

	size_t length() const;
	jsType type() const;
	string typeName() const;
	explicit operator bool() const;

	bool isNull() const { return ref == nullptr; }
	bool isUndefined() const { return ref != nullptr && type() == jsType::Undefined; }

	//	Boolean
	bool toBool() const;

	//	Number
	explicit operator double() const;
	double toDouble() const;

	//	String
	explicit operator string() const;
	string toString() const;

	//	Array
	void forEach(std::function<void(size_t, jsClassRef)> func) const;
		//用undefined赋值即为删除，找不到会返回undefined
	const jsClassRef& operator[](size_t) const;		

	//	Object
	void forEach(std::function<void(string, jsClassRef)> func) const;
		//只读，找不到会返回undefined
	const jsClassRef& operator[](const string&) const;
	const jsClassRef& operator[](const char* str) const { return operator[](string(str)); }

	/***********
	* 转字符串 *
	************/

	string stringify() const;
};

