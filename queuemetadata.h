#ifndef QUEUEMETADATA_H_   /* Include guard */
#define QUEUEMETADATA_H_

typedef struct {
    int front;
    int back;
    int numItems;
    int maxNumItems;
} QueueMetadata;

void printQueueMetadata(QueueMetadata* queueMetadata);

#endif // QUEUEMETADATA_H_