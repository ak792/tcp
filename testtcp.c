#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "tcp.h"
#include "tcppacketarr.h"

EndHost** setupAliceAndBob() {
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
    alice->destHost = bob; //cheat for now and just set this
    
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
    
    EndHost** aliceAndBob = malloc(sizeof(EndHost*) * 2);
    aliceAndBob[0] = alice;
    aliceAndBob[1] = bob;
    
    return aliceAndBob;
}



EndHost** setupAliceAndBobForConnectionTermination() {
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
    alice->sequenceNumberOfFirstByteInNextPacket = 71;
    alice->lastAckedSequenceNumber = 151;
    alice->senderBuffer = aliceSenderBuffer;
    alice->senderBufferQMD = aliceSenderBufferQMD;
    alice->receiverBuffer = aliceReceiverBuffer;
    alice->receiverBufferQMD = aliceReceiverBufferQMD;
    alice->lastAdvertisedWindowFromReceiver = initialLastAdvertisedWindowFromReceiver;
    alice->destHost = bob; //cheat for now and just set this
    
    bob->hostId = 1;
    bob->hostName = "Bob";
    bob->tcpPort = 1234;
    bob->thisSideTerminatedConnection = 0;
    bob->otherSideTerminatedConnection = 0;
    bob->sequenceNumberOfFirstByteInNextPacket = 151;
    bob->lastAckedSequenceNumber = 71;
    bob->senderBuffer = bobSenderBuffer;
    bob->senderBufferQMD = bobSenderBufferQMD;
    bob->receiverBuffer = bobReceiverBuffer;
    bob->receiverBufferQMD = bobReceiverBufferQMD;
    bob->lastAdvertisedWindowFromReceiver = initialLastAdvertisedWindowFromReceiver;
    bob->destHost = alice;
    
    EndHost** aliceAndBob = malloc(sizeof(EndHost*) * 2);
    aliceAndBob[0] = alice;
    aliceAndBob[1] = bob;
    
    return aliceAndBob;
}

//send the SYN packet Alice -> Bob
void sendSynPacket(EndHost* alice, EndHost* bob) {
    TcpPacket* synPacket = establishTcpConnection(alice, bob);    
    assert(synPacket->srcPort == 9999);
    assert(synPacket->destPort == 1234);
    assert(synPacket->SYN == 1);
    assert(synPacket->sequenceNumber == 50);
    assert(synPacket->ACK == 0);
    assert(synPacket->ackNumber == 0);
    assert(synPacket->FIN == 0);
    assert(synPacket->windowSize == 20);
    assert(synPacket->dataSize == 0);
    
    assert(bob->receiverBuffer[0] == synPacket);
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 0);
    assert(bob->receiverBufferQMD->numItems == 1);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
}

//send SYN-ACK Bob-> Alice
void sendSynAckPacket(EndHost* alice, EndHost* bob) {
    TcpPacket* synAckPacket = pollPacketsAndSendAck(bob);
    assert(synAckPacket->srcPort == 1234);
    assert(synAckPacket->destPort == 9999);
    assert(synAckPacket->SYN == 1);
    assert(synAckPacket->sequenceNumber == 100);
    assert(synAckPacket->ACK == 1);
    assert(synAckPacket->ackNumber == 51);
    assert(synAckPacket->FIN == 0);
    assert(synAckPacket->windowSize == 20);
    assert(synAckPacket->dataSize == 0);
    
    assert(alice->receiverBuffer[0] == synAckPacket);
    assert(alice->receiverBufferQMD->front == 0);
    assert(alice->receiverBufferQMD->back == 0);
    assert(alice->receiverBufferQMD->numItems == 1);
    assert(alice->receiverBufferQMD->maxNumItems == 20);
}

void sendAckPacketInResponseToSynAck(EndHost* alice, EndHost* bob) {
    TcpPacket* ackPacket = pollPacketsAndSendAck(alice);
    assert(ackPacket->srcPort == 9999);
    assert(ackPacket->destPort == 1234);
    assert(ackPacket->SYN == 0);
    assert(ackPacket->sequenceNumber == 50);
    assert(ackPacket->ACK == 1);
    assert(ackPacket->ackNumber == 101);
    assert(ackPacket->FIN == 0);
    assert(ackPacket->windowSize == 20);
    assert(ackPacket->dataSize == 0);
    
    assert(bob->receiverBuffer[0] == ackPacket);
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 0);
    assert(bob->receiverBufferQMD->numItems == 1);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    free(ackPacket);
}

