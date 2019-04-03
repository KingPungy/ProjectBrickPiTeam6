#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "../cppserver/client.h"

int main(int argc, char** argv)
{
	int port = DEFAULT_PORT;
	char* addr = 0;
	
	if (argc > 1)
	{
		int argsize = strlen(argv[1]);
		int addrsize = argsize;
		int portsize = 0;
		
		//printf("argv[1](%d): %s\n", argsize, argv[1]);
		
		for (int i = 0; i < argsize; i++)
		{
			if (argv[1][i] == ':')
			{
				addrsize = i;
				break;
			}
			//printf("c[%d]: %c(%#x)\n", i, argv[1][i], argv[1][i]);
		}
		
		addr = new char[addrsize + 1];
		memcpy(addr, argv[1], addrsize);
		
		portsize = argsize - (addrsize + 1);
		
		if (portsize > 0)
		{
			char* portstring = new char[portsize + 1];
			memcpy(portstring, &argv[1][addrsize + 1], portsize);
			
			port = atoi(portstring);
			
			delete[] portstring;
		}
		
		printf("IPv4(%d): %s\nPORT(%d): %d\n", addrsize, addr, portsize, port);
	}
	else
	{
		printf("usage: %s IPv4[:PORT]\n", argv[0]);
		exit(0);
	}
	
	std::string ip_addr = std::string(addr);
	client c(ip_addr, port);

	union
	{
		uint8_t buf[4];
		uint32_t time;
		int16_t value;
		uint8_t type;
		uint8_t number;
	} u_input;

	union
	{
		uint8_t buf[3];
		int16_t value;
		uint8_t id;
	} u_send_data;
	
	
	int filed = open("/dev/input/js0", O_RDONLY);

	for (;;)
	{
		read(filed, u_input.buf, 4);
		u_send_data.value = u_input.val;
		u_send_data.id = u_input.number | (u_input.type << 4);
		c.send_input((void*)u_send_data.buf);


		//usleep(1000 * 1000);
		//printf("< ip: %s:%d, msg: ", addr, port);
		//scanf("%1024s", buf);
		//c.send_ping();
		//printf("send ping\n");
		
		//senderr = send_message_raw(sendmsg, sockfd, serv_addr);
		//printf("send message with id = %#x\n", sendmsg.id);
		//recverr = recv_message_raw(recvmsg, sockfd, serv_addr);
		//printf("received message with id = %#x\n", recvmsg.id);
		
		//chlen = sendto(sockfd, buf, 1024, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		//if (chlen == -1)
		//	printf("Failed to send data to: %s:%d\n", addr, port);
			
		//recvfrom(sockfd, buf, 1024, 0, (struct sockaddr*)&serv_addr, &addr_size);
		//printf("> ip: %s:%d, msg: %s\n", inet_ntoa(serv_addr.sin_addr), port, buf);
	}
	
	return 0;
}
