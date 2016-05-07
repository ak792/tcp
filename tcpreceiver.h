#ifndef TCP_H_   /* Include guard */
#define TCP_H_

#include "tcppacket.h"
#include "tcppacketarr.h"
#include "queue.h"
#include "queuemetadata.h"

#include "endhost.h"
#include "tcpsender.h"

void putPacketInReceiverBuffer(EndHost* currHost, TcpPacket* receivedTcpPacket);

TcpPacket* pollPacketsAndSendAck(EndHost* currHost);

TcpPacket* pollNextPacketFromReceiverBuffer(EndHost* currHost);

#endif // TCP_H_