void testConnectionEstablishment() {
    printf("starting connection establishment\n");

    
    EndHost** aliceAndBob = setupAliceAndBob();
    EndHost* alice = aliceAndBob[0];
    EndHost* bob = aliceAndBob[1];
    
    sendSynPacket(alice, bob);
    sendSynAckPacket(alice, bob);
    sendAckPacketInResponseToSynAck(alice, bob);
    
    free(alice->senderBuffer);
    free(alice->senderBufferQMD);
    
    free(alice->receiverBuffer);
    free(alice->receiverBufferQMD);

    
    free(bob->senderBuffer);
    free(bob->senderBufferQMD);
    
    free(bob->receiverBuffer);
    free(bob->receiverBufferQMD);
    
    free(alice);
    free(bob);
    
    printf("done connection establishment\n\n");
}

void sendFinPacket(EndHost* alice, EndHost* bob) {
    TcpPacket* finPacket = terminateTcpConnection(alice, bob);
    assert(finPacket->srcPort == 9999);
    assert(finPacket->destPort == 1234);
    assert(finPacket->SYN == 0);
    assert(finPacket->sequenceNumber == 71);
    assert(finPacket->ACK == 1);
    assert(finPacket->ackNumber == 151);
    assert(finPacket->FIN == 1);
    assert(finPacket->windowSize == 20);
    assert(finPacket->dataSize == 0);
    
    assert(bob->receiverBuffer[0] == finPacket);
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 0);
    assert(bob->receiverBufferQMD->numItems == 1);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
}

void sendFinAckPacket(EndHost* alice, EndHost* bob) {
    TcpPacket* ackPacket = pollPacketsAndSendAck(bob);
    assert(ackPacket->srcPort == 1234);
    assert(ackPacket->destPort == 9999);
    assert(ackPacket->SYN == 0);
    assert(ackPacket->sequenceNumber == 152);
    assert(ackPacket->ACK == 1);
    assert(ackPacket->ackNumber == 75);
    assert(ackPacket->FIN == 1);
    assert(ackPacket->windowSize == 20);
    assert(ackPacket->dataSize == 0);
    
    assert(alice->receiverBuffer[0] == ackPacket);
    assert(alice->receiverBufferQMD->front == 0);
    assert(alice->receiverBufferQMD->back == 0);
    assert(alice->receiverBufferQMD->numItems == 1);
    assert(alice->receiverBufferQMD->maxNumItems == 20);
    
    free(ackPacket);
}


void testConnectionTermination() {
    printf("starting connection termination\n");

    
    EndHost** aliceAndBob = setupAliceAndBobForConnectionTermination();
    EndHost* alice = aliceAndBob[0];
    EndHost* bob = aliceAndBob[1];
    
    sendFinPacket(alice, bob);
    sendFinAckPacket(alice, bob);
    
    free(alice->senderBuffer);
    free(alice->senderBufferQMD);
    
    free(alice->receiverBuffer);
    free(alice->receiverBufferQMD);
    
    free(bob->senderBuffer);
    free(bob->senderBufferQMD);
    
    free(bob->receiverBuffer);
    free(bob->receiverBufferQMD);
    
    free(alice);
    free(bob);

    
    
    printf("ending connection termination\n\n");
}

EndHost** setupAliceAndBobForSteadyState() {
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
    alice->sequenceNumberOfFirstByteInNextPacket = 123;
    alice->lastAckedSequenceNumber = 456;
    alice->senderBuffer = aliceSenderBuffer;
    alice->senderBufferQMD = aliceSenderBufferQMD;
    alice->receiverBuffer = aliceReceiverBuffer;
    alice->receiverBufferQMD = aliceReceiverBufferQMD;
    alice->lastAdvertisedWindowFromReceiver = initialLastAdvertisedWindowFromReceiver;
    alice->destHost = bob; //cheat for now and just set this
    
    bob->hostId = 1;
    bob->hostName = "Bob";
    bob->tcpPort = 1234;
    bob->thisSideTerminatedConnection = 0;
    bob->otherSideTerminatedConnection = 0;
    bob->sequenceNumberOfFirstByteInNextPacket = 456;
    bob->lastAckedSequenceNumber = 123;
    bob->senderBuffer = bobSenderBuffer;
    bob->senderBufferQMD = bobSenderBufferQMD;
    bob->receiverBuffer = bobReceiverBuffer;
    bob->receiverBufferQMD = bobReceiverBufferQMD;
    bob->lastAdvertisedWindowFromReceiver = initialLastAdvertisedWindowFromReceiver;
    bob->destHost = alice;
    
    EndHost** aliceAndBob = malloc(sizeof(EndHost*) * 2);
    aliceAndBob[0] = alice;
    aliceAndBob[1] = bob;
    
    return aliceAndBob;
}

