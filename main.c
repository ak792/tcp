#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "queuetest.h"
#include "testtcp.h"
#include "dynamicarraytest.h"

int main(int argc, const char * argv[]) {
    runAllQueueTests();
    
    testDynamicArray();
    
    testConnectionEstablishment();
    testConnectionTermination();
    testSteadyState();
    
    return 0;
}
