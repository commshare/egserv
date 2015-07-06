//
//  client_shell.h
//  
//
//  Created by luopeng 2015.7.1
//  
//

#ifndef __client_shell__client_shell__
#define __client_shell__client_shell__

#include <string>
#include <vector>
#include "Thread.h"
#include "util.h"
#include "slog_api.h"
#include "epoll_io_loop.h"
#include "base_handler.hpp"
#include "tcp_client_async.h"
#include "HttpClient.h"
#include "json/json.h"

#define PROMPT		"im-client> "

using namespace std;

class CClientHandler : public CBaseHandler
{
public:
    CClientHandler() {}
    virtual ~CClientHandler() {}
    
    virtual void OnException(uint32_t nsockid, int32_t nErrorCode);
    virtual void OnClose(uint32_t nsockid);
    virtual void OnConnect(uint32_t nsockid);
    virtual void OnRecvData(const char* szBuf, int32_t nBufSize);
    
private:
    
};

class CClientShell : public CThread, public CBaseHandler
{

public:
    CClientShell() { m_cmds_hist = vector<vector<string>*>(); }
    virtual ~CClientShell(){}
    
    void SetClient(CTCPClientAsync* client) { m_client = client; }
    void SetClientHandler(CClientHandler* handler) { m_handler = handler; }
    
    void Start();
    void DispatchCmd(vector<string>* cmds);
    void PrintCmdsHist();
    
    void Login(string&, string&, string url="http://127.0.0.1:8080/msg_server");
    
    virtual void OnThreadRun();

private:
    vector<vector<string>*> m_cmds_hist;
    CEpollIOLoop* m_io;
    CTCPClientAsync* m_client;
    CClientHandler* m_handler;
    CHttpClient m_http_client;

};

#endif