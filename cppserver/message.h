#ifndef MESSAGE_H
#define MESSAGE_H

#include <netinet/in.h>

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

int send_message_raw(const message& msg, const int sockfd, const sockaddr_in& si_client);
int recv_message_raw(message& msg, const int sockfd, const sockaddr_in& si_client);

#endif // MESSAGE_H
