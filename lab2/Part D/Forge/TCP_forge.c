#include "TCP_forge.h"
#include "method.h"

void send_tcp(int *total_len){
    struct tcphdr *th = (struct tcphdr *)(send_buff + sizeof(struct iphdr) + sizeof(struct ethhdr));

    printf("tcp packaging start ... \n");
    //write udp_header
    th->th_sport	= htons(2333);
    th->th_dport	= htons(80);
    th->th_seq      = htons(0);
    th->th_ack      = htons(0);
    th->th_off      = sizeof (struct tcphdr) / 4;
    th->th_flags    = TH_SYN;
    th->th_win      = htons(4096);

    // define pseudo header for tcp checksum
    struct psd_hdr psd;
    psd.src_ipaddr = inet_addr(src_ip);
    psd.dst_ipaddr = inet_addr(dst_ip);
    psd.nop = 0;
    psd.protocol = 6;
    psd.tcp_len = htons(sizeof (struct tcphdr));

    unsigned char cur[1000];

    memcpy(cur, &psd, sizeof(struct psd_hdr));
    memcpy(cur + sizeof (struct psd_hdr), th, sizeof(struct tcphdr));
    th->th_sum = htons(checksum(cur, sizeof(struct tcphdr) + sizeof (struct psd_hdr)));

    *total_len += sizeof(struct tcphdr);
    printf("tcp packaging done ... \n");
}