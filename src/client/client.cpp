#include "netlib.h"
#include "HttpClient.h"
#include "json/json.h"

void server_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
	
}

void timer_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
	
}

void connect_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
	
}

void get_msgserver(char*& ip, uint32_t& port)
{
	CHttpClient ht;
	CURLcode nRet = ht.Get("http://127.0.0.1:8080/msg_server", resp);
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
        nPort = value["port"].asUInt();
    } catch (std::runtime_error msg) {
        loge("login falied. get json error:%s\n", msg.what());
        return;
    }
    ip = strPriorIp.c_str();
    port = nPort;
    logi("priorIP=%s, port=%d", ip, nPort);
}

int main(int argc, char* argv[])
{
    if ((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
		printf("Build: %s %s\n", __DATE__, __TIME__);
		return 0;
	}
	
	writePid();
	
	netlib_listen("0.0.0.0", 18000, server_callback, NULL);
	netlib_register_timer(timer_callback, NULL, 1000);
	
	char* ip;
	uint16_t port;
	get_msgserver(ip, port);
	netlib_connet(ip, port, connect_callback, NULL);

	netlib_eventloop();
}