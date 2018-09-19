// author: Kunal Roychoudhury
// CS&T IIEST, Shibpur

# include "encoderDecoder.h"

void Codec::memcpy (void *dest, const void *src, uint32_t numOfBytes) {
    char *tmp = (char *) dest;
    char *ptr = (char *) src;
    for (uint32_t i = 0; i < numOfBytes; i ++) {
        tmp[i] = ptr[i];
    }
    return;
}

void Codec::EncodePacket (char *packet, short &packetLength, Packet_Contents &ob) {
    //  using pass by reference to speed up; take care to not change the struct
    packetLength = 0;
    memset ((void *) packet, 0, PACKET_SIZE);
    uint16_t OPC = htons (ob.opcode);
    memcpy ((void *) &packet[packetLength], (void *) &OPC, sizeof (OPC));
    packetLength += sizeof (OPC);

    if (ob.opcode == RRQ || ob.opcode == WRQ) {
        memcpy ((void *) &packet[packetLength], (void *) ob.fileName, ob.fileNameLength);
        packetLength += ob.fileNameLength;
        memcpy ((void *) &packet[packetLength], (void *) ob.mode, ob.modeLength);
        packetLength += ob.modeLength;
    }
    else if (ob.opcode == DATA) {
        unsigned short BN = htons (ob.blockNumber);
        memcpy ((void *) &packet[packetLength], (void *) &BN, sizeof (BN));
        packetLength += sizeof (BN);
        memcpy ((void *) &packet[packetLength], (void *) ob.data, ob.dataLength);
        packetLength += ob.dataLength;
    }
    else if (ob.opcode == ACK) {
        uint16_t BN = htons (ob.blockNumber);
        memcpy ((void *) &packet[packetLength], (void *) &BN, sizeof (BN));
        packetLength += sizeof (BN);
    }
    else if (ob.opcode == ERROR) {
        uint16_t BN = htons (ob.blockNumber);
        memcpy ((void *) &packet[packetLength], (void *) &BN, sizeof (BN));
        packetLength += sizeof (BN);
        memcpy ((void *) &packet[packetLength], (void *) ob.data, ob.dataLength);   //  error message multiplexed with data
        packetLength += ob.dataLength;
    }
    else {
        packetLength = 0;
        return;
    }
    return;
}

void Codec::DecodePacket (Packet_Contents &ob, char *packet, short packetLength) {
    ob.refresh ();
    unsigned short index = 0;
    memcpy ((void *) &ob.opcode, packet, sizeof (ob.opcode));
    ob.opcode = ntohs (ob.opcode);
    index += sizeof (ob.opcode);

    if (ob.opcode == RRQ || ob.opcode == WRQ) {
        for (int i = index; i < packetLength; i++) {
            if (packet[i] == '\0') {
                ob.fileNameLength = i - index + 1;
                break;
            }
        }
        ob.fileName = new char [ob.fileNameLength];
        memcpy ((void *) ob.fileName, (void *) &packet[index], ob.fileNameLength);
        index += ob.fileNameLength + 1;
        for (int i = index; i < packetLength; i++) {
            if (packet[i] == '\0') {
                ob.modeLength = i - index + 1;
                break;
            }
        }
        ob.mode = new char [ob.modeLength];
        memcpy ((void *) ob.mode, (void *) &packet[index], ob.modeLength);
    }
    else if (ob.opcode == DATA) {
        memcpy ((void *) &ob.blockNumber, (void *) &packet[index], sizeof (ob.blockNumber));
        ob.blockNumber = ntohs (ob.blockNumber);
        index += sizeof (ob.blockNumber);
        ob.dataLength = packetLength - index;
        ob.data = new char [ob.dataLength];
        memcpy ((void *) ob.data, (void *) &packet[index], ob.dataLength);
    }
    else if (ob.opcode == ACK) {
        memcpy ((void *) &ob.blockNumber, (void *) &packet[index], sizeof (ob.blockNumber));
        ob.blockNumber = ntohs (ob.blockNumber);
        index += sizeof (ob.blockNumber);
    }
    else if (ob.opcode == ERROR) {
        memcpy ((void *) &ob.blockNumber, (void *) &packet[index], sizeof (ob.blockNumber));
        ob.blockNumber = ntohs (ob.blockNumber);
        index += sizeof (ob.blockNumber);
        ob.dataLength = packetLength - index - 1;   // -1 for END (0x00)
        ob.data = new char [ob.dataLength];
        memcpy ((void *) ob.data, (void *) &packet[index], ob.dataLength);
    }
    else {
        ob.opcode = ob.blockNumber = -1;
        ob.fileNameLength = ob.modeLength = ob.dataLength = -1;
        ob.fileName = ob.mode = ob.data = NULL;
    }
    return;
}
