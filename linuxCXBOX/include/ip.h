#ifndef IP_H
#define IP_H

#include <netinet/in.h>

// generic methods for getting ip adresses from an ip adress and an interface name

int get_ipv4(in_addr* ina, const char* name = 0);
int get_ipv4_str(in_addr* ina, char* ipv4, bool fill = true, const char* name = 0);
int get_ipv6(in6_addr* ina, const char* name);
int get_ipv6_str(in6_addr* ina, char* ipv6, bool fill = true, const char* name = 0);
int get_ip(void* sin, const char* name = 0); // sin is type of sockaddr_in or sockaddr_in6
int get_ip_str(void* sin, char* ip, bool fill = true, const char* name = 0); // sin is type of sockaddr_in or sockaddr_in6

#endif // IP_H
