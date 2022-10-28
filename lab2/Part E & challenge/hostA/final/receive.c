#include "protocol.h"

void own_mac_header(unsigned char *buffer){
    struct own_hdr *od = (struct own_hdr*)(buffer);

    printf("%.2x-%.2x-%.2x-%.2x-%.2x-%.2x -> %.2x-%.2x-%.2x-%.2x-%.2x-%.2x\n",
           od->src_mac[0], od->src_mac[1], od->src_mac[2],
           od->src_mac[3], od->src_mac[4], od->src_mac[5],
           od->dst_mac[0], od->dst_mac[1], od->dst_mac[2],
           od->dst_mac[3], od->dst_mac[4], od->dst_mac[5]);

    get_data(buffer);
}

void packet_print(unsigned char* buffer, int buffer_len){
    printf("\n---------------------------- Packet ----------------------------\n");
    for(int i = 0; i < buffer_len / 16; i++) {
        printf("\t");
        for (int j = 0; j < 16; j++)
            printf("%02X ", buffer[16 * i + j]);
        printf("\n");
    }
    printf("----------------------- Packet finished ------------------------\n\n");
}

void get_data(unsigned char *buffer){
    unsigned char *data = (unsigned char *)(buffer + sizeof (struct own_hdr));
    // print information
    printf("\thost A received : %s\n", data);
}

bool equal(unsigned char *dst, unsigned char *src){
    int i = 0;
    for(; i < 6; i++)
        if(dst[i] != src[i]) break;
    return i == 6;
}

bool filterByMacAddress(unsigned char *buffer, unsigned char mac_addr[], int type){
    struct own_hdr *od = (struct own_hdr *)(buffer);
    return type == 0 ? equal(od->src_mac, mac_addr) : equal(od->dst_mac, mac_addr);
}
