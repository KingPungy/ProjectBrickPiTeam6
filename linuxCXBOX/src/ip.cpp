#include "../include/ip.h"

#include <cstring>

#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int get_ipv4(in_addr* ina, const char* name)
{
	ifaddrs* ifa_ptr = 0;
	
	int error = getifaddrs(&ifa_ptr);
	if (error != 0)
		goto end;
	
	if (!name)
	{
		ina = &((sockaddr_in*)ifa_ptr->ifa_addr)->sin_addr;
		goto end;
	}
	
	for (ifaddrs* ifa = ifa_ptr; ifa != 0; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) // ipv4 check
		{
			if (strcmp(name, ifa->ifa_name) == 0)
			{
				ina->s_addr = ((sockaddr_in*)ifa->ifa_addr)->sin_addr.s_addr;
			
				//char addr_buf[INET_ADDRSTRLEN];
				//inet_ntop(AF_INET, ina, addr_buf, INET_ADDRSTRLEN);
				//printf("%s IP Address %s\n", ifa->ifa_name, addr_buf);
			}
		}
	}
	
	end:
	if (ifa_ptr != 0)
		freeifaddrs(ifa_ptr);
	return error;
}

int get_ipv4_str(in_addr* ina, char* ipv4, bool fill, const char* name)
{
	int err = 0;
	
	if (!ina)
		return (err = -1);
	
	if (fill)
	{
		err = get_ipv4(ina, name);
		if (err != 0)
			return err;
	}

	char addr_buf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, ina, addr_buf, INET_ADDRSTRLEN);
	strncpy(ipv4, addr_buf, INET_ADDRSTRLEN);
	
	return err;
}

int get_ipv6(in6_addr* ina, const char* name)
{
	ifaddrs* ifa_ptr = 0;
	
	int error = getifaddrs(&ifa_ptr);
	if (error != 0)
		goto end;
	
	if (!name)
	{
		ina = &((sockaddr_in6*)ifa_ptr->ifa_addr)->sin6_addr;
		goto end;
	}
	
	for (ifaddrs* ifa = ifa_ptr; ifa != 0; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET6) // ipv6 check
		{
			if (strcmp(name, ifa->ifa_name) == 0)
			{
				ina = &((sockaddr_in6*)ifa->ifa_addr)->sin6_addr;
			
				//char addr_buf[INET6_ADDRSTRLEN];
				//inet_ntop(AF_INET6, ina, addr_buf, INET6_ADDRSTRLEN);
				//printf("%s IP Address %s\n", ifa->ifa_name, addr_buf);
			}
		}
	}
	
	end:
	if (ifa_ptr != 0)
		freeifaddrs(ifa_ptr);
	return error;
}

int get_ipv6_str(in6_addr* ina, char* ipv6, bool fill, const char* name)
{
	int err = 0;
	
	if (!ina)
		return (err = -1);

	if (fill)
	{
		err = get_ipv6(ina, name);
		if (err != 0)
			return err;
	}
	
	char addr_buf[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET6, ina, addr_buf, INET6_ADDRSTRLEN);
	strncpy(ipv6, addr_buf, INET6_ADDRSTRLEN);
	
	return err;
}

int get_ip(void* sin, const char* name) // sin is type of sockaddr_in or sockaddr_in6
{
	int err = 0;
	if (!sin)
		err = -1;
	else if (((sockaddr_in*)sin)->sin_family == AF_INET) // ipv4 check
		err = get_ipv4(&((sockaddr_in*)sin)->sin_addr, name);
	else if (((sockaddr_in6*)sin)->sin6_family == AF_INET6) // ipv6 check
		err = get_ipv6(&((sockaddr_in6*)sin)->sin6_addr, name);

	return err;
}

int get_ip_str(void* sin, char* ip, bool fill, const char* name) // sin is type of sockaddr_in or sockaddr_in6
{
	int err = 0;
	if (!sin)
		err = -1;
	else if (((sockaddr_in*)sin)->sin_family == AF_INET) // ipv4 check
		err = get_ipv4_str(&((sockaddr_in*)sin)->sin_addr, ip, fill, name);
	else if (((sockaddr_in6*)sin)->sin6_family == AF_INET6) // ipv6 check
		err = get_ipv6_str(&((sockaddr_in6*)sin)->sin6_addr, ip, fill, name);

	return err;
}