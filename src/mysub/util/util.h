
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include "slog_api.h"

extern CSLog g_iminfo;
extern CSLog g_imdebug;
extern CSLog g_imerror;
extern CSLog g_imwarn;
extern CSLog g_imtrace;

#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)
#define logi(fmt, args...)  g_iminfo.Info("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define logd(fmt, args...) g_imdebug.Debug("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define logw(fmt, args...) g_imwarn.Warn("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define loge(fmt, args...) g_imerror.Error("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define logt(fmt, args...) g_imtrace.Trace("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)

namespace util{

using namespace std;

void split(string s, vector<string>& ret);
void split(string s, vector<string>& ret, char sep);

}