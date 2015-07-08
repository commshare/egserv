
#include "eg_common.h"
#include "egio.h"
#include "util.h"

Egio* Egio::Instance()
{
	static Egio egio;
	return &egio;
}

Egio::Egio()
{
	_epfd = epoll_create(1024);
	if (_epfd == -1) {
		loge("epoll_create failed");
		throw EgException("epoll_create failed");
	}
}

int Egio::SetAddr(const char* ip, const uint16_t port, sockaddr_in* pAddr)
{
	memset(pAddr, 0, sizeof(sockaddr_in));
	pAddr->sin_family = AF_INET;
	pAddr->sin_port = htons(port);
	pAddr->sin_addr.s_addr = inet_addr(ip);
	if (pAddr->sin_addr.s_addr == INADDR_NONE) {
		hostent* host = gethostbyname(ip);
		if (host == NULL) {
			loge("gethostbyname failed, ip=%s", ip);
			return INADDR_NONE;
		}
		pAddr->sin_addr.s_addr = *(uint32_t*)host->h_addr;
	}
}

int Egio::Connect(const char* ip, uint16_t port, EgConn* conn)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		loge("socket failed, err_code=%d", errno);
		throw EgException("socket create failed");
	}
	//set nonblock
	if (fcntl(sock, F_SETFL, O_NONBLOCK | fcntl(sock, F_GETFL)) == SOCKET_ERROR) {
		loge("set nonblock failed, err_code=%d", errno);
		throw EgException("set nonblock failed");
	}
	//set nodelay
	int nodelay = 1;
	if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay)) == SOCKET_ERROR) {
		loge("set nodelay failed, err_code=%d", errno);
		throw EgException("set nodelay failed");
	}
	
	sockaddr_in serv_addr;
	SetAddr(ip, port,  &serv_addr);
	if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
		if (!(errno == EINPROGRESS) || (errno == EWOULDBLOCK)) {
			loge("connect failed, err_code=%d", errno);
			close(sock);
			throw EgException("connect failed");
		}
	}
	
	conn->SetState(EGIO_STATE_CONNECTING);
	conn->SetPeerIp(ip);
	conn->SetPeerPort(port);
	_conn_map[sock] = conn;
	
	AddEvent(sock);
	
	return sock;
	
}

void Egio::AddEvent(int fd)
{
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLPRI | EPOLLERR | EPOLLHUP;
	ev.data.fd = fd;
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev) != 0) {
		loge("epoll_ctl add event failed, errno=%d", errno);
	}
}

void Egio::RemoveEvent(int fd)
{
	if (epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL) != 0) {
		loge("epoll_ctl remove event failed, errno=%d", errno);
	}
}

void Egio::OnWrite(int fd)
{
	EgConn* conn;
	auto it = _conn_map.find(fd);
	if (it != _conn_map.end()) {
		conn = it->second;
	} else {
		loge("_conn_map find fd=%d failed ", fd);
		throw EgException("_conn_map find failed");
	}
	
	if (conn->GetState() == EGIO_STATE_CONNECTING) {
		conn->SetState(EGIO_STATE_CONNECTED);
		conn->OnConfirm();
	} else {
		conn->OnWrite();
	}
}

void Egio::OnClose(int fd)
{
	
}

void Egio::OnRead(int fd)
{
	
}

void Egio::StartLoop()
{
	struct epoll_event events[1024];
	int nfds = 0;

    if(_loop_running)
        return;
    _loop_running = true;
    
	while (_loop_running)
	{
		nfds = epoll_wait(_epfd, events, 1024, 100); //default wait timeout 100
		for (int i = 0; i < nfds; i++)
		{
			int ev_fd = events[i].data.fd;
			
            #ifdef EPOLLRDHUP
            if (events[i].events & EPOLLRDHUP)
            {
                logt("On Peer Close, socket=%d", ev_fd);
                OnClose(ev_fd);
            }
            #endif

			if (events[i].events & EPOLLIN)
			{
				logt("OnRead, socket=%d", ev_fd);
				OnRead(ev_fd);
			}

			if (events[i].events & EPOLLOUT)
			{
				logt("OnWrite, socket=%d", ev_fd);
				OnWrite(ev_fd);
			}

			if (events[i].events & (EPOLLPRI | EPOLLERR | EPOLLHUP))
			{
				logt("OnClose, socket=%d", ev_fd);
				OnClose(ev_fd);
			}

		}

// _CheckTimer();
// _CheckLoop();
	}
}

