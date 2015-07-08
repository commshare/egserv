
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
		throw egex("epoll_create failed");
	}
}

void Egio::SetAddr(const char* ip, const uint16_t port, sockaddr_in* pAddr)
{
	memset(pAddr, 0, sizeof(sockaddr_in));
	pAddr->sin_family = AF_INET;
	pAddr->sin_port = htons(port);
	pAddr->sin_addr.s_addr = inet_addr(ip);
	if (pAddr->sin_addr.s_addr == INADDR_NONE) {
		hostent* host = gethostbyname(ip);
		if (host == NULL) {
			throw egex("gethostbyname failed, ip=%s", ip);
		}
		pAddr->sin_addr.s_addr = *(uint32_t*)host->h_addr;
	}
}

int Egio::Connect(const char* ip, uint16_t port, EgConn* conn)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		loge("socket failed, err_code=%d", errno);
		throw egex("socket create failed, errno=%d", errno);
	}
	//set nonblock
	if (fcntl(sock, F_SETFL, O_NONBLOCK | fcntl(sock, F_GETFL)) == SOCKET_ERROR) {
		loge("set nonblock failed, err_code=%d", errno);
		throw egex("set nonblock failed, errno=%d", errno);
	}
	//set nodelay
	int nodelay = 1;
	if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay)) == SOCKET_ERROR) {
		loge("set nodelay failed, err_code=%d", errno);
		throw egex("set nodelay failed, errno=%d", errno);
	}
	
	sockaddr_in serv_addr;
	SetAddr(ip, port,  &serv_addr);
	if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
		if (!(errno == EINPROGRESS) || (errno == EWOULDBLOCK)) {
			loge("connect failed, err_code=%d", errno);
			close(sock);
			throw egex("connect failed, errno=%d", errno);
		}
	}
	
	conn->SetState(EGIO_STATE_CONNECTING);
	conn->SetPeerIp(ip);
	conn->SetPeerPort(port);
	_conn_map[sock] = std::shared_ptr<EgConn>(conn);
	
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
	std::shared_ptr<EgConn> conn;
	auto it = _conn_map.find(fd);
	if (it != _conn_map.end()) {
		conn = it->second;
	} else {
		loge("_conn_map find fd=%d failed ", fd);
		throw egex("_conn_map find fd=%d failed", fd);
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
	_conn_map[fd]->OnClose();
	_conn_map.erase(fd);
	close(fd);
}

void Egio::OnRead(int fd)
{
	uint64_t avail;
	if ((ioctl(fd, FIONREAD, &avail) == SOCKET_ERROR) || (avail == 0)) {
		OnClose(fd);
	}
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

