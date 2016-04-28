#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "tcp.h"
#include "tcppacketarr.h"

#include "queuetest.h"
#include "testtcp.h"


void executeConnectionEstablishment(EndHost* alice, EndHost* bob) {
    printf("starting connection establishment\n");
    
    //send the SYN packet Alice -> Bob
    TcpPacket* synPacket = establishTcpConnection(alice, bob);

    //send SYN-ACK Bob-> Alice
    putPacketInReceiverBuffer(bob, synPacket);
    TcpPacket* synAckPacket = pollPacketsAndSendAck(bob);
    
    // send ACK as Alice
    putPacketInReceiverBuffer(alice, synAckPacket);
    TcpPacket* ackPacket = pollPacketsAndSendAck(alice);
    free(ackPacket);
    
    printf("done connection establishment\n\n");
}

void executeSteadyStatePutPacketInSenderBuffer(EndHost* alice, EndHost* bob) {

    printf("starting executeSteadyStatePutPacketInSenderBuffer\n");
    
    int data = 500;
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

    putPacketInSenderBuffer(alice, tcpPacket);
    
    printf("put packet in sender buffer ");
    printTcpPacket(tcpPacket);
    
    printf("ending executeSteadyStatePutPacketInSenderBuffer\n\n");
}

void executeSteadyStateSendPacketsFromSenderBuffer(EndHost* alice, EndHost* bob) {
    printf("starting executeSteadyStateSendPacketsFromSenderBuffer\n");
    
    int originalAliceSenderBufferSize = alice->senderBufferQMD->numItems;
    
    TcpPacketArr* sentPackets = sendPacketsFromSenderBuffer(alice);
    assert (alice->senderBufferQMD->numItems == 0);
    assert (*sentPackets->size == originalAliceSenderBufferSize);
    
    int size = *sentPackets->size;
    printf("putting %d packets in receiver buffer\n", size);
    
    for (int i = 0; i < size; i++) {
        printf("putting next packet in receiver buffer\n");

        int prevBobReceiverBufferSize = bob->receiverBufferQMD->numItems;
        putPacketInReceiverBuffer(bob, sentPackets->arr[i]);
        assert (bob->receiverBufferQMD->numItems == prevBobReceiverBufferSize + 1); //for now
        
    }
    
    assert (bob->receiverBufferQMD->numItems == originalAliceSenderBufferSize); //for now
    
    free(sentPackets->arr);
    free(sentPackets);
    
    printf("ending executeSteadyStateSendPacketsFromSenderBuffer\n\n");
}

void executeSteadyStateReceive(EndHost* alice, EndHost* bob) {
    
    printf("starting steady state receive\n");
    TcpPacket* ackPacket = pollPacketsAndSendAck(bob);
    free(ackPacket);
    
    printf("ending steady state receive\n\n");
}


void executeConnectionTermination(EndHost* alice, EndHost* bob) {
    printf("starting connection termination\n");
    
    // send FIN as Alice
    TcpPacket* finPacket = terminateTcpConnection(alice, bob);
    
    // send FIN-ACK as Bob
    putPacketInReceiverBuffer(bob, finPacket);
    TcpPacket* finAckPacket = pollPacketsAndSendAck(bob);
    
    // send ACK as Alice
    putPacketInReceiverBuffer(alice, finAckPacket);
    TcpPacket* ackPacket = pollPacketsAndSendAck(alice);
    free(ackPacket);
    
    printf("ending connection termination\n\n");
}

