#include "netlib.h"
#include "HttpClient.h"
#include "json/json.h"
#include "util.h"

void get_msg_server(string& ip, uint16_t& port)
{
	string resp;
	CHttpClient ht;
	string url = "http://127.0.0.1:8080/msg_server";
    CURLcode nRet = ht.Get(url, resp);
    if(nRet != CURLE_OK){
        loge("login falied. access url:%s error\n", url.c_str());
        return;
    }
    
    Json::Reader reader;
    Json::Value value;
    if(!reader.parse(resp, value))
    {
        loge("login falied. parse response error:%s\n", resp.c_str());
        return;
    }
    string strPriorIp, strBackupIp;
    uint16_t nPort;
    try {
        uint32_t nRet = value["code"].asUInt();
        if(nRet != 0)
        {
            string strMsg = value["msg"].asString();
            loge("login falied. errorMsg:%s\n", strMsg.c_str());
            return;
        }
        strPriorIp = value["priorIP"].asString();
        strBackupIp = value["backupIp"].asString();
        nPort = value["port"].asUInt();
    } catch (std::runtime_error msg) {
        loge("login falied. get json error:%s\n", msg.what());
        return;
    }
    ip = strPriorIp;
    port = nPort;
    logi("priorIP=%s, backIP=%s, port=%d", strPriorIp.c_str(), strBackupIp.c_str(), nPort);
}

void connect_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
	
}

int main(int argc, char* argv[])
{
    if ((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
		printf("Build: %s %s\n", __DATE__, __TIME__);
		return 0;
	}
	
	string ip;
	uint16_t port;
	
	get_msg_server(ip, port);
	logi("ip=%s, port=%d", ip.c_str(), port);
	netlib_connect(ip.c_str(), port, connect_callback, NULL);
	
	netlib_eventloop();
	return 0;
}