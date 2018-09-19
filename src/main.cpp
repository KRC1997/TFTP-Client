// author: Kunal Roychoudhury
// CS&T IIEST, Shibpur

# include <iostream>
# include <fstream>
# include <string.h>

# include "packetStructure.h"
# include "encoderDecoder.h"
# include "supportFunctions.h"

# define PROMPT "kunal_tftpc~: "

void readFile (char *, int, struct addrinfo *);
void writeFile (char *, int, struct addrinfo *);

char applicationPort[] = "69";    // this is the port number of the standard tftp client

int main (int argc, char *argv[]) {
    int errorStatus;
    const size_t hostNameSize = 50;
    char hostName[hostNameSize];
    char *executableFileName = argv[0];
    if (argc > 3) {
        cerr << "usage " << executableFileName <<" <Remote Node IP>" << endl;
        cerr << "\t \t OR" << endl;
        cerr << "usage " << executableFileName <<" <Remote Node IP> <Remote Node Port>" << endl;
        return (0);
    }

    char *remoteNodeIP = argv[1];
    char *remoteNodePort = applicationPort;
    if (argc == 3) {
        remoteNodePort = argv[2];
    }

    struct addrinfo *servInfo = getRemoteNodeInfo (remoteNodeIP, remoteNodePort, AF_INET, SOCK_DGRAM, AI_PASSIVE);
    struct addrinfo ptr;
    int clientSD = setupClientSocket (servInfo, ptr);
    if (clientSD == -1) {
        cerr << "Could not setup Socket" << endl;
        _printDebugMessage ("int clientSD = setupClientSocket (servInfo, ptr) failed!");
        return (0);     //  exit from main ()
    }
    else {cout << "\nClient Socket Descriptor: " << clientSD << endl;}
    errorStatus = gethostname (hostName, hostNameSize);
    cout << "Host Name: " << hostName << endl;
    struct sockaddr_in *tmp = (struct sockaddr_in *) &ptr.ai_addr;
    cout << "Port Number: " << tmp->sin_port << endl;

    cout << "Remote Node Port: " << remoteNodePort << endl;

    //  Packet_Contents (short opcode, short blockNumber, const char *fileName, const char *mode, const char *data)
    char buffer[PACKET_SIZE];
    short bufferLength;

    int commandListSize = 0;
    char commandBuffer [COMMAND_BUFFER_SIZE];

    Codec codec;
    Packet_Contents s_packet, r_packet;

    cout << endl;
    while (true) {
        cout << endl << PROMPT;
        takeCommandInput (commandBuffer);
        char ** commandList = breakCommandString (commandBuffer, &commandListSize);
        //printCommandList (commandList, commandListSize);
        if (strcmp (commandList[0], "exit\0") == 0) {
            free2D ((void **) commandList, commandListSize);
            break;
        }
        else if (strcmp (commandList[0], "get\0") == 0) {
            char *fileName = commandList[1];
            readFile (fileName, clientSD, &ptr);
        }
        else if (strcmp (commandList[0], "put\0") == 0) {
            char *fileName = commandList[1];
            writeFile (fileName, clientSD, &ptr);
        }
        else if (strcmp (commandList[0], "help\0") == 0){
            cout << "Commands Available -----" << endl;
            cout << "get : usage - 'get <filename>'" << endl;
            cout << "put : usage - 'put <filename>'" << endl << endl;
        }
        else {

        }
        free2D ((void **) commandList, commandListSize);
    }

    close (clientSD);
    delete ptr.ai_addr;
    return (0);
}

void readFile (char *filename, int SD, struct addrinfo *p) {
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    Codec codec;
    char buffer [PACKET_SIZE];
    short bufferLength;

    Packet_Contents s_packet (RRQ, 0, filename, BINARY, NULL), r_packet;
    codec.EncodePacket (buffer, bufferLength, s_packet);
    if (_debug){s_packet.printPacket();}

    int sentBytes = 0, recvBytes = 0;
    sentBytes = sendto (SD, buffer, bufferLength, 0, p->ai_addr, p->ai_addrlen);

    if (sentBytes < 0) {
        cerr << "Nothing Sent!" << endl;
        _printDebugMessage ("sentBytes = sendto (SD, buffer, bufferLength, 0, p->ai_addr, p->ai_addrlen) failed!");
        return;
    } else {
        if (_debug) {cout << "Read Request for '" << filename << "' sent ..." << endl;}
    }

    fstream file;
    file.open (filename, ios::binary | ios::out);
    if (!file) {
        cerr << "File could not be opened!" << endl;
        s_packet.refresh ();
        return;
    }

    do {
        recvBytes = recvfrom (SD, buffer, PACKET_SIZE, 0, (struct sockaddr *) &their_addr, &addr_len);
        if (recvBytes < 0) {
            cerr << "Nothing Received!" << endl;
        }
        codec.DecodePacket (r_packet, buffer, recvBytes);
        //  checking for error packet
        if (r_packet.opcode == ERROR) {
            cerr << "Error Occurred: " << r_packet.data << endl;
            file.close ();
            r_packet.refresh ();
            return;
        }
        else {
            if (_debug){cout << "Data Block " << r_packet.blockNumber << " received! Bytes = " << recvBytes << endl;}
            file.write (r_packet.data, r_packet.dataLength);
            s_packet.refresh ();
            s_packet.fill (ACK, r_packet.blockNumber, NULL, NULL, NULL);
            codec.EncodePacket (buffer, bufferLength, s_packet);
            //sentBytes = sendto (SD, buffer, bufferLength, 0, p->ai_addr, p->ai_addrlen);
            sentBytes = sendto (SD, buffer, bufferLength, 0, (struct sockaddr *) &their_addr, addr_len);
            if (sentBytes < 0) {
                cerr << "Nothing Sent!" << endl;
                _printDebugMessage ("sentBytes = sendto (SD, buffer, bufferLength, 0, p->ai_addr, p->ai_addrlen) failed!");
            } else {
                if (_debug) {cout << "ACK packet for Block " << s_packet.blockNumber << " sent! Bytes = " << sentBytes << endl;}
                memset ((void *) buffer, 0, PACKET_SIZE);
            }
        }
    }while (r_packet.dataLength >= 512);
    //  file received!
    file.close ();
    s_packet.refresh();
    r_packet.refresh();
    cout << "File Transfer Complete!" << endl;
    return;
}

