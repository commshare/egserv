#ifndef _BASE_UTIL
#define _BASE_UTIL

#include "util.h"

CSLog g_iminfo = CSLog("INFO");
CSLog g_imdebug = CSLog("DEBUG");
CSLog g_imwarn = CSLog("WARN");
CSLog g_imerror = CSLog("ERROR");
CSLog g_imtrace = CSLog("TRACE");


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

uint64_t get_tick_count()
{
#ifdef _WIN32
	LARGE_INTEGER liCounter; 
	LARGE_INTEGER liCurrent;

	if (!QueryPerformanceFrequency(&liCounter))
		return GetTickCount();

	QueryPerformanceCounter(&liCurrent);
	return (uint64_t)(liCurrent.QuadPart * 1000 / liCounter.QuadPart);
#else
	struct timeval tval;
	uint64_t ret_tick;

	gettimeofday(&tval, NULL);

	ret_tick = tval.tv_sec * 1000L + tval.tv_usec / 1000L;
	return ret_tick;
#endif
}

void util_sleep(uint32_t millisecond)
{
#ifdef _WIN32
	Sleep(millisecond);
#else
	usleep(millisecond * 1000);
#endif
}


#endif