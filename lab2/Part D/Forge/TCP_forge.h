#ifndef FORGE_TCP_FORGE_H
#define FORGE_TCP_FORGE_H

// define pseudo header for tcp checksum
struct psd_hdr{
    unsigned int src_ipaddr;
    unsigned int dst_ipaddr;
    unsigned char nop;
    unsigned char protocol;
    unsigned short tcp_len;
};

void send_tcp(int *total_len);




#endif //FORGE_TCP_FORGE_H
