#ifndef __EG_IO__
#define __EG_IO__

#include "eg_common.h"

#define Egio::Instance() g_egio

class EgException : public std::exception
{
	string _why;
public:
	EgException(const char* why) : _why(why) {}
	virtual const char* what() const throw() {
		return _why.c_str();
	}
}

class EgConn {
	
public:
	EgConn() {}
	virtual ~EgConn() {}
	
	virtual OnRead() {}
	virtual OnWrite() {}
	virtual OnClose() {}
	
	virtual const char* GetConnIp() {}
	virtual uint16_t GetConnPort() {}
}

class Egio {

	bool _loop_running;
	map<int, EgConn*> _conn_map;

public:

	typedef enum {
		EG_STATE_IDLE = 1,
		EG_STATE_LISTENING,
		EG_STATE_CONNECTING,
		EG_STATE_CONNECTED,
		EG_STATE_CLOSING
	} sock_state_t;

	int Connect(const char* server_ip, uint16_t port, EgConn* conn);
	int SetAddr(const char* ip, const uint16_t port, sockaddr_in* pAddr)
	void AddEvent(int fd);
	void RemoveEvent(int fd);
	void OnWrite(int fd);
	void OnClose(int fd);
	void OnRead(int fd);
	void StartLoop();
	
	static Egio* Instance();

}

#endif