void writeFile (char *filename, int SD, struct addrinfo *p) {
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    Codec codec;
    char buffer [PACKET_SIZE];
    short bufferLength;

    Packet_Contents s_packet (WRQ, 0, filename, BINARY, NULL), r_packet;
    codec.EncodePacket (buffer, bufferLength, s_packet);
    if (_debug){s_packet.printPacket();}

    int sentBytes = 0, recvBytes = 0;
    sentBytes = sendto (SD, buffer, bufferLength, 0, p->ai_addr, p->ai_addrlen);

    if (sentBytes < 0) {
        cerr << "Nothing Sent!" << endl;
        _printDebugMessage ("sentBytes = sendto (SD, buffer, bufferLength, 0, p->ai_addr, p->ai_addrlen) failed!");
        return;
    } else {
        if (_debug) {cout << "Write Request for '" << filename << "' sent ..." << endl;}
    }

    recvBytes = recvfrom (SD, buffer, PACKET_SIZE, 0, (struct sockaddr *) &their_addr, &addr_len);
    if (recvBytes < 0) {
        cerr << "Nothing Received!" << endl;
    } else {
        codec.DecodePacket (r_packet, buffer, recvBytes);
        if (r_packet.opcode == ACK) {
            if (_debug){cout << "ACK for Block " << r_packet.blockNumber << " received! Bytes = " << recvBytes << endl;}
        }
        else if (r_packet.opcode == ERROR) {
            cerr << "Error Occurred: " << r_packet.data << endl;
            r_packet.refresh ();
            return;
        } else {

        }
    }

    fstream file;
    file.open (filename, ios::in | ios::binary);
    if (!file) {
        cerr << "File could not be opened!" << endl;
        s_packet.refresh ();
        return;
    } else {
        cout << "Initializing Transfer!" << endl;
    }

    unsigned short BN = 1;
    do {
        s_packet.refresh ();
        char dataArray[512];
        memset ((void *) dataArray, 0, 512);
        file.read ((char *) dataArray, sizeof (dataArray));
        s_packet.fill (DATA, BN, NULL, NULL, dataArray, file.gcount ());
        codec.EncodePacket (buffer, bufferLength, s_packet);
        sentBytes = sendto (SD, buffer, bufferLength, 0, (struct sockaddr *) &their_addr, addr_len);
        if (sentBytes < 0) {
            cerr << "Nothing Sent!" << endl;
            _printDebugMessage ("sentBytes = sendto (SD, buffer, bufferLength, 0, (struct sockaddr *) &their_addr, addr_len) failed!");
        } else {
            cout << "Data Block " << s_packet.blockNumber << " sent! Bytes = " << sentBytes << endl;
            r_packet.refresh ();
            BN++;
        }

        recvBytes = recvfrom (SD, buffer, PACKET_SIZE, 0, (struct sockaddr *) &their_addr, &addr_len);
        if (recvBytes < 0) {
            cerr << "Nothing Received!" << endl;
        } else {
            codec.DecodePacket (r_packet, buffer, recvBytes);
            if (r_packet.opcode == ACK) {
                if (_debug){cout << "ACK for Block " << r_packet.blockNumber << " received! Bytes = " << recvBytes << endl;}
                r_packet.refresh ();
            }
            else if (r_packet.opcode == ERROR) {
                cerr << "Error Occurred: " << r_packet.data << endl;
                r_packet.refresh ();
                return;
            } else {

            }
        }
    }while (!file.eof ());
    file.close ();
    s_packet.refresh();
    r_packet.refresh();
    cout << "File Transfer Complete!" << endl;
    return;
}
