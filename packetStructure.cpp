// author: Kunal Roychoudhury
// CS&T IIEST, Shibpur

# include "packetStructure.h"

short Packet_Contents::STRlen (const char *string) {
    if (string == NULL) {
        return (-1);
    }
    else {
        short i = 0;
        for (i = 0; string[i] != '\0'; i ++);
        return (i+1);
    }
}

Packet_Contents::Packet_Contents () {
    this->opcode = this->blockNumber = 0;
    this->fileName = this->mode = this->data = NULL;
    this->fileNameLength = this->modeLength = this->dataLength = -1;
}

Packet_Contents::Packet_Contents (unsigned short opcode, unsigned short blockNumber, const char *fileName, const char *mode, const char *data) {
    this->opcode = opcode;
    this->blockNumber = blockNumber;
    this->fileNameLength = STRlen (fileName);
    this->modeLength = STRlen (mode);
    this->dataLength = STRlen (data);

    if (this->fileNameLength >= 0) {
        this->fileName = new char [this->fileNameLength];
        strcpy (this->fileName, fileName);
    }
    else {
        this->fileName = NULL;
    }

    if (this->modeLength >= 0) {
        this->mode = new char [this->modeLength];
        strcpy (this->mode, mode);
    }
    else {
        this->mode = NULL;
    }

    if (this->dataLength >= 0) {
        this->data = new char [this->dataLength];
        strcpy (this->data, data);
    }
    else {
        this->data = NULL;
    }
}

Packet_Contents::Packet_Contents (const Packet_Contents &ob) {
    this->opcode = ob.opcode;
    this->blockNumber = ob.blockNumber;
    this->fileNameLength = ob.fileNameLength;
    this->modeLength = ob.modeLength;
    this->dataLength = ob.dataLength;

    if (this->fileNameLength >= 0) {
        this->fileName = new char [this->fileNameLength];
        strcpy (this->fileName, ob.fileName);
    }
    else {
        this->fileName = NULL;
    }

    if (this->modeLength >= 0) {
        this->mode = new char [this->modeLength];
        strcpy (this->mode, ob.mode);
    }
    else {
        this->mode = NULL;
    }

    if (this->dataLength >= 0) {
        this->data = new char [this->dataLength];
        strcpy (this->data, ob.data);
    }
    else {
        this->data = NULL;
    }
}

Packet_Contents::~Packet_Contents () {
    delete this->fileName;
    delete this->mode;
    delete this->data;
}

void Packet_Contents::fill (unsigned short opcode, unsigned short blockNumber, const char *fileName, const char *mode, const char *data, unsigned short dataLength) {
    this->opcode = opcode;
    this->blockNumber = blockNumber;
    this->fileNameLength = STRlen (fileName);
    this->modeLength = STRlen (mode);
    if (dataLength == 0) {
        this->dataLength = STRlen (data);
    }else {
        this->dataLength = dataLength;
    }

    if (this->fileNameLength >= 0) {
        this->fileName = new char [this->fileNameLength];
        strcpy (this->fileName, fileName);
    }
    else {
        this->fileName = NULL;
    }

    if (this->modeLength >= 0) {
        this->mode = new char [this->modeLength];
        strcpy (this->mode, mode);
    }
    else {
        this->mode = NULL;
    }

    if (this->dataLength >= 0) {
        this->data = new char [this->dataLength];
        //strcpy (this->data, data);
        memcpy ((void *) this->data, (void *) data, this->dataLength);
    }
    else {
        this->data = NULL;
    }
}

void Packet_Contents::printPacket () {
    cout << "\nOpcode: " << this->opcode << "\tBlock Number: " << this->blockNumber << endl;
    cout << "File Name Length: " << this->fileNameLength << "\tMode Length: " << this->modeLength;
    cout << "\tData Length: " << this->dataLength << endl;
    if (this->fileName != NULL) {
        cout << "File Name: " << this->fileName;
    }
    else {
        cout << "File Name: " << "-N/A-";
    }
    if (this->mode != NULL) {
        cout << "\tMode: " << this->mode << endl;
    }
    else {
        cout << "\tMode: " << "-N/A-" << endl;
    }
    if (this->data != NULL) {
        cout << "Data |" << this->data << "|" << endl;
    }
    else {
        cout << "Data | -N/A- |" << endl;
    }
    cout << endl;
    return;
}

void Packet_Contents::refresh () {
    delete this->fileName;
    delete this->mode;
    delete this->data;
    this->opcode = this->blockNumber = 0;
    this->fileName = this->mode = this->data = NULL;
    this->fileNameLength = this->modeLength = this->dataLength = -1;
}
