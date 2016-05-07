#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tcpsender.h"
#include "tcpreceiver.h"
#include "queue.h"



void printArr(TcpPacket** packets, QueueMetadata* queueMetadata) {
    printf("\nprinting arr.\n");
    printQueueMetadata(queueMetadata);
    
    if (queueMetadata->numItems == 0) {
        return;
    }
    
    int i = queueMetadata->front;
    do {
        printf("printing packet %d\n", i);
        printTcpPacket(packets[i]);
        
        //increment i
        if (i != queueMetadata->maxNumItems) {
            i++;
        } else {
            i = 0;
        }
    } while (i != queueMetadata->back + 1);
}

//Tests can enqueue in empty queue and in a normal queue
void testEnqueue() {
    int maxNumPackets = 3;
    
    TcpPacket** packets = malloc(sizeof(TcpPacket*) * maxNumPackets);
    QueueMetadata* queueMetadata = malloc(sizeof(QueueMetadata*));
    
    queueMetadata->front = 0;
    queueMetadata->numItems = 0;
    queueMetadata->maxNumItems = maxNumPackets;
    queueMetadata->back = -1;
    
    
    TcpPacket* packet1 = malloc(sizeof(TcpPacket*));
    packet1->sequenceNumber = 1;
    
    TcpPacket* packet2 = malloc(sizeof(TcpPacket*));
    packet2->sequenceNumber = 2;
    
    TcpPacket* packet3 = malloc(sizeof(TcpPacket*));
    packet3->sequenceNumber = 3;
    
    assert(enqueue(packets, queueMetadata, packet1) == 1);
    assert(enqueue(packets, queueMetadata, packet2) == 1);
    assert(enqueue(packets, queueMetadata, packet3) == 1);
    
    assert(packets[0] == packet1);
    assert(packets[1] == packet2);
    assert(packets[2] == packet3);
}

//Should just drop the extra packet
void testEnqueueFullQueueDropsOverflow() {
    int maxNumPackets = 3;
    
    TcpPacket** packets = malloc(sizeof(TcpPacket*) * maxNumPackets);
    QueueMetadata* queueMetadata = malloc(sizeof(QueueMetadata*));
    
    queueMetadata->front = 0;
    queueMetadata->numItems = 0;
    queueMetadata->maxNumItems = maxNumPackets;
    queueMetadata->back = -1;
    
    
    TcpPacket* packet1 = malloc(sizeof(TcpPacket*));
    packet1->sequenceNumber = 1;
    
    TcpPacket* packet2 = malloc(sizeof(TcpPacket*));
    packet2->sequenceNumber = 2;
    
    TcpPacket* packet3 = malloc(sizeof(TcpPacket*));
    packet3->sequenceNumber = 3;
    
    TcpPacket* packet4 = malloc(sizeof(TcpPacket*));
    packet4->sequenceNumber = 4;
    
    assert(enqueue(packets, queueMetadata, packet1) == 1);
    assert(enqueue(packets, queueMetadata, packet2) == 1);
    assert(enqueue(packets, queueMetadata, packet3) == 1);
    assert(enqueue(packets, queueMetadata, packet4) == 0);
    
    
    assert(packets[0] == packet1);
    assert(packets[1] == packet2);
    assert(packets[2] == packet3);
}

void testDequeueMinSimple() {
    int maxNumPackets = 3;
    
    TcpPacket** packets = malloc(sizeof(TcpPacket*) * maxNumPackets);
    QueueMetadata* queueMetadata = malloc(sizeof(QueueMetadata*));
    
    queueMetadata->front = 0;
    queueMetadata->numItems = 0;
    queueMetadata->maxNumItems = maxNumPackets;
    queueMetadata->back = -1;
    
    
    TcpPacket* packet1 = malloc(sizeof(TcpPacket*));
    packet1->sequenceNumber = 1;
    
    TcpPacket* packet2 = malloc(sizeof(TcpPacket*));
    packet2->sequenceNumber = 2;
    
    TcpPacket* packet3 = malloc(sizeof(TcpPacket*));
    packet3->sequenceNumber = 3;
    
    enqueue(packets, queueMetadata, packet1);
    enqueue(packets, queueMetadata, packet2);
    enqueue(packets, queueMetadata, packet3);
    
    
    //    printArr(packets, queueMetadata);
    
    assert(packets[0] == packet1);
    assert(packets[1] == packet2);
    assert(packets[2] == packet3);
    
    TcpPacket* dequeuedPacket = dequeueMin(packets, queueMetadata);
    assert(dequeuedPacket == packet1);
    
    //    printArr(packets, queueMetadata);
    
    assert(packets[0] == packet2);
    assert(packets[1] == packet3);
    assert(packets[2] == NULL);
    
    dequeuedPacket = dequeueMin(packets, queueMetadata);
    assert(dequeuedPacket == packet2);
    //    printArr(packets, queueMetadata);
    
    assert(packets[0] == packet3);
    assert(packets[1] == NULL);
    assert(packets[2] == NULL);
    
    dequeuedPacket = dequeueMin(packets, queueMetadata);
    assert(dequeuedPacket == packet3);
    assert(packets[0] == NULL);
    assert(packets[1] == NULL);
    assert(packets[2] == NULL);
    
    free(packet1);
    free(packet2);
    free(packet3);
    
    free(queueMetadata);
    free(packets);
}


