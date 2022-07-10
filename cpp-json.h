#pragma once

#include "jsBase.h"

namespace JS {

	//所有类型均定义为JS::Object，都以对象指针进行操作
	using Object = jsClassRef;
	//5种类型
	using type = jsType;
	//!!注意!!: 此为c-bool替代品
	using Bool = jsClassRef::jsBoolean;

	//下列常数均定义在jsBase.cpp中
	
	//js类的英文名称
	extern const string jsTypeName[];
	//默认的数组长度最大值
	extern const size_t arrMaxSize;
	//undefined
	extern const Object undefined;
	//null
	extern const Object null;

	//解析JSON字符串的函数，返回Object
	Object parse(const string&);

}

//字面量构造
JS::Object operator""_js(const char*, size_t);
JS::Object operator""_jn(const char*, size_t);
JS::Object operator""_jn(unsigned long long);