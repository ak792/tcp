#ifndef DYNAMICARRAY_H__   /* Include guard */
#define DYNAMICARRAY_H__

#include "tcppacket.h"

TcpPacket** resizeArrayIfNecessary(TcpPacket** arr, int currSize, int currMaxSize);

#endif // DYNAMICARRAY_H__