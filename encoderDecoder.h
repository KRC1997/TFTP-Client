// author: Kunal Roychoudhury
// CS&T IIEST, Shibpur

# ifndef _ENCODER_DECODER_H_
# define _ENCODER_DECODER_H_

// header files required
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>    //for close()

# include "packetStructure.h"

// macros required by these classes and functions
# define TEXT "netascii\0"
# define BINARY "octet\0"
# define PACKET_SIZE 516

/*
    packet formats:
    RRQ/WRQ
    | 2 bytes opcode | n bytes filename | 1 byte 0x00 | mode = {"netascii", "octet"} | 1 byte 0x00 |
    DATA
    | 2 bytes opcode | 2 bytes block number | n bytes data (0 - 512) |  -> (all data packets contain 512 bytes except last (0-511))
    ACK
    | 2 bytes opcode | 2 bytes block number |
    ERROR
    | 2 bytes opcode | 2 bytes block number | n bytes error message | 1 byte 0x00 |
*/

enum _OPCODES_ {RRQ = 1, WRQ = 2, DATA = 3, ACK = 4, ERROR = 5};

class Codec {
private:
    void memcpy (void *, const void *, uint32_t);
    
public:
    void EncodePacket (char *, short &, Packet_Contents &);
    void DecodePacket (Packet_Contents &, char *, short);
};

# endif // _ENCODER_DECODER_H_
