#ifndef CLIENT_H
#define CLIENT_H

#include "message.h"
#include <string>

class client
{
	sockaddr_in m_si_server = { 0 };
	socklen_t m_si_size = 0;
	int m_sockfd = 0;
	message m_sendmsg = { 0 };
	message m_recvmsg = { 0 };
	int m_senderr = 0;
	int m_recverr = 0;
	std::string m_interface;
	
public:
	client() = default;
	client(const std::string& server_ip, int port = DEFAULT_PORT);
	~client();

	std::string& interface() { return m_interface; };
	std::string get_ip();
	std::string get_ip_server();
	void send_ping();
	void send_input(void* data);
	//int recv_wait();
};

#endif // CLIENT_H