void testSteadyStateSend2Receive1() {
    EndHost** aliceAndBob = setupAliceAndBobForSteadyState();
    EndHost* alice = aliceAndBob[0];
    EndHost* bob = aliceAndBob[1];
    
    //send 2, receive 1
    sendData(alice, bob, 500);
    assert(bob->receiverBuffer[0]->data == 500);
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 0);
    assert(bob->receiverBufferQMD->numItems == 1);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    sendData(alice, bob, 510);
    assert(bob->receiverBuffer[0]->data == 500);
    assert(bob->receiverBuffer[1]->data == 510);
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 1);
    assert(bob->receiverBufferQMD->numItems == 2);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    TcpPacket* ackPacket = pollPacketsAndSendAck(bob);
    assert(bob->receiverBuffer[0] == NULL);
    assert(bob->receiverBuffer[1] == NULL);
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 19);
    assert(bob->receiverBufferQMD->numItems == 0);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    assert(ackPacket->srcPort == 1234);
    assert(ackPacket->destPort == 9999);
    assert(ackPacket->SYN == 0);
    assert(ackPacket->sequenceNumber == 456);
    assert(ackPacket->ACK == 1);
    assert(ackPacket->ackNumber == 131);
    assert(ackPacket->FIN == 0);
    assert(ackPacket->windowSize == 20);
    assert(ackPacket->dataSize == 0);
    free(ackPacket);
    
    free(alice->senderBuffer);
    free(alice->senderBufferQMD);
    
    free(alice->receiverBuffer);
    free(alice->receiverBufferQMD);
    
    free(bob->senderBuffer);
    free(bob->senderBufferQMD);
    
    free(bob->receiverBuffer);
    free(bob->receiverBufferQMD);
    
    free(alice);
    free(bob);
}

void testSteadyStateSend4Receive1() {
    EndHost** aliceAndBob = setupAliceAndBobForSteadyState();
    EndHost* alice = aliceAndBob[0];
    EndHost* bob = aliceAndBob[1];
    
    sendData(alice, bob, 500);
    assert(bob->receiverBuffer[0]->data == 500);
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 0);
    assert(bob->receiverBufferQMD->numItems == 1);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    sendData(alice, bob, 510);
    assert(bob->receiverBuffer[0]->data == 500);
    assert(bob->receiverBuffer[1]->data == 510);
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 1);
    assert(bob->receiverBufferQMD->numItems == 2);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    sendData(alice, bob, 490);
    assert(bob->receiverBuffer[0]->data == 500);
    assert(bob->receiverBuffer[1]->data == 510);
    assert(bob->receiverBuffer[2]->data == 490);
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 2);
    assert(bob->receiverBufferQMD->numItems == 3);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    sendData(alice, bob, 480);
    assert(bob->receiverBuffer[0]->data == 500);
    assert(bob->receiverBuffer[1]->data == 510);
    assert(bob->receiverBuffer[2]->data == 490);
    assert(bob->receiverBuffer[3]->data == 480);
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 3);
    assert(bob->receiverBufferQMD->numItems == 4);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    TcpPacket* ackPacket = pollPacketsAndSendAck(bob);
    assert(bob->receiverBuffer[0] == NULL);
    assert(bob->receiverBuffer[1] == NULL);
    assert(bob->receiverBuffer[2] == NULL);
    assert(bob->receiverBuffer[3] == NULL);
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 19);
    assert(bob->receiverBufferQMD->numItems == 0);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    assert(ackPacket->srcPort == 1234);
    assert(ackPacket->destPort == 9999);
    assert(ackPacket->SYN == 0);
    assert(ackPacket->sequenceNumber == 456);
    assert(ackPacket->ACK == 1);
    assert(ackPacket->ackNumber == 139);
    assert(ackPacket->FIN == 0);
    assert(ackPacket->windowSize == 20);
    assert(ackPacket->dataSize == 0);
    free(ackPacket);
    
    
    free(alice->senderBuffer);
    free(alice->senderBufferQMD);
    
    free(alice->receiverBuffer);
    free(alice->receiverBufferQMD);
    
    free(bob->senderBuffer);
    free(bob->senderBufferQMD);
    
    free(bob->receiverBuffer);
    free(bob->receiverBufferQMD);
    
    free(alice);
    free(bob);
}

