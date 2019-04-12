#include "../include/client.h"

#include "../include/ip.h"
#include <arpa/inet.h>
#include <cstring>

client::client(const std::string& server_ip, int port)
{
	m_si_server.sin_family = AF_INET;
	m_si_server.sin_port = htons(port);
	inet_aton(server_ip.c_str(), &m_si_server.sin_addr);
	m_si_size = sizeof(m_si_server);

	m_sockfd = socket(m_si_server.sin_family, SOCK_DGRAM, 0);
	m_interface = "wlan0";
}

client::~client()
{

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
	message msg;
	msg.u.s.id = MESSAGE_ID_PING;
	msg.u.s.size = 0;
	msg.data = 0;

	int err = send_message(msg, m_sockfd, m_si_server);
}

void client::send_input_controller_btn_change(uint8_t* data)
{
	message msg;
	msg.u.s.id = MESSAGE_ID_INPUT_CONTROLLER_BTN_CHANGE;
	//msg.u.s.time = time(0);
	msg.u.s.size = 3;
	msg.data = data;
	
	int err = send_message(msg, m_sockfd, m_si_server);
}


void client::send_input_controller_btn_all(uint8_t* data)
{
	message msg;
	msg.u.s.id = MESSAGE_ID_INPUT_CONTROLLER_BTN_ALL;
	//msg.u.s.time = time(0);
	msg.u.s.size = 12;
	msg.data = (uint8_t*)data;
	
	int err = send_message(msg, m_sockfd, m_si_server);
}

void client::send_tts(char* data)
{
	message msg;
	msg.u.s.id = MESSAGE_ID_TTS;
	msg.u.s.size = strlen(data);
	msg.data = (uint8_t*)data;

	int err = send_message(msg, m_sockfd, m_si_server);
}