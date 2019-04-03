#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include "../cppserver/ip.h"
#include "../cppserver/message.h"

#define PORT 7263

int main(int argc, char** argv)
{
	sockaddr_in si_server, si_client;
	memset(&si_server, 0, sizeof(si_server));
	
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(PORT);
	
	char ip_server[20];
	memset(ip_server, 0, 20);
	
	printf("server ip (%d) = %s\n", get_ip_str(&si_server, ip_server, "wlan0"), ip_server);
	//inet_aton("127.000.000.001", &si_server.sin_addr);
	//si_server.sin_addr = inet_addr("127.0.0.1");
	
	int sockfd = socket(si_server.sin_family, SOCK_DGRAM, 0);
	bind(sockfd, (sockaddr*)&si_server, sizeof(si_server));
	
	char ip_client[20];
	memset(ip_client, 0, 20);
	printf("client ip (%d) = %s\n", get_ip_str(&si_client, ip_client, "wlan0"), ip_client);
	
	uint8_t buf[MESSAGE_LEN_MAX];
	
	//strcpy(buf, "Hello, client!");

	message sendmsg { 0, 0, 0 };
	message recvmsg { 0, 0, 0 };
	int senderr = 0;
	int recverr = 0;
	
	for (;;)
	{
		recverr = recv_message(recvmsg, sockfd, si_client);
		switch (recvmsg.id)
		{
			case MESSAGE_ID_PING:
				sendmsg.id = MESSAGE_ID_PONG;
				sendmsg.size = 0;
				senderr = send_message(sendmsg, sockfd, si_client);
				break;
		}
		//recvfrom(sockfd, buf, MESSAGE_LEN_MAX, 0, (sockaddr*)&si_client, &addr_size);
		//printf("> ip: %s:%d, msg: %s\n", inet_ntoa(si_client.sin_addr), PORT, buf);
		
		//printf("< ip: %s:%d, msg: %s\n", inet_ntoa(si_client.sin_addr), PORT, buf);
		//sendto(sockfd, buf, 1024, 0, (sockaddr*)&si_client, sizeof(si_client));
	}
	
	return 0;
}
