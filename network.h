#ifndef NETWORK_H_   /* Include guard */
#define NETWORK_H_

#include "tcpreceiver.h"

void sendOverNetwork(EndHost* bob, TcpPacket* packet);

#endif // NETWORK_H_