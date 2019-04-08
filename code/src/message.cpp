#include "../include/message.h"

#include <cstring>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int send_message(const message& msg, const int sockfd, const sockaddr_in& si)
{
	uint8_t hbuf[MESSAGE_LEN_MAX];
	memset(hbuf, 0, MESSAGE_LEN_MAX);

	memcpy(hbuf, msg.b, MESSAGE_HEADER_SIZE);
	memcpy(hbuf + MESSAGE_HEADER_SIZE, msg.data, msg.s.size);
	int sbytes = sendto(sockfd, hbuf, MESSAGE_LEN_MAX, 0, (sockaddr*)&si, sizeof(si));
	return sbytes;
}

int recv_message(message& msg, const int sockfd, const sockaddr_in& si)
{
	uint8_t hbuf[MESSAGE_HEADER_SIZE];
	memset(hbuf, 0, MESSAGE_HEADER_SIZE);
	socklen_t addr_size = sizeof(si);

	// MSG_DONTWAIT
	//printf("receiving 1\n");
	int hbytes = recvfrom(sockfd, hbuf, MESSAGE_HEADER_SIZE, 0, (sockaddr*)&si, &addr_size);
	//printf("end receiving 1\n");
	//printf("hbytes = %d\n", hbytes);
	//for (int i = 0; i < hbytes; i++)
	//	printf("%#4x ", hbuf[i]);
	//printf("\n");

	if (hbytes <= 0)
		return hbytes;

	memcpy(msg.b, hbuf, hbytes);

	if (hbytes < MESSAGE_HEADER_SIZE)
		return hbytes;

	//printf("id: %d, size: %d\n", msg.s.id, msg.s.size);

	if (msg.s.id == MESSAGE_ID_ERR)
		return -1;

	uint8_t* bbuf = new uint8_t[msg.s.size + MESSAGE_HEADER_SIZE];
	memset(bbuf, 0, msg.s.size);
	//printf("receiving 2\n");
	int bbytes = recvfrom(sockfd, bbuf, msg.s.size + MESSAGE_HEADER_SIZE, 0, (sockaddr*)&si, &addr_size);
	//printf("end receiving 2\n");

	//printf("bbytes = %d\n", bbytes);
	//for (int i = 0; i < bbytes; i++)
	//	printf("%#4x ", bbuf[i]);
	
	//printf("\n"); 

	if (bbytes <= 0)
		return hbytes;
	if (msg.data)
	{
		//printf("deleting msg.data\n");
		delete[] msg.data;
		//printf("deleted msg.data\n");
	}

	//printf("allocating msg.data[msg.s.size]\n");
	msg.data = new uint8_t[msg.s.size];
	//printf("allocated msg.data[msg.s.size]\n");
	//printf("copying bbuf + headersize to msg.data\n");
	memcpy(msg.data, bbuf + MESSAGE_HEADER_SIZE, bbytes);
	//printf("copyied bbuf + headersize to msg.data\n");
	//printf("deleting bbuf\n");
	delete[] bbuf;
	//printf("deleted bbuf\n");
	
	//printf("id: %d, size: %d\n", msg.s.id, msg.s.size);

	return hbytes + bbytes;
}