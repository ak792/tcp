#ifndef TCP_H_   /* Include guard */
#define TCP_H_

#include "tcppacket.h"
#include "tcppacketarr.h"
#include "queue.h"
#include "queuemetadata.h"

typedef struct EndHost EndHost;

struct EndHost {
    int hostId;
    char* hostName;
    int tcpPort;
    
    struct EndHost* destHost;
    
    int thisSideTerminatedConnection;
    int otherSideTerminatedConnection;
    
    int sequenceNumberOfFirstByteInNextPacket;
    int lastAckedSequenceNumber;
    
    TcpPacket** senderBuffer;
    QueueMetadata* senderBufferQMD;
    
    TcpPacket** receiverBuffer;
    QueueMetadata* receiverBufferQMD;
    
    int lastAdvertisedWindowFromReceiver;
    
};

void doSend(EndHost* srcHost, TcpPacket* tcpPacket);

void sendTcpPacket(EndHost* currHost, TcpPacket* packet);

TcpPacketArr* sendPacketsFromSenderBuffer(EndHost* currHost);

void sendData(EndHost* alice, EndHost* bob, int data);

TcpPacket* pollPacketsAndSendAck(EndHost* currHost);

TcpPacket* establishTcpConnection(EndHost* srcHost, EndHost* destHost);

TcpPacket* terminateTcpConnection(EndHost* srcHost, EndHost* destHost);

TcpPacket* pollNextPacketFromReceiverBuffer(EndHost* currHost);

void putPacketInReceiverBuffer(EndHost* currHost, TcpPacket* receivedTcpPacket);

int putPacketInSenderBuffer(EndHost* currHost, TcpPacket* tcpPacket);



#endif // TCP_H_