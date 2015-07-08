#include "HttpClient.h"
#include "util.h"
#include "json/json.h"

#include "egio.h"


class ClientConn : public EgConn
{
    string _msg_prior_ip;
    string _msg_backup_ip;
    uint16_t _port;
    string _login_url;
public:

    ClientConn(string loginUrl) : _login_url(loginUrl) {
        _GetMsgServer();
    }

    virtual void OnConfirm() () {
        logi("connect to %s:%d success!", _ip.c_str(), _port);
    }
    
    virtual const char* GetConnIp() {
        return _msg_backup_ip.c_str();
    }
    
    virtual uint16_t GetConnPort() {
        return _port;
    }
    
    void _GetMsgServer() {
    	string resp;
    	CHttpClient ht;

        CURLcode nRet = ht.Get(_login_url, resp);
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

        uint32_t nRet = value["code"].asUInt();
        if(nRet != 0)
        {
            string strMsg = value["msg"].asString();
            loge("login falied. errorMsg:%s\n", strMsg.c_str());
            return;
        }
        _msg_prior_ip = value["priorIP"].asString();
        _msg_backup_ip = value["backupIp"].asString();
        _port = value["port"].asUInt();

        logi("priorIP=%s, backIP=%s, port=%d", strPriorIp.c_str(), strBackupIp.c_str(), nPort);
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
	
	get_msg_server(ip, port);
	logi("ip=%s, port=%d", ip.c_str(), port);
	ToyConn* toyconn = new ToyConn("http://127.0.0.1:8080/msg_server");
	g_egio->Connect(toyconn);
	g_egio->StartLoop();
	
	return 0;
}