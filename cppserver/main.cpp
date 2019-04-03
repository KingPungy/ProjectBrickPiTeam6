#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include "server.h"

int main(int argc, char** argv)
{
	server s(DEFAULT_PORT);
	printf("ip: %s\n", s.get_ip().c_str());
	
	for (;;)
	{
		s.wait_msg();
		const message& msg = s.get_msg();
		printf("message received: id=%d\n", msg.id);
		
		//recverr = recv_message_raw(recvmsg, sockfd, si_client);
		//printf("client ip (%d) = %s\n", get_ip_str(&si_client, ip_client, false), ip_client);
		//printf("received message %#4x\n", recvmsg.id);
		
		//switch (recvmsg.id)
		//{
		//	case MESSAGE_ID_PING:
		///		sendmsg.id = MESSAGE_ID_RECV;
		///		sendmsg.size = 0;
		//		senderr = send_message_raw(sendmsg, sockfd, si_client);
		///		printf("send message %#4x\n", sendmsg.id);
		//		break;
		//}/
		//recvfrom(sockfd, buf, MESSAGE_LEN_MAX, 0, (sockaddr*)&si_client, &addr_size);
		//printf("> ip: %s:%d, msg: %s\n", inet_ntoa(si_client.sin_addr), PORT, buf);
		
		//printf("< ip: %s:%d, msg: %s\n", inet_ntoa(si_client.sin_addr), PORT, buf);
		//sendto(sockfd, buf, 1024, 0, (sockaddr*)&si_client, sizeof(si_client));
	}
	
	return 0;
}
