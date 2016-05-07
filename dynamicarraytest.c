#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tcppacket.h"
#include "dynamicarray.h"



TcpPacket* createPacketWithSequenceNumber(int seqNo) {
    TcpPacket* pkt = malloc(sizeof(TcpPacket*));
    pkt->sequenceNumber = seqNo;
    
    return pkt;
}

//TODO: broken
//currMaxSize = size we allocated arr to be. need to resize when hit this
//resizes if necessary, otherwise just returns the arr
void testResizeArrayIfNecessary() {
    int currSize = 3;
    int currMaxSize = 3;
    TcpPacket** arr = malloc(sizeof(TcpPacket*) * currMaxSize);
    
    TcpPacket* pkt1 = createPacketWithSequenceNumber(1);
    TcpPacket* pkt2 = createPacketWithSequenceNumber(2);
    TcpPacket* pkt3 = createPacketWithSequenceNumber(3);
    
    arr[0] = pkt1;
    arr[1] = pkt2;
    arr[2] = pkt3;
    
    TcpPacket** resizedArr = resizeArrayIfNecessary(arr, currSize, currMaxSize);
 
    assert(resizedArr[0] == pkt1);
    assert(resizedArr[1] == pkt2);
    assert(resizedArr[2] == pkt3);
    
    TcpPacket* pkt4 = createPacketWithSequenceNumber(4);
    resizedArr[3] = pkt4;
    assert(resizedArr[3] == pkt4);
    
    
    free(pkt1);
    free(pkt2);
    free(pkt3);
    free(pkt4);
}

void testDynamicArray() {
    testResizeArrayIfNecessary();
}