void testSteadyStateFillReceiverBufferThenReceive() {
    EndHost** aliceAndBob = setupAliceAndBobForSteadyState();
    EndHost* alice = aliceAndBob[0];
    EndHost* bob = aliceAndBob[1];
    
    for (int i = 0; i < 20; i++) {
        sendData(alice, bob, 10 * i);
        assert(bob->receiverBuffer[i]->data == 10 * i);
        
        assert(bob->receiverBufferQMD->front == 0);
        assert(bob->receiverBufferQMD->back == i);
        assert(bob->receiverBufferQMD->numItems == i + 1);
        assert(bob->receiverBufferQMD->maxNumItems == 20);
    }

    for (int i = 0; i < 20; i++) {
        assert(bob->receiverBuffer[i]->data == 10 * i);
    }
    
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 19);
    assert(bob->receiverBufferQMD->numItems == 20);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    
    TcpPacket* ackPacket = pollPacketsAndSendAck(bob);
    for (int i = 0; i < 20; i++) {
        assert(bob->receiverBuffer[i] == NULL);
    }
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 19);
    assert(bob->receiverBufferQMD->numItems == 0);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    assert(ackPacket->srcPort == 1234);
    assert(ackPacket->destPort == 9999);
    assert(ackPacket->SYN == 0);
    assert(ackPacket->sequenceNumber == 456);
    assert(ackPacket->ACK == 1);
    assert(ackPacket->ackNumber == 203);
    assert(ackPacket->FIN == 0);
    assert(ackPacket->windowSize == 20);
    assert(ackPacket->dataSize == 0);
    free(ackPacket);
    
    
    free(alice->senderBuffer);
    free(alice->senderBufferQMD);
    
    free(alice->receiverBuffer);
    free(alice->receiverBufferQMD);
    
    free(bob->senderBuffer);
    free(bob->senderBufferQMD);
    
    free(bob->receiverBuffer);
    free(bob->receiverBufferQMD);
    
    free(alice);
    free(bob);
}

void testOverflowReceiverBufferThenReceive() {
    EndHost** aliceAndBob = setupAliceAndBobForSteadyState();
    EndHost* alice = aliceAndBob[0];
    EndHost* bob = aliceAndBob[1];
    
    for (int i = 0; i < 25; i++) {
        sendData(alice, bob, 10 * i);
        
        assert(bob->receiverBufferQMD->front == 0);
        assert(bob->receiverBufferQMD->maxNumItems == 20);

        if (i < 20) {
            assert(bob->receiverBuffer[i]->data == 10 * i);
            assert(bob->receiverBufferQMD->back == i);
        }
        
        int expectedNumItems = (i + 1 <= 20) ? i + 1 : 20;
        assert(bob->receiverBufferQMD->numItems == expectedNumItems);
    }
    
    for (int i = 0; i < 20; i++) {
        assert(bob->receiverBuffer[i]->data == 10 * i);
    }
    
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 19);
    assert(bob->receiverBufferQMD->numItems == 20);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    
    TcpPacket* ackPacket = pollPacketsAndSendAck(bob);
    for (int i = 0; i < 20; i++) {
        assert(bob->receiverBuffer[i] == NULL);
    }
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 19);
    assert(bob->receiverBufferQMD->numItems == 0);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    assert(ackPacket->srcPort == 1234);
    assert(ackPacket->destPort == 9999);
    assert(ackPacket->SYN == 0);
    assert(ackPacket->sequenceNumber == 456);
    assert(ackPacket->ACK == 1);
    assert(ackPacket->ackNumber == 203);
    assert(ackPacket->FIN == 0);
    assert(ackPacket->windowSize == 20);
    assert(ackPacket->dataSize == 0);
    free(ackPacket);
    
    
    free(alice->senderBuffer);
    free(alice->senderBufferQMD);
    
    free(alice->receiverBuffer);
    free(alice->receiverBufferQMD);
    
    free(bob->senderBuffer);
    free(bob->senderBufferQMD);
    
    free(bob->receiverBuffer);
    free(bob->receiverBufferQMD);
    
    free(alice);
    free(bob);
}

