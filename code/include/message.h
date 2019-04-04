#ifndef MESSAGE_H
#define MESSAGE_H

#include <netinet/in.h>

#define MESSAGE_HEADER_SIZE	3 + sizeof(time_t)
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
	union // header
	{
		uint8_t b[MESSAGE_HEADER_SIZE];
		struct
		{
			uint8_t		id		= 0;	// 1 byte
			time_t		time	= 0;	// 
			uint16_t	size	= 0;	// 2 bytes, size of data
		} s = { 0 };
	} u = { 0 };
	uint8_t*	data	= 0;	// maximum size of MESSAGE_SIZE_MAX
};

int send_message(const message& msg, const int sockfd, const sockaddr_in& si);
int recv_message(message& msg, const int sockfd, const sockaddr_in& si);

#endif // MESSAGE_H
