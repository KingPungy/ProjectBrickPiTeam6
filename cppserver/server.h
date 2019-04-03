#ifndef SERVER_H
#define SERVER_H

#include "message.h"
#include <string>

class server
{
	sockaddr_in m_si_server = { 0 };
	sockaddr_in m_si_client = { 0 };
	socklen_t m_si_size = 0;
	int m_sockfd = 0;
	message m_sendmsg = { 0 };
	message m_recvmsg = { 0 };
	int m_senderr = 0;
	int m_recverr = 0;
	std::string m_interface;
	
public:
	server() = default;
	server(int port);
	~server();

	std::string& interface() { return m_interface; };
	std::string get_ip();
	std::string get_ip_client();
	const message& get_msg();
	void wait_msg();
	void send_ping();
	//int recv_wait();
};

#endif // SERVER_H
