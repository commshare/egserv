#include "HttpClient.h"
#include "util.h"
#include "json/json.h"

#include "egio.h"

typedef struct {
    string priorIp;
    string backupIp;
    uint16_t port;
} msg_server_url_t;

static void get_msg_server_url(string login_url, msg_server_url_t* msg_url) {
	string resp;
	CHttpClient ht;

    CURLcode nRet = ht.Get(login_url, resp);
    if(nRet != CURLE_OK){
        loge("login falied. access url:%s error\n", login_url.c_str());
        return;
    }
    
    Json::Reader reader;
    Json::Value value;
    if(!reader.parse(resp, value))
    {
        loge("login falied. parse response error:%s\n", resp.c_str());
        return;
    }

    uint32_t code = value["code"].asUInt();
    if(code != 0)
    {
        string strMsg = value["msg"].asString();
        loge("login falied. errorMsg:%s\n", strMsg.c_str());
        return;
    }
    msg_url->priorIp = value["priorIP"].asString();
    msg_url->backupIp = value["backupIp"].asString();
    msg_url->port = value["port"].asUInt();

    logi("priorIP=%s, backIP=%s, port=%d", 
        msg_url->priorIp.c_str(), msg_url->backupIp.c_str(), msg_url->port);
}


class ClientConn : public EgConn
{

public:

    virtual void OnConfirm()  {
        logi("connect to %s:%d success!", _peer_ip.c_str(), _peer_port);
    }
    

};

int main(int argc, char* argv[])
{
    if ((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
		printf("Build: %s %s\n", __DATE__, __TIME__);
		return 0;
	}
	
	string ip;
	uint16_t port;
    msg_server_url_t url;
    get_msg_server_url("http://127.0.0.1:8080/msg_server", &url);
	logi("ip=%s, port=%d", url.priorIp.c_str(), url.port);
	ClientConn* clientConn = new ClientConn();
	g_egio->Connect(url.priorIp.c_str(), url.port, clientConn);
	g_egio->StartLoop();
	
	return 0;
}