#include "../include/message.h"

#include <cstring>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctime>

#include <cstdio>
int send_message(const message& msg, const int sockfd, const sockaddr_in& si)
{
	uint8_t buf[MESSAGE_LEN_MAX];
	memset(buf, 0, MESSAGE_LEN_MAX);

	//msg.s.time = time(0);

	int o;
	for (o = 0; o < MESSAGE_HEADER_SIZE; o++)
		buf[o] = msg.u.b[o];

	memcpy(buf + o, msg.data, msg.u.s.size);
	int sbytes = sendto(sockfd, buf, MESSAGE_LEN_MAX, 0, (sockaddr*)&si, sizeof(si));
	
	//for (int i = 0; i < MESSAGE_HEADER_SIZE + msg.u.s.size; i++)
	//{
	//	if (i % 16 == 0)
	//		printf("\n");
	//	printf("%#x ", buf[i]);
	//}
	//printf("\n");
	
	return sbytes;
}

int recv_message(message& msg, const int sockfd, const sockaddr_in& si)
{
	uint8_t buf[MESSAGE_LEN_MAX];
	memset(buf, 0, MESSAGE_LEN_MAX);
	socklen_t addr_size = sizeof(si);
	int rbytes = recvfrom(sockfd, buf, MESSAGE_LEN_MAX, 0, (sockaddr*)&si, &addr_size);

	int readto = rbytes > MESSAGE_HEADER_SIZE ? MESSAGE_HEADER_SIZE : rbytes;

	int o = 0;
	for (o = 0; o < rbytes; o++)
		msg.u.b[o] = buf[o];
	
	if (msg.data)
		delete[] msg.data;

	if (rbytes > MESSAGE_HEADER_SIZE)
	{
		msg.data = new uint8_t[msg.u.s.size];
		
		memcpy(msg.data, buf + o, msg.u.s.size);
	}
	return rbytes;
}