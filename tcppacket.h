#ifndef TCPPACKET_H_   /* Include guard */
#define TCPPACKET_H_

typedef struct {
    int srcPort;
    int destPort;
    int sequenceNumber;
    int ackNumber;
    int dataOffset;
    int NS;
    int CWR;
    int ECE;
    int URG;
    int ACK;
    int PSH;
    int RST;
    int SYN;
    int FIN;
    int windowSize;
    int urgentPtr;
    
    int data; //TODO: not actually an int
    int dataSize; //not actually put in the packet, but it can be computed from the containing IP packet size
} TcpPacket;

void printTcpPacket(TcpPacket*);


#endif // TCPPACKET_H_