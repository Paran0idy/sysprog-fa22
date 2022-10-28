#include "method.h"

// dst_mac
uint8_t dst_mac[6] = {0x00, 0x0c, 0x29, 0x62, 0x2d, 0x87};

void str2ip(char *src, unsigned char *dst){
    int idx = 0;
    while(*src != '\0') {
        int cnt = 0;
        char *cur = src;
        while(*src != '.' && *src != '\0') cnt++, src++;
        char *tmp = (char *) malloc(sizeof (char) * cnt);
        memcpy(tmp, cur, cnt);
        dst[idx++] = atoi(tmp);
        if(*src == '\0') break;
        src++;
    }
}

unsigned short checksum(unsigned char* buf, int size) {
    unsigned int checkSum = 0;
    for (int i = 0; i < size; i += 2) {
        unsigned short first = (unsigned short)buf[i] << 8;
        unsigned short second = (unsigned short)buf[i+1] & 0x00ff;
        checkSum += first + second;
    }
    while (1) {
        unsigned short c = (checkSum >> 16);
        if (c > 0) {
            checkSum = (checkSum << 16) >> 16;
            checkSum += c;
        } else {
            break;
        }
    }
    return ~checkSum;
}

void forge(void(*pro)(int *total_len), uint16_t mac_protocol, uint8_t ip_protocol){
    int sock_raw = sock_raw = socket(AF_PACKET,SOCK_RAW,ETH_P_IP);
    if(sock_raw == -1)
        printf("error in socket\n");

    send_buff = (unsigned char*)malloc(64);             // increase in case of large data.
    memset(send_buff, 0, 64);

    int total_len = 0;
    get_eth_index(sock_raw);                // interface number
    get_mac(sock_raw, mac_protocol, &total_len);

    if(mac_protocol == ETH_P_ARP)  pro(&total_len);
    else send_ip(pro, sock_raw, ip_protocol, &total_len);

    struct sockaddr_ll sadr_ll;
    sadr_ll.sll_ifindex = ifreq_idx.ifr_ifindex;
    sadr_ll.sll_halen   = ETH_ALEN;
    for(int i = 0; i < 6; i++)
        sadr_ll.sll_addr[i]  = dst_mac[i];

    int cnt = 1;
    printf("sending...\n");

    while(cnt-- > 0){
        int send_len = sendto(sock_raw, send_buff, 64, 0, (const struct sockaddr*)&sadr_ll, sizeof(struct sockaddr_ll));
        if(send_len < 0){
            printf("error in sending....sendlen = %d....errno = %d\n", send_len, errno);
            break;
        }
    }
    close(sock_raw);
}

void get_eth_index(int sock_raw){
    memset(&ifreq_idx, 0, sizeof(ifreq_idx));
    strncpy(ifreq_idx.ifr_name, "ens33", IFNAMSIZ - 1);

    if((ioctl(sock_raw, SIOCGIFINDEX, &ifreq_idx)) < 0)
        printf("error in index ioctl reading");

    printf("index = %d\n", ifreq_idx.ifr_ifindex);
}

void get_mac(int sock_raw, uint16_t protocol, int *total_len){
    memset(&ifreq_mac, 0, sizeof(ifreq_mac));
    strncpy(ifreq_mac.ifr_name, "ens33", IFNAMSIZ - 1);

    if((ioctl(sock_raw, SIOCGIFHWADDR, &ifreq_mac)) < 0)
        printf("error in SIOCGIFHWADDR ioctl reading");

    printf("ethernet packaging start ... \n");

    // write eth_header
    struct ethhdr *eth = (struct ethhdr *)(send_buff);
    for(int i = 0; i < 6; i++) {
        eth->h_source[i] = (unsigned char) (ifreq_mac.ifr_hwaddr.sa_data[i]);
        eth->h_dest[i] = dst_mac[i];
    }

    if(protocol == ETH_P_ARP)   // ARP_protocol
        for(int i = 0; i < 6; i++) eth->h_dest[i] = 0xff;

    eth->h_proto = htons(protocol);   //0x0800
    printf("ethernet packaging done.\n");
    *total_len += sizeof(struct ethhdr);
}

void send_ip(void(*func)(int *total_len), int sock_raw, uint8_t protocol, int *total_len){
    memset(&ifreq_ip, 0, sizeof(ifreq_ip));
    strncpy(ifreq_ip.ifr_name,"ens33",IFNAMSIZ-1);

    if(ioctl(sock_raw, SIOCGIFADDR, &ifreq_ip) < 0){
        printf("error in SIOCGIFADDR \n");
    }

    printf("ip packaging start ... \n");
    // write ip_header
    struct iphdr *iph = (struct iphdr*)(send_buff + sizeof(struct ethhdr));
    iph->ihl	    = sizeof (struct iphdr) / 4;
    iph->version    = 4;
    iph->tos	    = 0;
    iph->id		    = 0;
    iph->ttl	    = 64;
    iph->protocol	= protocol;
    iph->saddr	    = inet_addr(inet_ntoa((((struct sockaddr_in *)&(ifreq_ip.ifr_addr))->sin_addr)));
    iph->daddr	    = inet_addr(dst_ip); // put destination IP address

    *total_len += sizeof(struct iphdr);
    // call back
    func(total_len);

    iph->tot_len	= htons(*total_len - sizeof(struct ethhdr));
    iph->check	    = htons(checksum((unsigned char*)(send_buff + sizeof(struct ethhdr)), sizeof(struct iphdr)));
    printf("ip packaging done ... \n");
}