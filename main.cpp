#include "cpp-json.h"
#include <iostream>

using namespace std;

void example() {
	try {
		//请先结合注释阅读 jsBase.h 中 jsClassRef 的公有接口

		//using namespace JS;

		//构造——字面量
		JS::Object obj1 = 						//字典
			{
				{"num1", "-5.190387e-5"_jn},			//数字
				{"num2", 918350_jn},					//数字（整数）
				{"str", "the\\   string\nhello\tworld\03\04"_js},//字符串
				{"list", 
					JS::Object(
						{1_jn, JS::undefined, JS::null}	//特殊值
					)									//数组
				},
			};

		//转JSON
		cout << obj1.stringify() << endl;

		//构造——手动
		JS::Object obj2(JS::type::Array);				//空数组，其他空对象类似
		obj2.set(1, "str at 1"_js);						//1设为。注意：此时0处会留空(undefined)
		obj2.set(2, "str at 2"_js);
		obj2.set(2, JS::undefined);						//删除2上的对象

		auto obj3 = JS::Object(JS::type::Number);
		//obj3.set("ERR");								//会报错
		obj3 = "aca";
		obj3 = JS::Bool::True;
		obj3 = 2.5;
		obj2.push(obj3);								//push到obj2的2位置上

		obj1.set("another list", obj2);					//常规类型都有set方法重载，array多一个push
		obj1.set("another another list", obj2);			//可以多次引用

		//转JSON
		cout << obj1.stringify() << endl;

		//读取
		//cout << obj1["list"].toString() << endl;		//会报错
		cout << obj1["str"].toString() << endl;			//手动转换
		cout << double(obj1["num1"]) << endl;			
		obj1["another list"].forEach([](size_t i, JS::Object obj) {
			cout << i << " " << obj.stringify() << endl;
			});											//array，dict都能forEach，注意回调函数参数类型

		//解析
		auto obj4 = JS::parse(obj1.stringify());		//放字符串进去
		cout << obj4.stringify() << endl;

		//警告：
		obj2.push(obj1);								//循环引用指定有你好果汁吃
		cout << obj1.stringify() << endl;

	}
	catch (std::exception& err) {
		cerr << err.what() << endl;
	}
}


int main() {
	example();

	//for (;;) {
	//	try {
	//		char s[100]{};
	//		cin.getline(s, 100);
	//		string str(s);

	//		auto obj = JS::parse(str);		//放字符串进去
	//		cout << obj.stringify() << endl;
	//	}
	//	catch (std::exception& err) {
	//		cerr << err.what() << endl;
	//	}
	//}


	system("pause");
	return 0;
}

//  {"a":{"a":{"a":[1,2,[],{}]}}}
//  {  " a "  : {		"  a  \t" :	{"a \"\"\"":	[1, 2 ,[  ],{	} ] } }		}   
//["[]",[{},{"a\"\u001f": ["\t\t\\u0030t\t\\"]  }, null]  ,{"oqev":-2.294E-5}]