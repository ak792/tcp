#include <stdio.h>
#include "queuemetadata.h"

void printQueueMetadata(QueueMetadata* queueMetadata) {
    printf("queueMetadata front: %d, back: %d, numItems %d, maxNumItems %d\n",
           queueMetadata->front, queueMetadata->back, queueMetadata->numItems, queueMetadata->maxNumItems);
}