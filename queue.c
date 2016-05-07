#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "tcppacket.h"
#include "queue.h"

int enqueue(TcpPacket** packets, QueueMetadata* queueMetadata, TcpPacket* packet) {
    int isFull = queueMetadata->maxNumItems == queueMetadata->numItems;
    if (isFull) {
        return 0;
    }
    
    if (queueMetadata->back < queueMetadata->maxNumItems - 1) {
        queueMetadata->back++;
    } else {
        queueMetadata->back = 0;
    }
    
    packets[queueMetadata->back] = packet;
    queueMetadata->numItems++;

    
    return 1;
}

//don't use
TcpPacket* dequeue(TcpPacket** packets, QueueMetadata* queueMetadata) {
    if (queueMetadata->numItems == 0) {
        return 0; // NULL
    }
        
    TcpPacket* packet = packets[queueMetadata->front];
    queueMetadata->numItems--;
    packets[queueMetadata->front] = NULL;
    
    if (queueMetadata->front < queueMetadata->maxNumItems - 1) {
        queueMetadata->front++;
    } else {
        queueMetadata->front = 0;
    }
    
    return packet;
}


int getMinInd(TcpPacket** packets, QueueMetadata* queueMetadata) {
    if (queueMetadata->numItems == 0) {
        return -1;
    }
    
    if (queueMetadata->numItems == 1) {
        return queueMetadata->front;
    }
    
    int minPacketInd = queueMetadata->front;
    int i = minPacketInd;
    do {
        if (packets[i]->sequenceNumber < packets[minPacketInd]->sequenceNumber) {
            minPacketInd = i;
        }
        
        //increment i
        if (i != queueMetadata->maxNumItems) {
            i++;
        } else {
            i = 0;
        }
    } while (i != queueMetadata->back + 1);
    
    return minPacketInd;
}

TcpPacket* getMinPacket(TcpPacket** packets, QueueMetadata* queueMetadata) {
    int minPacketInd = getMinInd(packets, queueMetadata);
    TcpPacket* minPacket = packets[minPacketInd];
    
    return minPacket;
}


//bad but functional way to do this. use a heap eventually
TcpPacket* dequeueMin(TcpPacket** packets, QueueMetadata* queueMetadata) {
    if (queueMetadata->numItems == 0) {
        return NULL; //NULL
    }
    
    int minPacketInd = getMinInd(packets, queueMetadata);
    TcpPacket* minPacket = packets[minPacketInd];

    //compaction
    int i = (minPacketInd != queueMetadata->back) ? minPacketInd + 1 : 0;
    int terminal = (queueMetadata->back != queueMetadata->maxNumItems) ? queueMetadata->back + 1 : 0;
    
    if (i != queueMetadata->front) {
        do {
            //compat
            if (i > 0) {
                packets[i - 1] = packets[i];
            } else {
                packets[queueMetadata->maxNumItems] = packets[i];
            }
            
            //increment i
            if (i != queueMetadata->maxNumItems) {
                i++;
            } else {
                i = 0;
            }
            
        } while (i != terminal);
    }
    packets[queueMetadata->back] = NULL;
    
    //decr back
    if (queueMetadata->back > 0) {
        queueMetadata->back--;
    } else {
        queueMetadata->back = queueMetadata->maxNumItems - 1;
    }
    
    queueMetadata->numItems--;
    
    return minPacket;
}

