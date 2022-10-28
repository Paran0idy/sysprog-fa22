#include <stdbool.h>
#include <stdio.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include "filter.h"
#include <netinet/tcp.h>
#include <netinet/udp.h>

void mac2str(char *mac, unsigned char *str){
    int num = 0;
    for(unsigned char *p = str; p != str + 6; p++){
        num = *mac > '9' ? (*mac - 'a' + 10) : (*mac - '0');
        *p |= num * 16;
        mac++;
        num = *mac > '9' ? (*mac - 'a' + 10) : (*mac - '0');
        mac++;
        *p |= num;
        mac++;
    }
    str[6] = '\0';
}

bool equal(unsigned char *dst, unsigned char *src){
    int cnt = 0;
    while(src != NULL && *dst == *src)
        dst++, src++, cnt++;
    return cnt == 6;
}

bool filterByIpAddress(unsigned char *buffer, char *ip_addr, int type){
    struct sockaddr_in source, dest;
    struct ethhdr *eth = (struct ethhdr *)(buffer);
    // if the type of packet is not IP.
    if((int)ntohs(eth->h_proto) != 2048)
        return false;

    struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ip->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ip->daddr;

    char *source_ip, *dest_ip;
    char temp[20];
    source_ip = inet_ntoa(source.sin_addr);
    strcpy(temp, source_ip);
    dest_ip = inet_ntoa(dest.sin_addr);

    return type == 0 ? !strcmp(temp, ip_addr) : !strcmp(dest_ip, ip_addr);

//    if(type == 0){          // filter by source address
//        if (!strcmp(temp,ip_addr)){
//            return true;
//        }else{
//            return false;
//        }
//    }else{                  // filter by distination address
//        if(!strcmp(dest_ip,ip_addr)){
//            return true;
//        }else{
//            return false;
//        }
//    }
}

bool filterByMacAddress(unsigned char *buffer, char *mac_addr, int type){
    /*
     * Todo("exercise 3 : Complete the code of Step 2 correctly, and submit your source code.")
     */

    struct ethhdr *eth = (struct ethhdr *)(buffer);
    // mac2str
    unsigned char str[7]  ={0};
    mac2str(mac_addr, str);

    return type == 0 ? equal(eth->h_source, str) : equal(eth->h_dest, str);
}

bool filterByProtocol(unsigned char* buffer, int type){
    /*
     * Todo("exercise 3 : Complete the code of Step 2 correctly, and submit your source code.")
     */

    struct ethhdr *eth = (struct ethhdr*)(buffer);
    int protocol = ntohs(eth->h_proto);
    if(protocol == 2048){                                           // IP
        struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
        if(type == 2 && ip->protocol == 1) return true;             // ICMP
        if(type == 3 && ip->protocol == 6) return true;             // TCP
        if(type == 4 && ip->protocol == 17) return true;            // UDP
    }
    else if(protocol == 2054 && type == 1)                          // ARP
        return true;
    return false;
}

bool filterByPort(unsigned char *buffer, unsigned short port, int type){
    /*
     * Todo("exercise 3 : Complete the code of Step 2 correctly, and submit your source code.")
     */
    struct ethhdr *eth = (struct ethhdr *)(buffer);

    if((int)ntohs(eth->h_proto) != 2048) // ip_protocol
        return false;

    struct iphdr *ip = (struct iphdr *)(buffer + sizeof (struct ethhdr));

    if (ip->protocol == 6) {
        struct tcphdr *p = (struct tcphdr *) (buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
        return type == 0 ? p->source == port : p->dest == port;
    }
    else if(ip->protocol == 17){
        struct udphdr *p = (struct udphdr *) (buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
        return type == 0 ? (int)ntohs(p->source) == port : (int)ntohs(p->dest) == port;
    }
    return false;
}