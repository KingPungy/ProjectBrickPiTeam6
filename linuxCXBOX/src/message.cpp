#include "../include/message.h"

#include <cstring>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int send_message(const message& msg, const int sockfd, const sockaddr_in& si)
{
	// create a buffer for the data to be send
	uint8_t* sbuf = new uint8_t[MESSAGE_LEN_MAX + msg.s.size];
	// set the buffer to zeros
	memset(sbuf, 0, MESSAGE_LEN_MAX + msg.s.size);

	// copy the header into the buffer
	memcpy(sbuf, msg.b, MESSAGE_HEADER_SIZE);
	// copy the body into the buffer
	memcpy(sbuf + MESSAGE_HEADER_SIZE, msg.data, msg.s.size);
	// send the data
	int sbytes = sendto(sockfd, sbuf, MESSAGE_HEADER_SIZE + msg.s.size, 0, (sockaddr*)&si, sizeof(si));
	// delete the temporary buffer
	delete[] sbuf;

	// return send bytes
	return sbytes;
}

int recv_message(message& msg, const int sockfd, const sockaddr_in& si)
{
	// buffer where the header of the message is stored
	uint8_t hbuf[MESSAGE_HEADER_SIZE];
	// set buffer to all zeros
	memset(hbuf, 0, MESSAGE_HEADER_SIZE);
	// recvfrom needs a pointer to a socklen_t
	socklen_t addr_size = sizeof(si);

	// get the header data
	int hbytes = recvfrom(sockfd, hbuf, MESSAGE_HEADER_SIZE, 0, (sockaddr*)&si, &addr_size);

	// something went wrong or there is no waiting data
	if (hbytes <= 0)
		return hbytes;

	// copy received data to the message header
	memcpy(msg.b, hbuf, hbytes);

	// the header is not complete
	if (hbytes < MESSAGE_HEADER_SIZE)
		return hbytes;

	// something went wrong
	if (msg.s.id == MESSAGE_ID_ERR)
		return -1;

	// buffer for the body of the message
	uint8_t* bbuf = new uint8_t[MESSAGE_HEADER_SIZE + msg.s.size];
	// set the buffer to all zeros
	memset(bbuf, 0, MESSAGE_HEADER_SIZE + msg.s.size);

	// get the body data
	int bbytes = recvfrom(sockfd, bbuf, MESSAGE_HEADER_SIZE + msg.s.size, 0, (sockaddr*)&si, &addr_size);

	// something went wrong or there is no data
	if (bbytes <= 0)
		return hbytes;
	if (msg.data)
	{
		// if there is pointer to the message data delete it because we are going to make a new one
		delete[] msg.data;
	}

	// make a new message data pointer
	msg.data = new uint8_t[msg.s.size];
	// set the message data to all zeros
	memset(msg.data, 0, msg.s.size);

	// copy message body data to the message data pointer with the header offset because for some reason it gets received aswell. -> maybe look into why
	memcpy(msg.data, bbuf + MESSAGE_HEADER_SIZE, msg.s.size);

	// delete temporary body buffer
	delete[] bbuf;

	// return the number of bytes received (bbytes is header size too large)
	return hbytes + bbytes - MESSAGE_HEADER_SIZE;
}