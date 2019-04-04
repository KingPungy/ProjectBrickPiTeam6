#include "../include/message.h"

#include <cstring>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int send_message(const message& msg, const int sockfd, const sockaddr_in& si)
{
	uint8_t hbuf[MESSAGE_LEN_MAX];
	memset(hbuf, 0, MESSAGE_LEN_MAX);

	int o;
	for (o = 0; o < MESSAGE_HEADER_SIZE; o++)
		hbuf[0] = msg.b[o];

	memcpy(hbuf + o, msg.data, msg.s.size);
	int sbytes = sendto(sockfd, hbuf, MESSAGE_LEN_MAX, 0, (sockaddr*)&si, sizeof(si));
	return sbytes;
}

#include <cstdio>
int recv_message(message& msg, const int sockfd, const sockaddr_in& si)
{
	uint8_t hbuf[MESSAGE_HEADER_SIZE];
	memset(hbuf, 0, MESSAGE_HEADER_SIZE);
	socklen_t addr_size = sizeof(si);

	// MSG_DONTWAIT
	int hbytes = recvfrom(sockfd, hbuf, MESSAGE_HEADER_SIZE, 0, (sockaddr*)&si, &addr_size);
	//printf("hbytes = %d\n", hbytes);
	//for (int i = 0; i < hbytes; i++)
	//	printf("%#4x ", hbuf[i]);
	//printf("\n");

	if (hbytes <= 0)
		return hbytes;

	memcpy(msg.b, hbuf, hbytes);

	if (hbytes < MESSAGE_HEADER_SIZE)
		return hbytes;

	uint8_t* bbuf = new uint8_t[msg.s.size + MESSAGE_HEADER_SIZE];  // does not have to be deleted;
	memset(bbuf, 0, msg.s.size);
	int bbytes = recvfrom(sockfd, bbuf, msg.s.size + MESSAGE_HEADER_SIZE, 0, (sockaddr*)&si, &addr_size);

	//printf("bbytes = %d\n", bbytes);
	//for (int i = 0; i < bbytes; i++)
	//	printf("%#4x ", bbuf[i]);
	//printf("\n");

	if (bbytes <= 0)
		return hbytes;

	if (msg.data)
		delete[] msg.data;

	msg.data = new uint8_t[msg.s.size];
	memcpy(msg.data, bbuf + MESSAGE_HEADER_SIZE, bbytes);

	return hbytes + bbytes;
}