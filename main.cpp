#include "cpp-json.h"
#include <iostream>

using namespace std;

void example() {
	try {
		//���Ƚ��ע���Ķ� jsBase.h �� jsClassRef �Ĺ��нӿ�

		//using namespace JS;

		//���졪��������
		JS::Object obj1 = 						//�ֵ�
			{
				{"num1", "-5.190387e-5"_jn},			//����
				{"num2", 918350_jn},					//���֣�������
				{"str", "the\\   string\nhello\tworld\03\04"_js},//�ַ���
				{"list", 
					JS::Object(
						{1_jn, JS::undefined, JS::null}	//����ֵ
					)									//����
				},
			};

		//תJSON
		cout << obj1.stringify() << endl;

		//���졪���ֶ�
		JS::Object obj2(JS::type::Array);				//�����飬�����ն�������
		obj2.set(1, "str at 1"_js);						//1��Ϊ��ע�⣺��ʱ0��������(undefined)
		obj2.set(2, "str at 2"_js);
		obj2.set(2, JS::undefined);						//ɾ��2�ϵĶ���

		auto obj3 = JS::Object(JS::type::Number);
		//obj3.set("ERR");								//�ᱨ��
		obj3 = "aca";
		obj3 = JS::Bool::True;
		obj3 = 2.5;
		obj2.push(obj3);								//push��obj2��2λ����

		obj1.set("another list", obj2);					//�������Ͷ���set�������أ�array��һ��push
		obj1.set("another another list", obj2);			//���Զ������

		//תJSON
		cout << obj1.stringify() << endl;

		//��ȡ
		//cout << obj1["list"].toString() << endl;		//�ᱨ��
		cout << obj1["str"].toString() << endl;			//�ֶ�ת��
		cout << double(obj1["num1"]) << endl;			
		obj1["another list"].forEach([](size_t i, JS::Object obj) {
			cout << i << " " << obj.stringify() << endl;
			});											//array��dict����forEach��ע��ص�������������

		//����
		auto obj4 = JS::parse(obj1.stringify());		//���ַ�����ȥ
		cout << obj4.stringify() << endl;

		//���棺
		obj2.push(obj1);								//ѭ������ָ������ù�֭��
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

	//		auto obj = JS::parse(str);		//���ַ�����ȥ
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