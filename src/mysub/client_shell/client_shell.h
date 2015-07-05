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

using namespace std;

#define PROMPT		"im-client> "

class CClientShell : public CThread
{
public:
    CClientShell();
    virtual ~CClientShell(){}
    void SetLoginServer(string url);
    void SetIoloop(CEpollIOLoop* io);
    void Start();
    void DispatchCmd(vector<string>* cmds);
    void PrintCmdsHist();
    virtual void OnThreadRun();

private:
    string m_loginserver_url;
    vector<vector<string>*> m_cmds_hist;
    CEpollIOLoop* m_io;

};


#endif