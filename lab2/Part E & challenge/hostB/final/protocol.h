#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <linux/if_packet.h>
#include <arpa/inet.h>

// for each ioctl keep diffrent ifreq structure otherwise error may come in sending(sendto )
struct ifreq ifreq_c, ifreq_i;
int sen_sock;
int total_len;

#define src_ip "192.168.83.128"
#define dst_ip "192.168.83.129"

struct own_hdr{
    unsigned char dst_mac[6];
    unsigned char src_mac[6];
};

// send
void get_eth_index();
void get_own_mac(unsigned char *buffer, unsigned char dst_mac[]);
void send_data(unsigned char *buffer);

// receive
void packet_print(unsigned char* buffer, int buffer_len);
void own_mac_header(unsigned char *buffer);
bool filterByMacAddress(unsigned char *buffer, unsigned char mac_addr[], int type);
bool equal(unsigned char *dst, unsigned char *src);
void get_data(unsigned char *buffer);

#endif //PROTOCOL_H