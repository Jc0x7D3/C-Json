#include "jsBool.h"
#include "jsException.h"

string jsBool::stringify() const {
	return data ? "true" : "false";
}

jsClassRef::jsClassRef(jsBoolean x) {
	ref = (jsBaseRef)(new jsBool(x == jsBoolean::True));
}

//jsClassRef& jsClassRef::operator=(bool x) const {
//	jsTypeDetection::check(this, jsType::Boolean);
//	((jsBool*)ref.get())->data = x;
//}

bool jsClassRef::toBool() const {
	jsTypeDetection::check(this, jsType::Boolean);
	return ((jsBool*)ref.get())->data;

}