void testDequeueMinOutOfOrder() {
    int maxNumPackets = 3;
    
    TcpPacket** packets = malloc(sizeof(TcpPacket*) * maxNumPackets);
    QueueMetadata* queueMetadata = malloc(sizeof(QueueMetadata*));
    
    queueMetadata->front = 0;
    queueMetadata->numItems = 0;
    queueMetadata->maxNumItems = maxNumPackets;
    queueMetadata->back = maxNumPackets - 1;
    
    
    TcpPacket* packet1 = malloc(sizeof(TcpPacket*));
    packet1->sequenceNumber = 100;
    
    TcpPacket* packet2 = malloc(sizeof(TcpPacket*));
    packet2->sequenceNumber = 50;
    
    TcpPacket* packet3 = malloc(sizeof(TcpPacket*));
    packet3->sequenceNumber = 75;
    
    
    TcpPacket* packet4 = malloc(sizeof(TcpPacket*));
    packet4->sequenceNumber = 230;
    
    enqueue(packets, queueMetadata, packet1);
    enqueue(packets, queueMetadata, packet2);
    enqueue(packets, queueMetadata, packet3);
    
    
//    printArr(packets, queueMetadata);
    
    assert(packets[0] == packet1);
    assert(packets[1] == packet2);
    assert(packets[2] == packet3);
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 2);
    assert(queueMetadata->numItems == 3);
    assert(queueMetadata->maxNumItems == 3);
    
    TcpPacket* dequeuedPacket = dequeueMin(packets, queueMetadata);
    assert(dequeuedPacket == packet2);
    
//    printArr(packets, queueMetadata);
    
    assert(packets[0] == packet1);
    assert(packets[1] == packet3);
    assert(packets[2] == NULL);
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 1);
    assert(queueMetadata->numItems == 2);
    assert(queueMetadata->maxNumItems == 3);
    
    dequeuedPacket = dequeueMin(packets, queueMetadata);
    assert(dequeuedPacket == packet3);
//    printArr(packets, queueMetadata);
    
    assert(packets[0] == packet1);
    assert(packets[1] == NULL);
    assert(packets[2] == NULL);
    
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 0);
    assert(queueMetadata->numItems == 1);
    assert(queueMetadata->maxNumItems == 3);
    
    enqueue(packets, queueMetadata, packet4);
    
    assert(packets[0] == packet1);
    assert(packets[1] == packet4);
    assert(packets[2] == NULL);
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 1);
    assert(queueMetadata->numItems == 2);
    assert(queueMetadata->maxNumItems == 3);
    
    dequeuedPacket = dequeueMin(packets, queueMetadata);
    assert(dequeuedPacket == packet1);
    assert(packets[0] == packet4);
    assert(packets[1] == NULL);
    assert(packets[2] == NULL);
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 0);
    assert(queueMetadata->numItems == 1);
    assert(queueMetadata->maxNumItems == 3);
    
    dequeuedPacket = dequeueMin(packets, queueMetadata);
    assert(dequeuedPacket == packet4);
    assert(packets[0] == NULL);
    assert(packets[1] == NULL);
    assert(packets[2] == NULL);
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 2);
    assert(queueMetadata->numItems == 0);
    assert(queueMetadata->maxNumItems == 3);
    
    free(packet1);
    free(packet2);
    free(packet3);
    free(packet4);
    
    free(queueMetadata);
    free(packets);
}

