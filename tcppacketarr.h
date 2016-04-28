#ifndef TCPPACKETARR_H_   /* Include guard */
#define TCPPACKETARR_H_

#include "tcppacket.h"


typedef struct {
    TcpPacket** arr;
    int *size;
    
} TcpPacketArr;

#endif // TCPPACKETARR_H_