void testReceiveWithEmptyReceiverBuffer() {
    EndHost** aliceAndBob = setupAliceAndBobForSteadyState();
    EndHost* alice = aliceAndBob[0];
    EndHost* bob = aliceAndBob[1];
    
    
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == -1);
    assert(bob->receiverBufferQMD->numItems == 0);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    
    TcpPacket* ackPacket = pollPacketsAndSendAck(bob);
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == -1);
    assert(bob->receiverBufferQMD->numItems == 0);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    assert(ackPacket == NULL);
    
    free(alice->senderBuffer);
    free(alice->senderBufferQMD);
    
    free(alice->receiverBuffer);
    free(alice->receiverBufferQMD);
    
    free(bob->senderBuffer);
    free(bob->senderBufferQMD);
    
    free(bob->receiverBuffer);
    free(bob->receiverBufferQMD);
    
    free(alice);
    free(bob);
}

void swap(TcpPacket** arr, int i, int j) {
    TcpPacket* temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void sendDataCustomSequenceNumber(EndHost* alice, EndHost* bob, int data, int sequenceNumber) {
    int sizeOfData = 4; // data is 4 bytes
    
//    int currSequenceNumber = alice->sequenceNumberOfFirstByteInNextPacket;
//    alice->sequenceNumberOfFirstByteInNextPacket += sizeOfData;
    
    TcpPacket* tcpPacket = malloc(sizeof(TcpPacket));
    tcpPacket->srcPort = alice->tcpPort;
    tcpPacket->destPort = bob->tcpPort;
    tcpPacket->SYN = 0; // there is already a connection
    tcpPacket->sequenceNumber = sequenceNumber;
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
    
    
    doSend(alice, tcpPacket);

}

void testReceiveWithOutOfOrderSequenceNumbers() {
    EndHost** aliceAndBob = setupAliceAndBobForSteadyState();
    EndHost* alice = aliceAndBob[0];
    EndHost* bob = aliceAndBob[1];
    
    
    sendDataCustomSequenceNumber(alice, bob, 10, alice->sequenceNumberOfFirstByteInNextPacket + 4);
    sendDataCustomSequenceNumber(alice, bob, 20, alice->sequenceNumberOfFirstByteInNextPacket + 8);
    sendDataCustomSequenceNumber(alice, bob, 30, alice->sequenceNumberOfFirstByteInNextPacket + 0);
    
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 2);
    assert(bob->receiverBufferQMD->numItems == 3);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    TcpPacket* ackPacket = pollPacketsAndSendAck(bob);
    for (int i = 0; i < 3; i++) {
        assert(bob->receiverBuffer[i] == NULL);
    }
    assert(bob->receiverBufferQMD->front == 0);
    assert(bob->receiverBufferQMD->back == 19);
    assert(bob->receiverBufferQMD->numItems == 0);
    assert(bob->receiverBufferQMD->maxNumItems == 20);
    
    assert(ackPacket->srcPort == 1234);
    assert(ackPacket->destPort == 9999);
    assert(ackPacket->SYN == 0);
    assert(ackPacket->sequenceNumber == 456);
    assert(ackPacket->ACK == 1);
    assert(ackPacket->ackNumber == alice->sequenceNumberOfFirstByteInNextPacket + 8 + 4);
    assert(ackPacket->FIN == 0);
    assert(ackPacket->windowSize == 20);
    assert(ackPacket->dataSize == 0);
    free(ackPacket);
    
    
    free(alice->senderBuffer);
    free(alice->senderBufferQMD);
    
    free(alice->receiverBuffer);
    free(alice->receiverBufferQMD);
    
    free(bob->senderBuffer);
    free(bob->senderBufferQMD);
    
    free(bob->receiverBuffer);
    free(bob->receiverBufferQMD);
    
    free(alice);
    free(bob);
}

void testSteadyState() {
    
    testSteadyStateSend2Receive1();
    testSteadyStateSend4Receive1();
    
    testSteadyStateFillReceiverBufferThenReceive();
    testOverflowReceiverBufferThenReceive();
    
    testReceiveWithEmptyReceiverBuffer();
    testReceiveWithOutOfOrderSequenceNumbers();
}
