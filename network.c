#include "tcp.h"

void sendOverNetwork(struct EndHost* bob, TcpPacket* packet) {
    putPacketInReceiverBuffer(bob, packet);
}
