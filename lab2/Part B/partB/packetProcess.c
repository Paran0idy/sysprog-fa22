#include <stdio.h>
#include "packetProcess.h"

void packet_print(unsigned char* buffer, int buffer_len){
    printf("\n---------------------------- Packet ----------------------------\n");

    /*
     * Todo("Exercise 1: complete the packet_print function.")
     */

    for(int i = 0; i < buffer_len / 16; i++) {
        for (int j = 0; j < 16; j++)
            printf("%02X ", buffer[16 * i + j]);
        printf("\n");
    }

    printf("\n----------------------- Packet finished ------------------------");
}
