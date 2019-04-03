#include "server.h"

#include "ip.h"
#include <arpa/inet.h>

server::server(int port)
{
	m_si_server.sin_family = AF_INET;
	m_si_server.sin_port = htons(port);
	m_si_size = sizeof(m_si_server);
	m_sockfd = socket(m_si_server.sin_family, SOCK_DGRAM, 0);
	
	bind(m_sockfd, (sockaddr*)&m_si_server, sizeof(m_si_server));
	m_interface = "wlan0";
}

server::~server()
{

}

std::string server::get_ip()
{
	char buf[16];
	int err = get_ip_str(&m_si_server.sin_addr, buf, false, m_interface.c_str());
	return std::string(buf);
}

std::string server::get_ip_client()
{
	char buf[16];
	int err = get_ip_str(&m_si_client.sin_addr, buf, false, m_interface.c_str());
	return std::string(buf);
}

const message& server::get_msg()
{
	return m_recvmsg;
}

void server::wait_msg()
{
	recv_message_raw(m_recvmsg, m_sockfd, m_si_client);
}

void server::send_ping()
{
	message msg;
	msg.id = MESSAGE_ID_PING;
	msg.size = 0;
	msg.data = 0;

	int err = send_message_raw(msg, m_sockfd, m_si_client);
}
