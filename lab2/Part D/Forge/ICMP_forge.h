#ifndef FORGE_ICMP_FORGE_H
#define FORGE_ICMP_FORGE_H

// define icmp union
#define icmp_id         icmp_hun.ih_idseq.icd_id
#define icmp_seq        icmp_hun.ih_idseq.icd_seq

void send_icmp(int *total_len);

#endif //FORGE_ICMP_FORGE_H
