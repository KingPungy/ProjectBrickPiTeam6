#include "../include/client.h"

#include "../include/ip.h"
#include <arpa/inet.h>
#include <cstring>

client::client(const std::string& server_ip, int port)
{
	// set up the class
	m_si_server.sin_family = AF_INET;
	m_si_server.sin_port = htons(port);
	inet_aton(server_ip.c_str(), &m_si_server.sin_addr);

	m_sockfd = socket(m_si_server.sin_family, SOCK_DGRAM, 0);
	m_interface = "wlan0";
}

client::~client()
{
	// if these pointers exist deallocate them
	if (m_sendmsg.data)
		delete[] m_sendmsg.data;
	if (m_recvmsg.data)
		delete[] m_recvmsg.data;
}

std::string client::get_ip()
{
	char buf[16];
	in_addr ina;
	int err = get_ip_str(&ina, buf, true, m_interface.c_str());
	return std::string(buf);
}

std::string client::get_ip_server()
{
	char buf[16];
	int err = get_ip_str(&m_si_server.sin_addr, buf, false, m_interface.c_str());
	return std::string(buf);
}

void client::send_ping()
{
	// create a message struct with the correct data
	message msg;
	msg.s.id = MESSAGE_ID_PING;
	msg.s.size = 0;
	msg.data = 0;

	// send the data
	int err = send_message(msg, m_sockfd, m_si_server);
}

void client::send_input_controller_btn_change(uint8_t* data)
{
	// create a message struct with the correct data
	message msg;
	msg.s.id = MESSAGE_ID_INPUT_CONTROLLER_BTN_CHANGE;
	msg.s.size = 3;
	msg.data = data;
	
	// send the data
	int err = send_message(msg, m_sockfd, m_si_server);
}


void client::send_input_controller_btn_all(uint8_t* data)
{
	// create a message struct with the correct data
	message msg;
	msg.s.id = MESSAGE_ID_INPUT_CONTROLLER_BTN_ALL;
	msg.s.size = 12;
	msg.data = (uint8_t*)data;
	
	// send the data
	int err = send_message(msg, m_sockfd, m_si_server);
}