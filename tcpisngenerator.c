#include "endhost.h"

int generateInitialSequenceNumber(EndHost* host) {
    return (host->hostId + 1) * 50;
}