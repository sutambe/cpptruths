#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char const *argv[]) {
  struct ifaddrs *ifaddr, *ifa;

  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs");
    return -1;
  }

  /* Walk through linked list, maintaining head pointer so we
     can free list later */
  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL ||
        //ifa->ifa_addr->sa_family != AF_PACKET) continue;
        ifa->ifa_addr->sa_family != AF_INET) continue;

    struct sockaddr_in *addr_in = (struct sockaddr_in *)ifa->ifa_addr;
    //struct sockaddr_in *addr_in = (struct sockaddr_in *)ifa->ifa_netmask;
    char *ip = inet_ntoa(addr_in->sin_addr);
    printf("interface %s is %s\n", ifa->ifa_name, ip);
  }

  freeifaddrs(ifaddr);
  return 0;
}
