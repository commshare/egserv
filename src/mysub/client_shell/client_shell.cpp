//
//  client_shell.cpp
//  client_shell
//
//  Created by luopeng on 2015.7.1.
//  
//

#include "client_shell.h"

void CClientHandler::OnException(uint32_t nsockid, int32_t nErrorCode)
{
	
}

void CClientHandler::OnClose(uint32_t nsockid)
{
	
}

void CClientHandler::OnConnect(uint32_t nsockid)
{
	
}

void CClientHandler::OnRecvData(const char* szBuf, int32_t nBufSize)
{
	
}


void CClientShell::Start()
{
	m_client->DoConnect.connect(m_handler, &CClientHandler::OnConnect);
    m_client->DoClose.connect(m_handler, &CClientHandler::OnClose);
    m_client->DoException.connect(m_handler, &CClientHandler::OnException);
    m_client->DoRecv.connect((CBaseHandler*)m_handler, &CClientHandler::OnRecv);
    StartThread();

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

void CClientShell::Login(string& username, string& password, string url)
{
	string resp;
    CURLcode nRet = m_http_client.Get(url, resp);
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
}

void CClientShell::PrintCmdsHist()
{
	for(auto cmds : m_cmds_hist){
		for(auto cmd : (*cmds)){
			cout << "[" << cmd << "]";
		}
		cout << endl;
	}
	for(auto it1 = m_cmds_hist.begin(); it1 != m_cmds_hist.end(); it1++){
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
		m_cmds_hist.push_back(cmds);
		DispatchCmd(cmds);
	}
}
