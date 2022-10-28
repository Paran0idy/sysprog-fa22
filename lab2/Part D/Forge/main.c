#include "method.h"
#include "TCP_forge.h"
#include "UDP_forge.h"
#include "ICMP_forge.h"
#include "ARP_forge.h"

int main() {
    forge(send_udp, ETH_P_IP, IPPROTO_UDP);
    sleep(2);
    forge(send_tcp, ETH_P_IP, IPPROTO_TCP);
    sleep(2);
    forge(send_icmp, ETH_P_IP, IPPROTO_ICMP);
    sleep(2);
    forge(send_arp, ETH_P_ARP, 0);
    return 0;
}