#ifndef ENDHOST_H_   /* Include guard */
#define ENDHOST_H_

#include "tcppacket.h"
#include "queuemetadata.h"

typedef struct EndHost EndHost;

struct EndHost {
    int hostId;
    char* hostName;
    int tcpPort;
    
    struct EndHost* destHost;
    
    int thisSideTerminatedConnection;
    int otherSideTerminatedConnection;
    
    int sequenceNumberOfFirstByteInNextPacket;
    int lastAckedSequenceNumber;
    
    TcpPacket** senderBuffer;
    QueueMetadata* senderBufferQMD;
    
    TcpPacket** receiverBuffer;
    QueueMetadata* receiverBufferQMD;
    
    int lastAdvertisedWindowFromReceiver;
    
};

#endif // ENDHOST_H_