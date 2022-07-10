#include "jsNumber.h"
#include "jsException.h"

#define MAX_NUMBUR_LENGTH 30

string jsNumber::stringify() const {
	if (isfinite(data)) {
		char buffer[MAX_NUMBUR_LENGTH + 1]{};
		sprintf_s(buffer, MAX_NUMBUR_LENGTH, "%g", data);
		return buffer;
	}
	else return "null";
}

jsClassRef operator""_jn(const char* str, size_t size) {
	return jsClassRef(atof(str));
}

jsClassRef operator""_jn(unsigned long long x) {
#pragma warning(disable: 4244)
	return jsClassRef((double)x);
}

jsClassRef::operator double() const {
	jsTypeDetection::check(this, jsType::Number);
	return ((jsNumber*)ref.get())->data;
}

double jsClassRef::toDouble() const {
	jsTypeDetection::check(this, jsType::Number);
	return ((jsNumber*)ref.get())->data;
}

//jsClassRef& jsClassRef::operator=(double x) const {
//	jsTypeDetection::check(this, jsType::Number);
//	((jsNumber*)ref.get())->data = x;
//}

jsClassRef::jsClassRef(double x) {
	ref = (jsBaseRef)(new jsNumber(x));
}