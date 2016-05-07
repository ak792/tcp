#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "endhost.h"
#include "tcppacket.h"
#include "tcppacketarr.h"
#include "queue.h"
#include "queuemetadata.h"

#include "network.h"
#include "tcpisngenerator.h"

int putPacketInSenderBuffer(EndHost* currHost, TcpPacket* tcpPacket) {
    return enqueue(currHost->senderBuffer, currHost->senderBufferQMD, tcpPacket);
}

void doSend(EndHost* srcHost, TcpPacket* tcpPacket) {
    printf("%s sending ", srcHost->hostName);
    printTcpPacket(tcpPacket);
    
    sendOverNetwork(srcHost->destHost, tcpPacket);
}

TcpPacketArr* sendPacketsFromSenderBuffer(EndHost* currHost) {
    int numItemsToSend;
    if (currHost->senderBufferQMD->numItems <= currHost->lastAdvertisedWindowFromReceiver) {
        numItemsToSend = currHost->senderBufferQMD->numItems;
    } else {
        numItemsToSend = currHost->lastAdvertisedWindowFromReceiver;
    }
    
    TcpPacketArr* polledPackets = malloc(sizeof(TcpPacketArr));
    polledPackets->arr = malloc(numItemsToSend * sizeof(TcpPacket*));
    polledPackets->size = &numItemsToSend;
    
    for (int i = 0; i < numItemsToSend; i++) {
        //TODO: shouldn't actually dequeue until get an ack (retransmission timeout
        TcpPacket* polledTcpPacket = dequeueMin(currHost->senderBuffer, currHost->senderBufferQMD);
        doSend(currHost, polledTcpPacket);
        
        polledPackets->arr[i] = polledTcpPacket;
    }
    
    return polledPackets;
}

void sendTcpPacket(EndHost* currHost, TcpPacket* tcpPacket) {
    int success = putPacketInSenderBuffer(currHost, tcpPacket);
    assert (success);
    
    TcpPacketArr* packets = sendPacketsFromSenderBuffer(currHost);
    free(packets->arr);
    free(packets);
}

void sendData(EndHost* alice, EndHost* bob, int data) {
    int sizeOfData = 4; // data is 4 bytes
    
    int currSequenceNumber = alice->sequenceNumberOfFirstByteInNextPacket;
    alice->sequenceNumberOfFirstByteInNextPacket += sizeOfData;
    
    TcpPacket* tcpPacket = malloc(sizeof(TcpPacket));
    tcpPacket->srcPort = alice->tcpPort;
    tcpPacket->destPort = bob->tcpPort;
    tcpPacket->SYN = 0; // there is already a connection
    tcpPacket->sequenceNumber = currSequenceNumber;
    tcpPacket->ACK = 1;
    tcpPacket->ackNumber = alice->lastAckedSequenceNumber;
    tcpPacket->dataOffset = 5;
    tcpPacket->FIN = 0;
    tcpPacket->windowSize = alice->receiverBufferQMD->maxNumItems - alice->receiverBufferQMD->numItems;
    tcpPacket->data = data;
    tcpPacket->dataSize = sizeOfData;
    tcpPacket->NS = 0;
    tcpPacket->CWR = 0;
    tcpPacket->ECE = 0;
    tcpPacket->URG = 0;
    tcpPacket->PSH = 0;
    tcpPacket->RST = 0;
    tcpPacket->urgentPtr = 0;
    
    int success = putPacketInSenderBuffer(alice, tcpPacket);
    assert (success);
    
    TcpPacketArr* packets = sendPacketsFromSenderBuffer(alice);
    free(packets->arr);
    free(packets);
}


// sends SYN message
TcpPacket* establishTcpConnection(EndHost* srcHost, EndHost* destHost) {
    int initialSequenceNumber = generateInitialSequenceNumber(srcHost);
    srcHost->sequenceNumberOfFirstByteInNextPacket = initialSequenceNumber + 1;
    
    int newWindowSize = srcHost->receiverBufferQMD->maxNumItems - srcHost->receiverBufferQMD->numItems;
    
    TcpPacket* synPacket = malloc(sizeof(TcpPacket));
    synPacket->srcPort = srcHost->tcpPort;
    synPacket->destPort = destHost->tcpPort;
    synPacket->SYN = 1; // there is already a connection
    synPacket->sequenceNumber = initialSequenceNumber;
    synPacket->ACK = 0;
    synPacket->ackNumber = 0;
    synPacket->dataOffset = 5;
    synPacket->FIN = 0;
    synPacket->windowSize = newWindowSize;
    synPacket->data = 0;
    synPacket->dataSize = 0;
    synPacket->NS = 0;
    synPacket->CWR = 0;
    synPacket->ECE = 0;
    synPacket->URG = 0;
    synPacket->PSH = 0;
    synPacket->RST = 0;
    synPacket->urgentPtr = 0;
    
    sendTcpPacket(srcHost, synPacket);
    
    return synPacket;
}

// sends FIN message
TcpPacket* terminateTcpConnection(EndHost* srcHost, EndHost* destHost) {
    int data = 0; // 4 bytes
    
    // each byte in the payload gets its own sequence number. the sequence number transmitted is the sequence number of the FIRST byte in the payload
    // ex if send 4 bytes in payload w/ sequence number of 100 in the sequence number field, then bytes 100, 101, 102, and 103 were transmitted in this packet
    // and the ACK back is 104
    
    int sequenceNumber = srcHost->sequenceNumberOfFirstByteInNextPacket; // sequence number of first byte
    srcHost->sequenceNumberOfFirstByteInNextPacket = srcHost->sequenceNumberOfFirstByteInNextPacket + 1; //FIN sequnce -> increment by 1
    
    srcHost->thisSideTerminatedConnection = 1;
    
    int newWindowSize = srcHost->receiverBufferQMD->maxNumItems - srcHost->receiverBufferQMD->numItems;
    
    TcpPacket *finTcpPacket = malloc(sizeof(TcpPacket));
    finTcpPacket->srcPort = srcHost->tcpPort;
    finTcpPacket->destPort = destHost->tcpPort;
    finTcpPacket->SYN = 0; // there is already a connection
    finTcpPacket->sequenceNumber = sequenceNumber;
    finTcpPacket->ACK = 1;
    finTcpPacket->ackNumber = srcHost->lastAckedSequenceNumber;
    finTcpPacket->FIN = 1;
    finTcpPacket->windowSize = newWindowSize;
    finTcpPacket->data = data;
    finTcpPacket->dataSize = 0;
    finTcpPacket->NS = 0;
    finTcpPacket->CWR = 0;
    finTcpPacket->ECE = 0;
    finTcpPacket->URG = 0;
    finTcpPacket->PSH = 0;
    finTcpPacket->RST = 0;
    finTcpPacket->urgentPtr = 0;
    
    
    sendTcpPacket(srcHost, finTcpPacket);
    
    return finTcpPacket; //TODO: free memory for dynamically allocated packet
}
