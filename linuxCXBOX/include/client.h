#ifndef CLIENT_H
#define CLIENT_H

#include "message.h"
#include <string>

class client
{
	// variables in order for the send and receive methods to work properly
	sockaddr_in m_si_server = { 0 };
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

	// some helper methods to get readable variables
	std::string& interface() { return m_interface; };
	std::string get_ip();
	std::string get_ip_server();
	// some helper methods to send specific data to a server
	void send_ping();
	void send_input_controller_btn_change(uint8_t* data);
	void send_input_controller_btn_all(uint8_t* data);
	
	//int recv_wait();
};

#endif // CLIENT_H