#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include "../include/client.h"

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
		
		printf("IPv4[:port] = %s:%d\n", addr, port);
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
		uint8_t buf[8];
		struct
		{
			uint32_t time;
			int16_t value;
			uint8_t type;
			uint8_t number;
		} __attribute__((packed)) s;
	} __attribute__((packed)) u_input;

	union
	{
		uint8_t buf[3];
		struct
		{
			int16_t value;
			uint8_t id;
		} __attribute__((packed)) s;
	} __attribute__((packed)) u_send_data;
	
	
	int filed = open("/dev/input/js0", O_RDONLY);

	for (int i = 0; ; i++)
	{
		int rbytes = read(filed, u_input.buf, 8);
		if (rbytes > 0)
		{
			u_send_data.s.value = u_input.s.value;
			u_send_data.s.id = u_input.s.number | (u_input.s.type << 4);
			c.send_input((void*)u_send_data.buf);
			//printf("sending input... rbytes = %d (%d)\n", rbytes, i);
			//for (int j = 0; j < 3; j++)
			//	printf("%#x ", u_send_data.buf[j]);
			//printf("\n");
		}
		else
		{
			//printf("\rrbytes = %d (%d)", rbytes, i);
			//fflush(stdout);
		}
	}
	
	return 0;
}