//
//  client_shell.cpp
//  client_shell
//
//  Created by luopeng on 2015.7.1.
//  
//

#include "client_shell.h"

CClientShell::CClientShell()
{
	m_cmds_hist = vector<vector<string>*>();
}

void CClientShell::DispatchCmd(vector<string>* cmds)
{
	if(cmds->size() == 2 && (*cmds)[0] == "show" && 
		(*cmds)[1] == "hist"){
		PrintCmdsHist();
	}
	
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


void CClientShell::SetLoginServer(string url)
{
    m_loginserver_url = url;
}

void CClientShell::SetIoloop(CEpollIOLoop* io)
{
	m_io = io;
}


void CClientShell::Start()
{
    StartThread();
    
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
