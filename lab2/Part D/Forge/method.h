#ifndef FORGE_METHOD_H
#define FORGE_METHOD_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <linux/if_packet.h>
#include <arpa/inet.h>

// for each ioctl keep diffrent ifreq structure otherwise error may come in sending(sendto )
struct ifreq ifreq_mac, ifreq_idx, ifreq_ip;
unsigned char *send_buff;

// ip addr
#define src_ip "192.168.83.128"
#define dst_ip "192.168.83.129"

// function declaration
void get_eth_index(int sock_raw);
void get_mac(int sock_raw, uint16_t protocol, int *total_len);

void str2ip(char *src, unsigned char *dst);
unsigned short checksum(unsigned char *buf, int size);

void forge(void(*pro)(int *total_len), uint16_t mac_protocol, uint8_t ip_protocol);     // forge template
void send_ip(void(*func)(int *total_len), int sock_raw, uint8_t protocol, int *total_len);

#endif //FORGE_METHOD_H