void executeTcpRun() {
    int maxNumPacketsInSenderBuffer = 20;
    int maxNumPacketsInReceiverBuffer = 20;
    int initialLastAdvertisedWindowFromReceiver = 20;
    
    TcpPacket** aliceSenderBuffer = malloc(sizeof(TcpPacket*) * maxNumPacketsInSenderBuffer);
    QueueMetadata* aliceSenderBufferQMD = malloc(sizeof(QueueMetadata));
    aliceSenderBufferQMD->front = 0;
    aliceSenderBufferQMD->back = -1;
    aliceSenderBufferQMD->numItems = 0;
    aliceSenderBufferQMD->maxNumItems = maxNumPacketsInSenderBuffer;
    
    TcpPacket** aliceReceiverBuffer = malloc(sizeof(TcpPacket*) * maxNumPacketsInReceiverBuffer);
    QueueMetadata* aliceReceiverBufferQMD = malloc(sizeof(QueueMetadata));
    aliceReceiverBufferQMD->front = 0;
    aliceReceiverBufferQMD->back = -1;
    aliceReceiverBufferQMD->numItems = 0;
    aliceReceiverBufferQMD->maxNumItems = maxNumPacketsInReceiverBuffer;
    
    TcpPacket** bobSenderBuffer = malloc(sizeof(TcpPacket*) * maxNumPacketsInSenderBuffer);
    QueueMetadata* bobSenderBufferQMD = malloc(sizeof(QueueMetadata));
    bobSenderBufferQMD->front = 0;
    bobSenderBufferQMD->back = -1;
    bobSenderBufferQMD->numItems = 0;
    bobSenderBufferQMD->maxNumItems = maxNumPacketsInSenderBuffer;
    
    
    TcpPacket** bobReceiverBuffer = malloc(sizeof(TcpPacket*) * maxNumPacketsInReceiverBuffer);
    QueueMetadata* bobReceiverBufferQMD = malloc(sizeof(QueueMetadata));
    bobReceiverBufferQMD->front = 0;
    bobReceiverBufferQMD->back = -1; //maybe set this to maxNumItems
    bobReceiverBufferQMD->numItems = 0;
    bobReceiverBufferQMD->maxNumItems = maxNumPacketsInReceiverBuffer;
    
    
    EndHost* alice = malloc(sizeof(EndHost));
    EndHost* bob = malloc(sizeof(EndHost));

    alice->hostId = 0;
    alice->hostName = "Alice";
    alice->tcpPort = 9999;
    alice->thisSideTerminatedConnection = 0;
    alice->otherSideTerminatedConnection = 0;
    alice->sequenceNumberOfFirstByteInNextPacket = -1;
    alice->lastAckedSequenceNumber = -1;
    alice->senderBuffer = aliceSenderBuffer;
    alice->senderBufferQMD = aliceSenderBufferQMD;
    alice->receiverBuffer = aliceReceiverBuffer;
    alice->receiverBufferQMD = aliceReceiverBufferQMD;
    alice->lastAdvertisedWindowFromReceiver = initialLastAdvertisedWindowFromReceiver;
    alice->destHost = bob;
    
    bob->hostId = 1;
    bob->hostName = "Bob";
    bob->tcpPort = 1234;
    bob->thisSideTerminatedConnection = 0;
    bob->otherSideTerminatedConnection = 0;
    bob->sequenceNumberOfFirstByteInNextPacket = -1;
    bob->lastAckedSequenceNumber = -1;
    bob->senderBuffer = bobSenderBuffer;
    bob->senderBufferQMD = bobSenderBufferQMD;
    bob->receiverBuffer = bobReceiverBuffer;
    bob->receiverBufferQMD = bobReceiverBufferQMD;
    bob->lastAdvertisedWindowFromReceiver = initialLastAdvertisedWindowFromReceiver;
    bob->destHost = alice;
    
    
    
    executeConnectionEstablishment(alice, bob);
    
    
    //TODO: send out of order packets and reorder upon receipt
    executeSteadyStatePutPacketInSenderBuffer(alice, bob);
    assert (alice->senderBufferQMD->numItems == 1);
    assert (bob->receiverBufferQMD->numItems == 0);
    
    executeSteadyStateSendPacketsFromSenderBuffer(alice, bob);
    assert (alice->senderBufferQMD->numItems == 0);
    assert (bob->receiverBufferQMD->numItems == 1);
    
    executeSteadyStateReceive(alice, bob);
    
    
    executeSteadyStatePutPacketInSenderBuffer(alice, bob);
    executeSteadyStatePutPacketInSenderBuffer(alice, bob);
    executeSteadyStatePutPacketInSenderBuffer(alice, bob);
    executeSteadyStateSendPacketsFromSenderBuffer(alice, bob);
    
    executeSteadyStateReceive(alice, bob);
    executeSteadyStateReceive(alice, bob);
    executeSteadyStateReceive(alice, bob);
    
    
    executeConnectionTermination(alice, bob);
    
    free(aliceSenderBuffer);
    free(aliceSenderBufferQMD);
    
    free(aliceReceiverBuffer);
    free(aliceReceiverBufferQMD);
    
    free(bobSenderBuffer);
    free(bobSenderBufferQMD);
    
    free(bobReceiverBuffer);
    free(bobReceiverBufferQMD);
    
    free(alice);
    free(bob);
}

int main(int argc, const char * argv[]) {
    runAllQueueTests();
    
    testConnectionEstablishment();
    testConnectionTermination();
    testSteadyState();
    
//    executeTcpRun();
    return 0;
}
