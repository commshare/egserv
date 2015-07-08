#ifndef __EG_IO__
#define __EG_IO__

#include "eg_common.h"

#define g_egio Egio::Instance()

typedef enum {
	EGIO_STATE_IDLE = 1,
	EGIO_STATE_LISTENING,
	EGIO_STATE_CONNECTING,
	EGIO_STATE_CONNECTED,
	EGIO_STATE_CLOSING
} eg_conn_state_t;

class EgException : public std::exception
{
	std::string _why;
public:
	EgException(const char* why) : _why(why) {}
	virtual const char* what() const throw() {
		return _why.c_str();
	}
};

class EgConn {
	
protected:
	eg_conn_state_t _conn_state;
	uint16_t _peer_port;
	std::string _peer_ip;
	
public:
	EgConn() {}
	virtual ~EgConn() {}
	
	virtual void OnRead() {}
	virtual void OnWrite() {}
	virtual void OnClose() {}
	virtual void OnConfirm() {}
	
	void SetState(eg_conn_state_t state) { _conn_state = state; }
	eg_conn_state_t& GetState() { return _conn_state; }
	
	void SetPeerIp(const char* ip) { _peer_ip = ip; }
	void SetPeerPort(uint16_t port) { _peer_port = port; }
	
	std::string& GetPeerIp() { return _peer_ip; }
	uint16_t GetPeerPort() { return _peer_port; }
	
};

class Egio {

	bool _loop_running;
	std::map<int, EgConn*> _conn_map;
	int _epfd;

public:
	Egio();
	virtual ~Egio() {}
	int Connect(const char* ip, uint16_t port, EgConn* conn);
	int SetAddr(const char* ip, const uint16_t port, sockaddr_in* pAddr);
	void AddEvent(int fd);
	void RemoveEvent(int fd);
	void OnWrite(int fd);
	void OnClose(int fd);
	void OnRead(int fd);
	void StartLoop();

	static Egio* Instance();

};

#endif