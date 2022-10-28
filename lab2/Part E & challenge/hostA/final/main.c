#include "protocol.h"

int main() {
    /*------------------send-------------------------*/
    // create a sen socket
    sen_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sen_sock == -1)
        printf("error in socket\n");

    unsigned char *sen_bf = (unsigned char*)malloc(64); // increase in case of large data.
    memset(sen_bf, 0, 64);

    unsigned char dst_mac[] = {0x00, 0x0c, 0x29, 0x62, 0x2d, 0x87};

    get_eth_index();                // interface number
    get_own_mac(sen_bf, dst_mac);

    struct sockaddr_ll sadr_ll;
    sadr_ll.sll_ifindex = ifreq_i.ifr_ifindex;
    sadr_ll.sll_halen   = ETH_ALEN;
    for(int i = 0; i < 6; i++) sadr_ll.sll_addr[i]  = dst_mac[i];

    int cnt = 1;

    /*------------------receive-------------------------*/
    int PACKET_LEN = 16000;
    int PACKET_AMOUNT = 1000;
    struct sockaddr saddr;

    // create a rec socket
    int rec_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(rec_sock < 0){
        printf("error in creating socket.\n");
        return -1;
    }

    // turn on the promiscuous mode
    struct packet_mreq mr;
    mr.mr_type = PACKET_MR_PROMISC;
    setsockopt(rec_sock, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr));

    while(cnt-- > 0){
        // send
        int send_len = (int)sendto(sen_sock, sen_bf, 64, 0, (const struct sockaddr*)&sadr_ll, sizeof(struct sockaddr_ll));
        if(send_len < 0){
            printf("error in sending....sendlen = %d....errno = %d\n", send_len, errno);
            break;
        }
        // receive
        while(1) {
            unsigned char *rec_bf = (unsigned char *) malloc(PACKET_LEN);
            memset(rec_bf, 0, PACKET_LEN);
            int saddr_len = sizeof saddr;
            int data_size = (int) recvfrom(rec_sock, rec_bf, PACKET_LEN, 0, &saddr, (socklen_t *) &saddr_len);
            if (data_size > 0) {
                if (filterByMacAddress(rec_bf, dst_mac, 0)) {   // received filter packet
                    own_mac_header(rec_bf);
                    break;
                }
            } else {
                printf("error in recvfrom func\n");
                return -1;
            }
        }
    }
    close(sen_sock);
    close(rec_sock);
}