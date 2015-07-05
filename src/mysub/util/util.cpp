#ifndef _BASE_UTIL
#define _BASE_UTIL

#include "util.h"




CSLog g_iminfo = CSLog("INFO");
CSLog g_imdebug = CSLog("DEBUG");
CSLog g_imwarn = CSLog("WARN");
CSLog g_imerror = CSLog("ERROR");
CSLog g_imtrace = CSLog("TRACE");

namespace util{
	
using namespace std;

void split(string s, vector<string>& ret)
{
	string element;

	for (auto& it : s){
		if (it == ' ' || it == '\t') {
			if (!element.empty()) {
				ret.push_back(element);
				element.clear();
			}
		} else {
			element += it;
		}
	}
	// put the last one
	if (!element.empty())
		ret.push_back(element);
}

void split(string s, vector<string>& ret, char sep)
{
	string element;

	for (auto& it : s) {
		if (it == sep) {
			if (!element.empty()) {
				ret.push_back(element);
				element.clear();
			}
		} else {
			element += it;
		}
	}
	// put the last one
	if (!element.empty())
		ret.push_back(element);
}

}

#endif