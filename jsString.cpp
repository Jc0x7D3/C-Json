#include "jsString.h"
#include "jsException.h"
#include <regex>


jsClassRef operator""_js(const char* str, size_t len) {
	return jsClassRef(str);
}

string strClarify(const string& data) {
	string res("\"");

	char c;
	for (size_t i = 0; i < data.size(); i++) {
		c = data[i];

		if (c >= 32 && c < 127) {
			if (c == '\\' || c == '"') res += '\\';
			res += c;
		}
		else {
			res += '\\';

			switch (c)
			{
			case '\b': res += 'b'; break;
			case '\t': res += 't'; break;
			case '\n': res += 'n'; break;
			case '\f': res += 'f'; break;
			case '\r': res += 'r'; break;
			default: {
				static const char HEX[] = "0123456789ABCDEF";
				static const char PRE[] = "u00";

				res += PRE;
				res += HEX[((unsigned char)c) / 16];
				res += HEX[((unsigned char)c) % 16];

				break;
			}
			}

		}
	}

	res += '\"';
	return res;
}

string jsString::stringify() const {
	return strClarify(data);
}

jsClassRef::operator string() const {
	jsTypeDetection::check(this, jsType::String);
	return ((jsString*)ref.get())->data;
}

string jsClassRef::toString() const {
	jsTypeDetection::check(this, jsType::String);
	return ((jsString*)ref.get())->data;
}

jsClassRef::jsClassRef(const string& x){
	ref = (jsBaseRef)(new jsString(x.c_str()));
}