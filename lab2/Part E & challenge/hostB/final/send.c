#include "protocol.h"

void get_eth_index(){
    memset(&ifreq_i, 0, sizeof(ifreq_i));
    strncpy(ifreq_i.ifr_name,"ens33",IFNAMSIZ-1);
    if((ioctl(sen_sock, SIOCGIFINDEX, &ifreq_i)) < 0)
        printf("error in index ioctl reading");
}

void get_own_mac(unsigned char *buffer, unsigned char dst_mac[]){
    memset(&ifreq_c, 0, sizeof(ifreq_c));
    strncpy(ifreq_c.ifr_name,"ens33",IFNAMSIZ-1);
    if((ioctl(sen_sock, SIOCGIFHWADDR, &ifreq_c)) < 0)
        printf("error in SIOCGIFHWADDR ioctl reading");

    struct own_hdr *od = (struct own_hdr *)(buffer);
    for(int i = 0; i < 6; i++) {
        od->src_mac[i] = (unsigned char) (ifreq_c.ifr_hwaddr.sa_data[i]);
        od->dst_mac[i] = dst_mac[i];
    }

    total_len += sizeof(struct own_hdr);

    printf("%.2x-%.2x-%.2x-%.2x-%.2x-%.2x -> %.2x-%.2x-%.2x-%.2x-%.2x-%.2x\n",
           od->src_mac[0], od->src_mac[1], od->src_mac[2],
           od->src_mac[3], od->src_mac[4], od->src_mac[5],
           od->dst_mac[0], od->dst_mac[1], od->dst_mac[2],
           od->dst_mac[3], od->dst_mac[4], od->dst_mac[5]);

    send_data(buffer);
}

void send_data(unsigned char *buffer){
    // send information
    char *data = "a message from B";
    int len = total_len;
    memcpy(buffer + total_len, data, strlen(data) + 1);
    total_len += (int)strlen(data) + 1;
    printf("\thost B send : %s\n", buffer + len);
}