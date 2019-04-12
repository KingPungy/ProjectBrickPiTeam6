#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <algorithm>

#include "../include/client.h"

// different compile options for different methods of sending information or handling the xbox driver
#define OPTION 0

void exit_handler(int signo);

int main(int argc, char** argv)
{
	signal(SIGINT, exit_handler);
	int port = DEFAULT_PORT;
	char* addr = 0;
	
	// get the ip address from the arguments
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
	delete[] addr;
	client c(ip_addr, port);

#if OPTION == 0 || OPTION == 1
	#if OPTION == 1
	int pfd[2];
	if (pipe2(pfd, O_DIRECT | O_NONBLOCK) == -1)
	{
		fprintf(stderr, "Pipe failed!\n");
		return 1;
	}

	pid_t forkp;
	if ((forkp = fork()) < 0)
	{
		fprintf(stderr, "Fork failed!\n");
		return 1;
	}
	else if (forkp == 0)
	{
		char driver[] = "/usr/bin/xboxdrv";
		close(pfd[0]);
		if (dup2(pfd[1], STDOUT_FILENO) < 0)
		{
			fprintf(stderr, "dup2 error\n");
			return 1;
		}

		if (execl(driver, driver, "--detach-kernel-driver", "--quiet", (char*)0) < 0)
		{
			fprintf(stderr, "exec error\n");
			return 1;
		}
		close(pfd[1]);
		return 0;
	}
	else
	{
		close(pfd[1]);
	#endif

		union send_data
		{
			uint8_t buf[12];
			struct
			{
				int16_t X1;
				int16_t Y1;
				int16_t X2;
				int16_t Y2;
				bool du		: 1;
				bool dd		: 1;
				bool dl		: 1;
				bool dr		: 1;
				bool back	: 1;
				bool guide	: 1;
				bool start	: 1;
				bool TL		: 1;
				bool TR		: 1;
				bool A		: 1;
				bool B		: 1;
				bool X		: 1;
				bool Y		: 1;
				bool LB		: 1;
				bool RB		: 1;
				bool extra	: 1;
				uint8_t LT;
				uint8_t RT;
			} __attribute__((packed)) s;
		} __attribute__((packed));

		send_data u_send_data;
		send_data u_send_data_old;

		std::string inbuf;
	#if OPTION == 1
		char buf[1024];
	#endif
		std::map<std::string, int> datamap;

		for (unsigned int n = 0; ; n++)
		{
			datamap.clear();
	#if OPTION == 0
			getline(std::cin, inbuf);
	#elif OPTION == 1
			read(pfd[0], buf, 1024);
			
			size_t len = strlen(buf);
			//printf("got message! (%lu) %s\n", len, buf);

			if (len < 1)
			{
				usleep(10 * 1000);
				continue;
			}
			
			inbuf = std::string(buf);

			auto pend = inbuf.find('\n');
			if (pend >= inbuf.back())
			{
				usleep(10 * 1000);
				continue;
			}
			
			inbuf.erase(pend, inbuf.back());
	#endif
			
			inbuf.erase(std::remove(inbuf.begin(), inbuf.end(), ' '), inbuf.end());
			
			while (inbuf.size() > 0)
			{
				auto pinp = inbuf.find(':');
				if (pinp == inbuf.back())
					break;
				
				std::string inp = inbuf.substr(0, pinp);
				inbuf.erase(0, pinp + 1);
				
				size_t end;
				for (end = 0; end < inbuf.size(); end++)
					if (inbuf[end] > '9' || inbuf[end] < '0' && inbuf[end] != '-')
						break;
				
				std::string num = inbuf.substr(0, end);
				inbuf.erase(0, end);
				
				datamap.emplace(inp, atoi(num.c_str()));
			}
			
	#ifdef DEBUG
			for (const auto& item : datamap)
				printf("%s=%i,", item.first.c_str(), item.second);
			printf("\n");
	#endif
			
			u_send_data.s.X1 		= (uint16_t)	datamap["X1"];
			u_send_data.s.Y1 		= (uint16_t)	datamap["Y1"];
			u_send_data.s.X2 		= (uint16_t)	datamap["X2"];
			u_send_data.s.Y2 		= (uint16_t)	datamap["Y2"];
			u_send_data.s.du 		= (bool)		datamap["du"];
			u_send_data.s.dd 		= (bool)		datamap["dd"];
			u_send_data.s.dl 		= (bool)		datamap["dl"];
			u_send_data.s.dr 		= (bool)		datamap["dr"];
			u_send_data.s.back 		= (bool)		datamap["back"];
			u_send_data.s.guide 	= (bool)		datamap["guide"];
			u_send_data.s.start 	= (bool)		datamap["start"];
			u_send_data.s.TL 		= (bool)		datamap["TL"];
			u_send_data.s.TR 		= (bool)		datamap["TR"];
			u_send_data.s.A 		= (bool)		datamap["A"];
			u_send_data.s.B 		= (bool)		datamap["B"];
			u_send_data.s.X 		= (bool)		datamap["X"];
			u_send_data.s.Y 		= (bool)		datamap["Y"];
			u_send_data.s.LB		= (bool)		datamap["LB"];
			u_send_data.s.RB 		= (bool)		datamap["RB"];
			u_send_data.s.extra		= (bool)		false;
			u_send_data.s.LT 		= (uint8_t)		datamap["LT"];
			u_send_data.s.RT 		= (uint8_t)		datamap["RT"];
			
			bool changed = false;
			for (int i = 0; i < 12; i++)
				if (u_send_data.buf[i] != u_send_data_old.buf[i])
				{
					changed = true;
					break;
				}

			c.send_input_controller_btn_all(u_send_data.buf);
			
			u_send_data_old = u_send_data;

			printf("\r");
			for (int j = 0; j < 12; j++)
				printf("%#4x ", u_send_data.buf[j]);
			printf("\t%4d", changed);
			//printf("\n");
			fflush(stdout);
		}

	#if OPTION == 1
		close(pfd[0]);
	}
	#endif
#elif OPTION == 2
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
			c.send_input_controller_btn_change(u_send_data.buf);
			printf("sending input... (%d)\n", i);
			for (int j = 0; j < 3; j++)
				printf("%#x ", u_send_data.buf[j]);
			printf("\n");
		}
		else
		{
			printf("\rrbytes = %d (%d)", rbytes, i);
			fflush(stdout);
		}
	}
#endif

	return 0;
}

void exit_handler(int signo)
{
	switch (signo)
	{
		case SIGINT:
		{
			
			break;
		}
	}
	
	kill(0, signo);
	usleep(10 * 1000);
	exit(signo);
}