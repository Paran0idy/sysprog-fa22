#include "ARP_forge.h"
#include "method.h"

void send_arp(int *total_len){
    printf("arp packaging start ... \n");
    struct ether_arp *ah = (struct ether_arp *)(send_buff + sizeof (struct ethhdr));

    str2ip(src_ip, ah->arp_spa);
    str2ip(dst_ip, ah->arp_tpa);

    for(int i = 0; i < 6; i++){
        ah->arp_sha[i] = (unsigned char)(ifreq_mac.ifr_hwaddr.sa_data[i]);
        ah->arp_tha[i]   =  (unsigned char)0x00;
    }

    ah->ea_hdr.ar_hrd = htons(0x01);
    ah->ea_hdr.ar_pro = htons(ETH_P_IP);
    ah->ea_hdr.ar_hln = ETH_ALEN;
    ah->ea_hdr.ar_pln = 0x04;
    ah->ea_hdr.ar_op = htons(ARPOP_REQUEST);

    *total_len += sizeof (struct ether_arp);
    printf("arp packaging done ... \n");
}
