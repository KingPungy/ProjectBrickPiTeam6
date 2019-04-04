#include "../include/server.h"

#include "../include/ip.h"
#include <arpa/inet.h>

server::server(int port)
{
	m_si_server.sin_family = AF_INET;
	m_si_server.sin_port = htons(port);
	m_si_size = sizeof(m_si_server);
	m_sockfd = socket(m_si_server.sin_family, SOCK_DGRAM | SOCK_NONBLOCK, 0);
	bind(m_sockfd, (sockaddr*)&m_si_server, sizeof(m_si_server));
	m_interface = std::string("wlan0");
	m_msgq_recv = std::vector<s_message>();
	m_msgq_send = std::vector<s_message>();
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

bool server::has_message()
{
	s_message msg;
	msg.bytes = recv_message(msg.msg, m_sockfd, m_si_client);
	m_msgq_recv.push_back(msg);

	if (msg.bytes <= MESSAGE_HEADER_SIZE)
		return false;
	if (msg.bytes < MESSAGE_HEADER_SIZE + msg.msg.u.s.size)
		return false;
	return true;
}

const message server::get_message()
{
	if (m_msgq_recv.size() > 0)
	{
    	m_msgq_recv.front() = std::move(m_msgq_recv.back());
    	s_message msg = m_msgq_recv.back();
		m_msgq_recv.pop_back();
		return msg.msg;
	}

	return message { 0 };
}

void server::send_ping()
{
	s_message msg;
	msg.msg.u.s.id = MESSAGE_ID_PING;
	msg.msg.u.s.size = 0;
	msg.msg.data = 0;

	int err = send_message(msg.msg, m_sockfd, m_si_client);
}
