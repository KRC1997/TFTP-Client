// author: Kunal Roychoudhury
// CS&T IIEST, Shibpur

# ifndef _PACKET_STRUCTURE_H_
# define _PACKET_STRUCTURE_H_

// headers required
# include <iostream>
# include <string.h>

// macros required
# define _debug true
# define _printDebugMessage(s) if(_debug){cerr << s << endl;}

using namespace std;

class Packet_Contents {
private:
    short STRlen (const char *);
public:
    uint16_t opcode;       //  2 bytes
    uint16_t blockNumber;  //  2 bytes
    char *fileName;     //  n bytes
    short fileNameLength;   //  not part of packet
    char *mode;         //  6 - 9 bytes
    short modeLength;       //  not part of packet
    char *data;         //  n bytes     multiplexed with DATA and ERROR Message
    short dataLength;       //  not part of packet

    Packet_Contents ();
    Packet_Contents (unsigned short, unsigned short, const char *, const char *, const char *);
    Packet_Contents (const Packet_Contents &ob);
    ~ Packet_Contents ();
    void fill (unsigned short, unsigned short, const char *, const char *, const char *, unsigned short dataLength = 0);
    void printPacket ();
    void refresh ();
};

# endif // _PACKET_STRUCTURE_H_
