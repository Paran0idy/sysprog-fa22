#include <stdio.h>
#include <string.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <net/if_arp.h>
#include "packetProcess.h"

struct sockaddr_in source, dest;
int total = 0, tcp = 0, udp = 0, icmp = 0, igmp = 0, other = 0, arp_packet = 0;
int ipheader_len;

// print format of etherNet-header
void mac_header(unsigned char *buffer){
    struct ethhdr *eth = (struct ethhdr*)(buffer);
    printf("\nEthernet Header\n");
    printf("\t|-Source Address          : %.2x-%.2x-%.2x-%.2x-%.2x-%.2x\n",
           eth->h_source[0], eth->h_source[1], eth->h_source[2],
           eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    printf("\t|-Destination Address     : %.2x-%.2x-%.2x-%.2x-%.2x-%.2x\n",
           eth->h_dest[0], eth->h_dest[1], eth->h_dest[2],
           eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    printf("\t|-Protocol                : %d\n", ntohs(eth->h_proto));
}

// print format of ip-header
void ip_header(unsigned char *buffer){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    struct iphdr *ip = (struct  iphdr*)(buffer + sizeof (struct ethhdr));
    printf("\nIP Header\n");
    // detail
    struct in_addr src, dst;
    memcpy(&src, &ip->saddr, 4);
    memcpy(&dst, &ip->daddr, 4);

    printf("\t|-Source IP               : %s\n", inet_ntoa(src));
    printf("\t|-Destination IP          : %s\n", inet_ntoa(dst));
    printf("\t|-Version                 : %u\n", ip->version);
    printf("\t|-Internet Header Length  : %u DWORDS OR %u Bytes\n", ip->ihl, 4 * ip->ihl);
    printf("\t|-Total length            : %d Bytes\n", ntohs(ip->tot_len));
    printf("\t|-Identification          : %04x\n", ntohs(ip->id));
    printf("\t|-TTL                     : %d\n", ip->ttl);
    printf("\t|-Protocol                : %d\n", ip->protocol);
    printf("\t|-Header Checksum         : %04x\n", ntohs(ip->check));

    ipheader_len = sizeof (struct iphdr);
}

// print format of arp-header
void arp_header(unsigned char *buffer){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */


    struct ether_arp *arp = (struct ether_arp*)(buffer + sizeof (struct ethhdr));

    printf("\nARP Header\n");

    printf("\t|-Hardware Address        : %d\n", ntohs(arp->ea_hdr.ar_hrd));
    printf("\t|-Protocol Address        : %04x\n", ntohs(arp->ea_hdr.ar_pro));
    printf("\t|-Hardware Address Length : %u\n", arp->ea_hdr.ar_hln);
    printf("\t|-Protocol Address Length : %u\n", arp->ea_hdr.ar_pln);
    printf("\t|-ARP OPCODE              : %d\n", ntohs(arp->ea_hdr.ar_op));
    printf("\t|-Sender Hard Address     : %.2x-%.2x-%.2x-%.2x-%.2x-%.2x\n",
           arp->arp_sha[0], arp->arp_sha[1], arp->arp_sha[2],
           arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);
    printf("\t|-Target Hard Address     : %.2x-%.2x-%.2x-%.2x-%.2x-%.2x\n",
           arp->arp_tha[0], arp->arp_tha[1], arp->arp_tha[2],
           arp->arp_tha[3], arp->arp_tha[4], arp->arp_tha[5]);
    printf("\t|-Sender IP Address       : %u.%u.%u.%u\n",
           arp->arp_spa[0], arp->arp_spa[1], arp->arp_spa[2], arp->arp_spa[3]);
    printf("\t|-Target IP Address       : %u.%u.%u.%u\n",
           arp->arp_tpa[0], arp->arp_tpa[1], arp->arp_tpa[2], arp->arp_tpa[3]);
}

// print format of tcp-header
void tcp_header(unsigned char* buffer){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    struct tcphdr *tcp_hdr = (struct tcphdr*)(buffer + sizeof (struct ethhdr) + sizeof (struct iphdr));

    printf("\nTCP Header\n");

    printf("\t|-Source Port             : %d\n", ntohs(tcp_hdr->source));
    printf("\t|-Destination Port        : %d\n", ntohs(tcp_hdr->dest));
    printf("\t|-Seq Number              : %u\n", ntohl(tcp_hdr->seq));
    printf("\t|-Ack Number              : %u\n", ntohl(tcp_hdr->ack_seq));
    printf("\t|-Window                  : %d\n", ntohs(tcp_hdr->window));
    printf("\t|-Checksum                : %04x\n", ntohs(tcp_hdr->check));
    printf("\t|-Urgent Pointer          : %d\n", ntohs(tcp_hdr->urg_ptr));
}

// print format of udp-header
void udp_header(unsigned char* buffer){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    struct udphdr *udp_hdr = (struct udphdr*)(buffer + sizeof (struct ethhdr) + sizeof (struct iphdr));

    printf("\nUDP Header\n");

    printf("\t|-Source Port             : %d\n", ntohs(udp_hdr->source));
    printf("\t|-Destination Port        : %d\n", ntohs(udp_hdr->dest));
    printf("\t|-Checksum                : %04x\n", ntohs(udp_hdr->check));
    printf("\t|-Length                  : %d\n", ntohs(udp_hdr->len));
}

// print format of icmp-header
void icmp_header(unsigned char *buffer){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */

    struct icmphdr *icmp_hdr = (struct icmphdr*)buffer;

    printf("\nICMP Header\n");

    printf("\t|TYPE                     : %d\n", icmp_hdr->type);
    printf("\t|CODE                     : %d\n", icmp_hdr->code);
    printf("\t|CHECKSUM                 : %d\n", ntohs(icmp_hdr->checksum));
}

// print format of payload
void payload(unsigned char* buffer, int buffer_len){

    printf("\nData\n");
    for(int i = 0; i < buffer_len; i++){
        if(i != 0 && i % 16 == 0)
            printf("\n");
        printf(" %.2X ", buffer[i]);
    }

    printf("\n");
}

// print ICMP packet
void ICMP_printer(unsigned char *buffer, int buffer_len){
    printf("\n---------------------------ICMP Packet---------------------------");
    // print header
    mac_header(buffer);
    ip_header(buffer);
    icmp_header(buffer);
    // print packet payload
    unsigned char *data = (buffer + ipheader_len  + sizeof(struct ethhdr) + sizeof(struct icmphdr));
    int data_len = buffer_len - (ipheader_len  + sizeof(struct ethhdr) + sizeof(struct icmphdr));
    payload(data, data_len);
    printf("--------------------------------------------------------------\n\n\n");
}

// print ARP packet
void ARP_printer(unsigned char *buffer){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    printf("\n---------------------------ARP Packet---------------------------");

    mac_header(buffer);
    arp_header(buffer);

    printf("--------------------------------------------------------------\n\n\n");

}

// print TCP packet
void TCP_printer(unsigned char *buffer, int buffer_len){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */

    printf("\n---------------------------TCP Packet---------------------------");
    // print header
    mac_header(buffer);
    ip_header(buffer);
    tcp_header(buffer);
    // print packet payload
    unsigned char *data = (buffer + ipheader_len  + sizeof(struct ethhdr) + sizeof(struct tcphdr));
    int data_len = buffer_len - (ipheader_len  + sizeof(struct ethhdr) + sizeof(struct tcphdr));
    payload(data, data_len);
    printf("--------------------------------------------------------------\n\n\n");
}

// print UDP packet
void UDP_printer(unsigned char* buffer, int buffer_len){
    /*
     * Todo("exercise 2 : Complete the code of Step 1 correctly, and submit your source code.")
     */
    printf("\n---------------------------UDP Packet---------------------------");
    // print header
    mac_header(buffer);
    ip_header(buffer);
    udp_header(buffer);
    // print packet payload
    unsigned char *data = (buffer + ipheader_len  + sizeof(struct ethhdr) + sizeof(struct udphdr));
    int data_len = buffer_len - (ipheader_len  + sizeof(struct ethhdr) + sizeof(struct udphdr));
    payload(data, data_len);
    printf("--------------------------------------------------------------\n\n\n");
}

// packet process func
void packet_process(unsigned char* buffer, int buffer_len){
    ++total;
    struct ethhdr *eth = (struct ethhdr*)(buffer);
    switch((int) ntohs(eth->h_proto)){
        case 2048:{  // 0800 -> ip
            struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
            switch(ip->protocol){
                case 1:                                     // ICMP packet
                    ++icmp;
                    ICMP_printer(buffer, buffer_len);
                    break;

                case 2:
                    ++igmp;
                    break;

                case 6:                                     // TCP packet
                    ++tcp;
                    TCP_printer(buffer, buffer_len);         // print packet info
                    break;

                case 17:                                    // UDP packet
                    ++udp;
                    UDP_printer(buffer, buffer_len);          // print packet info
                    break;

                default:
                    ++other;
            }
            break;
        }

        case 2054:  // 0806 -> arp
            ++arp_packet;
            ARP_printer(buffer);            // print ARP packet info
            break;

        default:
            ++other;
    }

    printf("Result: [TCP : %d], [UDP : %d], [ARP : %d], [ICMP : %d], [IGMP : %d], [OTHER : %d], [TOTAL : %d]\n",
           tcp, udp, arp_packet, icmp, igmp, other, total);
}