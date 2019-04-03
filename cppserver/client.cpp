#include "client.h"

#include "ip.h"
#include <arpa/inet.h>

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
	msg.id = MESSAGE_ID_PING;
	msg.size = 0;
	msg.data = 0;

	int err = send_message_raw(msg, m_sockfd, m_si_server);
}

void client::send_input(void* data)
{
	message msg;
	msg.id = MESSAGE_ID_INPUT;
	msg.size = 3;
	msg.data = (uint8_t*)data;

	int err = send_message_raw(msg, m_sockfd, m_si_server);
}
