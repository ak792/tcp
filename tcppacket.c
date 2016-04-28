#include <stdio.h>
#include "tcp.h"

void printTcpPacket(TcpPacket* tcpPacket) {
    printf("tcpPacket: {srcPort: %d, destPort: %d, SYN: %d, sequenceNumber: %d, ACK %d, ackNumber: %d, FIN: %d, windowSize: %d, sizeof(data): %d}\n",
           tcpPacket->srcPort, tcpPacket->destPort, tcpPacket->SYN, tcpPacket->sequenceNumber, tcpPacket->ACK, tcpPacket->ackNumber, tcpPacket->FIN,
           tcpPacket->windowSize, tcpPacket->dataSize);
}
