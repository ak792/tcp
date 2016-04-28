#ifndef QUEUETEST_H_   /* Include guard */
#define QUEUETEST_H_

void testEnqueue();
void testEnqueueFullQueueDropsOverflow();
void testDequeueMinSimple();
void testDequeueMinOutOfOrder();
void testDequeueMinEmptyQueue();
void testDequeueMinFullQueueThenEnqueue();

void runAllQueueTests();

#endif // QUEUETEST_H_