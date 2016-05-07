#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tcppacketarr.h"
#include "queue.h"
#include "network.h"
#include "tcpisngenerator.h"
#include "dynamicarray.h"



void putPacketInReceiverBuffer(EndHost* currHost, TcpPacket* receivedTcpPacket) {
    int success = enqueue(currHost->receiverBuffer, currHost->receiverBufferQMD, receivedTcpPacket);
    
    // if the receiver buffer is full, discard the packet
    
    if (success) {
        printf("Successfully put packet in receiver buffer");
    } else {
        printf("Attempt to put packet in receiver buffer failed");
    }
}

TcpPacket** pollNextPacketsFromReceiverBuffer(EndHost* currHost) {
    //TODO: dynamically resize the array so don't have to always do max possible
    int maxPacketsSize = currHost->receiverBufferQMD->maxNumItems;
    
    TcpPacket** packets = malloc(sizeof(TcpPacket*) * maxPacketsSize);
    int packetsSize = 0;
    
    int nextSeqNumExpected = currHost->lastAckedSequenceNumber;
    int currReceiverBufferSize = currHost->receiverBufferQMD->numItems;
    TcpPacket* minPacket = getMinPacket(currHost->receiverBuffer, currHost->receiverBufferQMD);
    if (minPacket == NULL) {
        return packets;
    }
    
    assert(minPacket != NULL);
    
    while (currHost->receiverBufferQMD->numItems > 0 && (minPacket->sequenceNumber == nextSeqNumExpected || minPacket->SYN == 1)) {
        //works correctly in dequeueMin, but isn't coming out of the function call correctly?
        minPacket = dequeueMin(currHost->receiverBuffer, currHost->receiverBufferQMD);
        assert(currHost->receiverBufferQMD->numItems == currReceiverBufferSize - 1); // check it was decreased in dequeueMin
        currReceiverBufferSize--;
        
        packets[packetsSize] = minPacket;
        packetsSize++;


        nextSeqNumExpected = minPacket->sequenceNumber + minPacket->dataSize;
        
        minPacket = getMinPacket(currHost->receiverBuffer, currHost->receiverBufferQMD);
    }

    return packets;
}

//TODO: eventually run this in a thread
//TODO: duplicates
TcpPacket* pollPacketsAndSendAck(EndHost* currHost) {
    
    int origReceiverBufferSize = currHost->receiverBufferQMD->numItems;
    
    TcpPacket** tcpPacketsFromReceiverBuffer = pollNextPacketsFromReceiverBuffer(currHost);
    
    
    
    int newReceiverBufferSize = currHost->receiverBufferQMD->numItems;
    int numPacketsPolled = origReceiverBufferSize - newReceiverBufferSize;
    
    for (int i = 0; i < numPacketsPolled; i++) {
        printTcpPacket(tcpPacketsFromReceiverBuffer[i]);
    }

    
    if (numPacketsPolled == 0) {
        return NULL; //TODO: nothing handles this yet
    }
    for (int i = 0; i < numPacketsPolled; i++) {
        assert (tcpPacketsFromReceiverBuffer[i]->destPort == currHost->tcpPort);
    }
    
    //do stuff with packet here
    

    //start sending ACK
    
    int sizeOfData = 0;
    int data = 0;

    //TODO: for now, just look at the last one. think about if in the future, we need to get all of them
    TcpPacket* lastTcpPacket = tcpPacketsFromReceiverBuffer[numPacketsPolled - 1];
    
    int tcpPortOnOtherHost = lastTcpPacket->srcPort;

    // each byte in the payload gets its own sequence number. the sequence number transmitted is the sequence number of the FIRST byte in the payload
    // ex if send 4 bytes in payload w/ sequence number of 100 in the sequence number field, then bytes 100, 101, 102, and 103 were transmitted in this packet
    // and the ACK back is 104
    
    //ex last ACK was 200, got a 4 byte packet, should ACK 200 + 4 = 204 to indicate receieved 200, 201, 202, 203
    int ackNumber;
    if (!lastTcpPacket->SYN) {
        ackNumber = lastTcpPacket->sequenceNumber + sizeof(lastTcpPacket->data);
    } else {
        ackNumber = lastTcpPacket->sequenceNumber + 1;
    }
    currHost->lastAckedSequenceNumber = ackNumber; //we ACK the seqno of the next packet we expect

    
    // for now, just do FIN, FIN-ACK, ACK
    if (lastTcpPacket->FIN) {
        currHost->otherSideTerminatedConnection = 1;
    }
    
    //if they terminated connection but we didn't yet, let's terminate it with a FIN-ACK
    // note that this won't be necessary once we move to FIN, ACK, FIN, ACK model
    int FIN = currHost->otherSideTerminatedConnection && !currHost->thisSideTerminatedConnection;
    
    int sequenceNumber;
    if (FIN) {
        sequenceNumber = currHost->sequenceNumberOfFirstByteInNextPacket + 1;
        currHost->sequenceNumberOfFirstByteInNextPacket = sequenceNumber + 1;
    } else if (lastTcpPacket->SYN) {
        sequenceNumber = generateInitialSequenceNumber(currHost);
        currHost->sequenceNumberOfFirstByteInNextPacket = sequenceNumber + 1;
    } else {
        // sequence number of first byte
        sequenceNumber = currHost->sequenceNumberOfFirstByteInNextPacket;
        currHost->sequenceNumberOfFirstByteInNextPacket += sizeOfData; // no data, just an ack
    }
    
    //somehow, this is setting receivedTcpPacket->FIN = 0. not sure how, but moved accessing receivedTcpPacket->FIN to above this line
    // if received a SYN (but not a SYN-ACK), let's send a SYN-ACK back
    int SYN = (lastTcpPacket->SYN && !lastTcpPacket->ACK);

    int numSlotsFreeInBuffer = currHost->receiverBufferQMD->maxNumItems - currHost->receiverBufferQMD->numItems;
    
    currHost->lastAdvertisedWindowFromReceiver = lastTcpPacket->windowSize;
    
    TcpPacket* responseTcpPacket = malloc(sizeof(TcpPacket));
    responseTcpPacket->srcPort = currHost->tcpPort;
    responseTcpPacket->destPort = tcpPortOnOtherHost;
    responseTcpPacket->SYN = SYN; // there is already a connection
    responseTcpPacket->sequenceNumber = sequenceNumber;
    responseTcpPacket->ACK = 1;
    responseTcpPacket->ackNumber = ackNumber;
    responseTcpPacket->dataOffset = 5;
    responseTcpPacket->FIN = FIN;
    responseTcpPacket->windowSize = numSlotsFreeInBuffer;
    responseTcpPacket->data = data;
    responseTcpPacket->NS = 0;
    responseTcpPacket->CWR = 0;
    responseTcpPacket->ECE = 0;
    responseTcpPacket->URG = 0;
    responseTcpPacket->PSH = 0;
    responseTcpPacket->RST = 0;
    responseTcpPacket->urgentPtr = 0;
    responseTcpPacket->dataSize = 0;
    
    sendTcpPacket(currHost, responseTcpPacket);
    
    free(lastTcpPacket);
    return responseTcpPacket;
}

