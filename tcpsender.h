#ifndef TCPSENDER_H_   /* Include guard */
#define TCPSENDER_H_

#include "endhost.h"
#include "tcppacketarr.h"

void doSend(EndHost* srcHost, TcpPacket* tcpPacket);

void sendTcpPacket(EndHost* currHost, TcpPacket* packet);

int putPacketInSenderBuffer(EndHost* currHost, TcpPacket* tcpPacket);

TcpPacketArr* sendPacketsFromSenderBuffer(EndHost* currHost);

void sendData(EndHost* alice, EndHost* bob, int data);


TcpPacket* establishTcpConnection(EndHost* srcHost, EndHost* destHost);

TcpPacket* terminateTcpConnection(EndHost* srcHost, EndHost* destHost);

#endif //TCPSENDER_H_