void testDequeueMinEmptyQueue() {
    int maxNumPackets = 3;
    
    TcpPacket** packets = malloc(sizeof(TcpPacket*) * maxNumPackets);
    QueueMetadata* queueMetadata = malloc(sizeof(QueueMetadata*));
    
    queueMetadata->front = 0;
    queueMetadata->numItems = 0;
    queueMetadata->maxNumItems = maxNumPackets;
    queueMetadata->back = maxNumPackets - 1;
    
//    printArr(packets, queueMetadata);
    
    TcpPacket* dequeuedPacket = dequeueMin(packets, queueMetadata);
    assert(dequeuedPacket == NULL);
    
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 2);
    assert(queueMetadata->numItems == 0);
    assert(queueMetadata->maxNumItems == 3);
    
    free(queueMetadata);
    free(packets);
}

void testDequeueMinFullQueueThenEnqueue() {
    int maxNumPackets = 3;
    
    TcpPacket** packets = malloc(sizeof(TcpPacket*) * maxNumPackets);
    QueueMetadata* queueMetadata = malloc(sizeof(QueueMetadata*));
    
    queueMetadata->front = 0;
    queueMetadata->numItems = 0;
    queueMetadata->maxNumItems = maxNumPackets;
    queueMetadata->back = maxNumPackets - 1;
    
    
    TcpPacket* packet1 = malloc(sizeof(TcpPacket*));
    packet1->sequenceNumber = 1;
    
    TcpPacket* packet2 = malloc(sizeof(TcpPacket*));
    packet2->sequenceNumber = 2;
    
    TcpPacket* packet3 = malloc(sizeof(TcpPacket*));
    packet3->sequenceNumber = 3;
    
    TcpPacket* packet4 = malloc(sizeof(TcpPacket*));
    packet4->sequenceNumber = 230;
    
    enqueue(packets, queueMetadata, packet1);
    enqueue(packets, queueMetadata, packet2);
    enqueue(packets, queueMetadata, packet3);
    
    assert(packets[0] == packet1);
    assert(packets[1] == packet2);
    assert(packets[2] == packet3);
    
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 2);
    assert(queueMetadata->numItems == 3);
    assert(queueMetadata->maxNumItems == 3);
    
    TcpPacket* dequeuedPacket = dequeueMin(packets, queueMetadata);
    assert(dequeuedPacket == packet1);
    
    assert(packets[0] == packet2);
    assert(packets[1] == packet3);
    assert(packets[2] == NULL);
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 1);
    assert(queueMetadata->numItems == 2);
    assert(queueMetadata->maxNumItems == 3);
    
    enqueue(packets, queueMetadata, packet4);
    assert(packets[0] == packet2);
    assert(packets[1] == packet3);
    assert(packets[2] == packet4);
    
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 2);
    assert(queueMetadata->numItems == 3);
    assert(queueMetadata->maxNumItems == 3);
    
    
    dequeuedPacket = dequeueMin(packets, queueMetadata);
    assert(dequeuedPacket == packet2);
    
    assert(packets[0] == packet3);
    assert(packets[1] == packet4);
    assert(packets[2] == NULL);
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 1);
    assert(queueMetadata->numItems == 2);
    assert(queueMetadata->maxNumItems == 3);
    
    dequeuedPacket = dequeueMin(packets, queueMetadata);
    assert(dequeuedPacket == packet3);
    assert(packets[0] == packet4);
    assert(packets[1] == NULL);
    assert(packets[2] == NULL);
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 0);
    assert(queueMetadata->numItems == 1);
    assert(queueMetadata->maxNumItems == 3);
    
    dequeuedPacket = dequeueMin(packets, queueMetadata);
    assert(dequeuedPacket == packet4);
    assert(packets[0] == NULL);
    assert(packets[1] == NULL);
    assert(packets[2] == NULL);
    
    
    assert(queueMetadata->front == 0);
    assert(queueMetadata->back == 2);
    assert(queueMetadata->numItems == 0);
    assert(queueMetadata->maxNumItems == 3);
    
    free(packet1);
    free(packet2);
    free(packet3);
    
    free(queueMetadata);
    free(packets);
}

void runAllQueueTests() {
    testEnqueue();
    testEnqueueFullQueueDropsOverflow();
    testDequeueMinSimple();
    testDequeueMinOutOfOrder();
    testDequeueMinEmptyQueue();
    testDequeueMinFullQueueThenEnqueue();
}