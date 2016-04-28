#ifndef QUEUE_H_   /* Include guard */
#define QUEUE_H_

#include "queuemetadata.h"
#include "tcppacket.h"

int enqueue(TcpPacket* packets[], QueueMetadata* queueMetadata, TcpPacket* packet);
TcpPacket* dequeue(TcpPacket* packets[], QueueMetadata* queueMetadata);
TcpPacket* dequeueMin(TcpPacket* packets[], QueueMetadata* queueMetadata);
TcpPacket* getMinPacket(TcpPacket** packets, QueueMetadata* queueMetadata);


#endif // QUEUE_H_