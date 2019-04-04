#include "../include/message.h"

#include <cstring>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctime>

int send_message_raw(const message& msg, const int sockfd, const sockaddr_in& si_client)
{
	uint8_t buf[MESSAGE_LEN_MAX];
	memset(buf, 0, MESSAGE_LEN_MAX);
	union
	{
		uint8_t b[2];
		uint16_t i;
	} u_size;
	union
	{
		uint8_t b[sizeof(time_t)];
		time_t t;
	} u_time;
	int offset = 0;
	buf[offset++] = msg.id;
	u_size.i = msg.size;
	for (int i = 0; i < 2; i++)
		buf[i + offset] = u_size.b[i];
	offset += 2;
	//msg.time = time(0);
	u_time.t = time(0);
	for (int i = 0; i < sizeof(time_t); i++)
		buf[i + offset] = u_size.b[i];
	offset += sizeof(time_t);
	
	memcpy(buf + offset, msg.data, msg.size);
	sendto(sockfd, buf, MESSAGE_LEN_MAX, 0, (struct sockaddr*)&si_client, sizeof(si_client));
}

int recv_message_raw(message& msg, const int sockfd, const sockaddr_in& si_client)
{
	uint8_t buf[MESSAGE_LEN_MAX];
	memset(buf, 0, MESSAGE_LEN_MAX);
	socklen_t addr_size = sizeof(si_client);
	recvfrom(sockfd, buf, MESSAGE_LEN_MAX, 0, (struct sockaddr*)&si_client, &addr_size);
	union
	{
		uint8_t b[2];
		uint16_t i;
	} u_size;
	union
	{
		uint8_t b[sizeof(time_t)];
		time_t t;
	} u_time;
	int offset = 0;
	msg.id = buf[offset++];
	for (int i = 0; i < 2; i++)
		u_size.b[i] = buf[i + offset];
	msg.size = u_size.i;
	offset += 2;
	for (int i = 0; i < sizeof(time_t); i++)
		u_size.b[i] = buf[i + offset];
	offset += sizeof(time_t);
	msg.time = u_time.t;
	memcpy(msg.data, buf + offset, msg.size);
}
