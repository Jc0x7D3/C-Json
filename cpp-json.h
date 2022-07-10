#pragma once

#include "jsBase.h"

namespace JS {

	//�������;�����ΪJS::Object�����Զ���ָ����в���
	using Object = jsClassRef;
	//5������
	using type = jsType;
	//!!ע��!!: ��Ϊc-bool���Ʒ
	using Bool = jsClassRef::jsBoolean;

	//���г�����������jsBase.cpp��
	
	//js���Ӣ������
	extern const string jsTypeName[];
	//Ĭ�ϵ����鳤�����ֵ
	extern const size_t arrMaxSize;
	//undefined
	extern const Object undefined;
	//null
	extern const Object null;

	//����JSON�ַ����ĺ���������Object
	Object parse(const string&);

}

//����������
JS::Object operator""_js(const char*, size_t);
JS::Object operator""_jn(const char*, size_t);
JS::Object operator""_jn(unsigned long long);