#include "../include/message.h"

#include <cstring>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctime>

#define MESSAGE_SIZE_MAX	1013
#define MESSAGE_LEN_MAX		1024

#define MESSAGE_ID_ERR		0x0000
#define MESSAGE_ID_PING		0x0001
#define MESSAGE_ID_RECV		0x0002
#define MESSAGE_ID_INPUT	0x0003

#define MESSAGE_ID_MAX		0xffff

#define DEFAULT_PORT 7263

struct message
{
	uint8_t		id		= 0;	// 1 byte
	time_t		time	= 0;	// 8 bytes
	uint16_t	size	= 0;	// 2 bytes, size of data
	uint8_t*	data	= 0;	// maximum size of MESSAGE_SIZE_MAX
};

extern int send_message_raw(const message& msg, const int sockfd, const sockaddr_in& si_client)
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
		uint8_t b[8];
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
	for (int i = 0; i < 8; i++)
		buf[i + offset] = u_size.b[i];
	offset += 8;
	
	memcpy(buf + offset, msg.data, msg.size);
	sendto(sockfd, buf, MESSAGE_LEN_MAX, 0, (struct sockaddr*)&si_client, sizeof(si_client));
}

extern int recv_message_raw(message& msg, const int sockfd, const sockaddr_in& si_client)
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
		uint8_t b[8];
		time_t t;
	} u_time;
	int offset = 0;
	msg.id = buf[offset++];
	for (int i = 0; i < 2; i++)
		u_size.b[i] = buf[i + offset];
	msg.size = u_size.i;
	offset += 2;
	for (int i = 0; i < 8; i++)
		u_size.b[i] = buf[i + offset];
	offset += 8;
	msg.time = u_time.t;
	memcpy(msg.data, buf + offset, msg.size);
}
