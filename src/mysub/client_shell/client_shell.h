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
    CClientHandler(CTCPClientAsync*);
    virtual ~CClientHandler() {}
    
    virtual void OnException(uint32_t nsockid, int32_t nErrorCode);
    virtual void OnClose(uint32_t nsockid);
    virtual void OnConnect(uint32_t nsockid);
    virtual void OnRecvData(const char* szBuf, int32_t nBufSize);
    
private:
    CTCPClientAsync* _pTcpClient;
};

class CClientShell : public CThread, public CBaseHandler
{

public:
    CClientShell() { _cmdsHist = vector<vector<string>*>(); }
    virtual ~CClientShell(){}
    
    void SetClient(CTCPClientAsync* client) { _pTcpClient = client; }
    void SetClientHandler(CClientHandler* handler) { _pClientHandler = handler; }
    
    void Start();
    void DispatchCmd(vector<string>* cmds);
    void PrintCmdsHist();
    
    void Login(string&, string&);
    void ConnectServer();
    
    virtual void OnThreadRun();

private:
    vector<vector<string>*> _cmdsHist;
    CTCPClientAsync* _pTcpClient;
    CClientHandler* _pClientHandler;
    CHttpClient _httpClient;

};

#endif