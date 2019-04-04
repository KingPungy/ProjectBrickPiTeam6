#ifndef SERVER_H
#define SERVER_H

#include "message.h"
#include <string>
#include <vector>

class server
{
	sockaddr_in m_si_server = { 0 };
	sockaddr_in m_si_client = { 0 };
	socklen_t m_si_size = 0;
	int m_sockfd = 0;

	struct s_message
	{
		message msg;
		int bytes;
	};
	
	std::vector<s_message> m_msgq_recv;
	std::vector<s_message> m_msgq_send;
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
	bool has_message();
	const message get_message();						// gets the last message from the queue
	void send_ping();
};

#endif // SERVER_H
