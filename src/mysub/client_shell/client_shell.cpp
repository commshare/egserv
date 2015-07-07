//
//  client_shell.cpp
//  client_shell
//
//  Created by luopeng on 2015.7.1.
//  
//

#include "client_shell.h"

CClientHandler::CClientHandler(CTCPClientAsync* tcpclient) : _pTcpClient(tcpclient) {}

void CClientHandler::OnException(uint32_t nsockid, int32_t nErrorCode)
{
    _pTcpClient->ShutDown();
}

void CClientHandler::OnClose(uint32_t nsockid)
{
	
}

void CClientHandler::OnConnect(uint32_t nsockid)
{
	logi("On connect!!!");
}

void CClientHandler::OnRecvData(const char* szBuf, int32_t nBufSize)
{
	
}


void CClientShell::Start()
{
	_pTcpClient->DoConnect.connect(_pClientHandler, &CClientHandler::OnConnect);
    _pTcpClient->DoClose.connect(_pClientHandler, &CClientHandler::OnClose);
    _pTcpClient->DoException.connect(_pClientHandler, &CClientHandler::OnException);
    _pTcpClient->DoRecv.connect((CBaseHandler*)_pClientHandler, &CClientHandler::OnRecv);
    ConnectServer();
    StartThread();

}

void CClientShell::ConnectServer()
{
	string resp;
	string url="http://127.0.0.1:8080/msg_server";
	
    CURLcode nRet = _httpClient.Get(url, resp);
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
    logi("priorIP=%s, backIP=%s, port=%d", strPriorIp.c_str(), strBackupIp.c_str(), nPort);
    _pTcpClient->ConnectAsync(strPriorIp.c_str(), nPort);
}

void CClientShell::DispatchCmd(vector<string>* cmds)
{
	if(cmds->size() == 2 && (*cmds)[0] == "show" && 
		(*cmds)[1] == "hist"){
		PrintCmdsHist();
	}else if(cmds->size() == 3 && (*cmds)[0] == "login"){
        string username = (*cmds)[1];
        string password = (*cmds)[2];
        Login(username, password);
    }
	
}

void CClientShell::Login(string& username, string& password)
{

}

void CClientShell::PrintCmdsHist()
{
	for(auto cmds : _cmdsHist){
		for(auto cmd : (*cmds)){
			cout << "[" << cmd << "]";
		}
		cout << endl;
	}
	for(auto it1 = _cmdsHist.begin(); it1 != _cmdsHist.end(); it1++){
		for(auto it2 = (*it1)->begin(); it2 != (*it1)->end(); it2++){
			cout << "<"  << *it2 << ">";
		}
		cout << endl;
	}

}

void CClientShell::OnThreadRun()
{
    string line;
    vector<string>* cmds;
    
	while (true)
	{
		cout << PROMPT << " ";
		getline(cin, line);
		cmds = new vector<string>();
		util::split(line, *cmds);
		_cmdsHist.push_back(cmds);
		DispatchCmd(cmds);
	}
}
