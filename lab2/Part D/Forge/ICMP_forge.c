#include "ICMP_forge.h"
#include "method.h"

void send_icmp(int *total_len){
    printf("icmp packaging start ... \n");
    struct icmp *ih = (struct icmp *)(send_buff + sizeof(struct iphdr) + sizeof(struct ethhdr));

    ih->icmp_type = ICMP_ECHO;
    ih->icmp_code = 0;
    ih->icmp_cksum = htons(checksum((unsigned char *)(send_buff + sizeof (struct ethhdr) + sizeof (struct iphdr)), sizeof (struct icmp)));
    ih->icmp_id = 0;
    ih->icmp_seq = 0;

    *total_len += sizeof(struct icmp);
    printf("icmp packaging done ... \n");
}