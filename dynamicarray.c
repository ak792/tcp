#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tcppacket.h"

//keeping because works. but don't need it in this project
TcpPacket** resizeArrayIfNecessary(TcpPacket** arr, int currSize, int currMaxSize) {
    if (currSize != currMaxSize) {
        return arr;
    }
    
    //dynamic array resizing
    
    currMaxSize *= 2;
    TcpPacket** tmp = malloc(sizeof(TcpPacket*) * currMaxSize);
    for (int i = 0; i < currMaxSize; i++) {
        tmp[i] = arr[i];
    }
    
    free(arr);
    arr = tmp;
    free(tmp);
    
    return arr;
}
