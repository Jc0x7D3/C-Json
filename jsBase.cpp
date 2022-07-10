#include "cpp-json.h"

/*******
* 常数 *
*******/

const string jsTypeName[] = {
	"Undefined",
	"Boolean",
	"Number",
	"String",
	"Array",
	"Dictionary",
};

const jsClassRef JS::undefined(jsType::Undefined);
const jsClassRef JS::null;


/*************
* 基础类方法 *
*************/

#include "jsException.h"
#include "jsBool.h"
#include "jsNumber.h"
#include "jsString.h"
#include "jsArray.h"
#include "jsDict.h"

size_t jsClassRef::length() const {
	if (isNull()) return 0;
	else return ref->length();
}
jsType jsClassRef::type() const {
	if (isNull()) return jsType::Undefined;
	else return ref->type();
}
string jsClassRef::typeName() const {
	return jsTypeName[(int)type()];
}
jsClassRef::operator bool() const {
	if (ref == nullptr || type() == jsType::Undefined) return false;

	switch (type())
	{
	case jsType::Boolean:
		return toBool();
	case jsType::Number:
	{
		double num = toDouble();
		return num && !isnan(num);
	}
	case jsType::String:
		return (bool)length();
	default:
		return true;
	}
}

jsClassRef::jsClassRef(jsType T) {
	switch (T)
	{
	case jsType::Boolean:
		ref = jsBaseRef(new jsBool());
		break;
	case jsType::Number:
		ref = jsBaseRef(new jsNumber());
		break;
	case jsType::String:
		ref = jsBaseRef(new jsString());
		break;
	case jsType::Array:
		ref = jsBaseRef(new jsArray());
		break;
	case jsType::Object:
		ref = jsBaseRef(new jsDict());
		break;
	default:
		ref = jsBaseRef(new jsBaseClass());
		break;
	}
}