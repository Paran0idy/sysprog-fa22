#include "UDP_forge.h"
#include "method.h"

void send_udp(int *total_len){
    printf("udp packaging start ... \n");
    struct udphdr *uh = (struct udphdr *)(send_buff + sizeof(struct iphdr) + sizeof(struct ethhdr));

    uh->source	= htons(23451);
    uh->dest	= htons(23452);
    uh->check	= 0;   //Many OSes ignore this field , so we do not calculate it.

    *total_len += sizeof(struct udphdr);
    uh->len		= htons((*total_len - sizeof(struct iphdr) - sizeof(struct ethhdr)));
    printf("udp packaging start ... \n");
}