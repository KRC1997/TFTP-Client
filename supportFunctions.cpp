// author: Kunal Roychoudhury
// CS&T IIEST, Shibpur

# include "supportFunctions.h"

void printByte (char *ptr) {
    printf ("%c", *ptr & 0x80 ? '1':'0');
    printf ("%c", *ptr & 0x40 ? '1':'0');
    printf ("%c", *ptr & 0x20 ? '1':'0');
    printf ("%c", *ptr & 0x10 ? '1':'0');
    printf ("%c", *ptr & 0x08 ? '1':'0');
    printf ("%c", *ptr & 0x04 ? '1':'0');
    printf ("%c", *ptr & 0x02 ? '1':'0');
    printf ("%c", *ptr & 0x01 ? '1':'0');
    return;
}

void printBitMap (void *ptr, int numOfBytes) {
    if (false) {
        return;
    }
    else {
        //  ptr will point to an array with numOfBytes Bytes;
        char *tmp = (char *) ptr;
        while (numOfBytes != 0) {
            printByte (tmp);
            (tmp ++);
            numOfBytes --;
            printf ("|");
        }
        printf ("\b \n");
    }
    return;
}

void print_addrinfo (struct addrinfo *ptr) {
    cout << "\nContents of addrinfo:" << endl;
    cout << "ai_flags: " << ptr->ai_flags << endl;
    cout << "ai_family: " << ptr->ai_family << endl;
    cout << "ai_socktype: " << ptr->ai_socktype << endl;
    cout << "ai_protocol: " << ptr->ai_protocol << endl;
    cout << "ai_addrlen: " << ptr->ai_addrlen << endl;
    //cout << "ai_canonname: " << ptr->ai_canonname << endl;
    cout << "ai_addr->sa_family: " << ptr->ai_addr->sa_family << endl;
    //cout << "ai_addr->sa_data: " << ptr->ai_addr->sa_data << endl;
    cout << endl;
    return;
}

void copy_addrinfo (struct addrinfo &dest, struct addrinfo &src) {
    dest.ai_addr = new struct sockaddr;
    dest.ai_flags = src.ai_flags;
    dest.ai_family = src.ai_family;
    dest.ai_socktype = src.ai_socktype;
    dest.ai_protocol = src.ai_protocol;
    dest.ai_addrlen = src.ai_addrlen;
    memset ((void *) dest.ai_addr, 0, dest.ai_addrlen);
    dest.ai_canonname = src.ai_canonname;
    dest.ai_addr->sa_family = src.ai_addr->sa_family;
    memcpy ((void *) dest.ai_addr->sa_data, src.ai_addr->sa_data, 14);
    dest.ai_next = src.ai_next;
    return;
}

void takeCommandInput (char *commandBuffer) {
    fflush (stdin); //flush input buffer
    //fgets (commandBuffer, COMMAND_BUFFER_SIZE, stdin);
    //scanf ("%[^\n]%*c", commandBuffer);
    //The [] is the scanset character. [^\n] tells that while the input is not a newline ('\n') take input.
    //gets (commandBuffer);
    cin.getline (commandBuffer, COMMAND_BUFFER_SIZE);
    return;
}

void free2D (void **ptr, int listSize) {
    int i = 0;
    for (i = 0; i < listSize; i++){
        free (ptr[i]);
    }
    delete ptr;
    return;
}

char ** breakCommandString (char *commandBuffer, int *listSize){
    char **result = NULL;
    *listSize = 0;
    int commandBuffer_length = strlen (commandBuffer);
    if (commandBuffer_length == 0) {
        //  empty buffer
        return (result);
    }
    //  find number of words
    int i = 0, word_count = 0;
    while (i < commandBuffer_length){
        while (commandBuffer[i] != ' '){
            i++;
        }
        i++;    //to skip the corresponding ' ' character
        word_count++;
    }
    //printf ("\nWord Count: %d\n", word_count);

    result = new char *[word_count];
    for (i = 0; i < word_count; i++){
        result[i] = new char [COMMAND_LIST_WORD_SIZE];
    }

    //printf ("\nWord Count: %d\n", word_count);
    //  copy the words to the list
    char temp[COMMAND_LIST_WORD_SIZE];
    int temp_index = 0, result_index = 0; i = 0;
    while (i < commandBuffer_length){
        while (commandBuffer[i] != ' ' && commandBuffer[i] != '\0'){
            temp[temp_index ++] = commandBuffer[i ++];
        }
        temp[temp_index] = '\0';    //end the word wtih null character
        temp_index = 0;
        i++;    //to skip the corresponding ' ' character
        strcpy (result[result_index++], temp);
    }
    *listSize = word_count;
    return (result);
}

void printCommandList (char **commandList, int commandListSize) {
    if (_debug) {
        int i = 0;
        printf ("commandListSize: %d\n", commandListSize);
        for (i = 0; i < commandListSize; i++){
            printf ("commandList [%d]: %s\n", i, commandList[i]);
        }
    }
    return;
}

struct addrinfo * getRemoteNodeInfo (char *remoteNodeIP, char *remoteNodePort, short ipVersion, short socketType, short socketFlags) {
    struct addrinfo hints;              // to store socket details
    struct addrinfo *servinfo;          // pointer to start of linked list

    memset(&hints, 0, sizeof hints);    // make sure the struct is empty
    hints.ai_family = ipVersion;        // use IPv4
    hints.ai_socktype = socketType;     // UDP stream sockets
    hints.ai_flags = socketFlags;       // fill in my IP for me

    int status;
    if ((status = getaddrinfo (remoteNodeIP, remoteNodePort, &hints, &servinfo)) != 0) {
        cerr << "getaddrinfo: " << gai_strerror (status) << endl;
        _printDebugMessage ("status = getaddrinfo (remoteNodeIP, remoteNodePort, &hints, &servinfo) failed!");
        return (NULL);    //  ERROR point 1
    }
    else {
        _printDebugMessage ("status = getaddrinfo (remoteNodeIP, remoteNodePort, &hints, &servinfo) successfull!");
        return (servinfo);
        //  this is the pointer to start of the linked list that holds details of the remote node
    }
}

int setupClientSocket (struct addrinfo *servinfo, struct addrinfo &ptr) {
    int clientSD;
    struct addrinfo *p = NULL;
    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((clientSD = socket (p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror ("socket");
            continue;   //  skip the address
        }
        break;  //statement encountered then socket creation succesfull
    }
    if (p == NULL) {
        // looped off the end of the list with no successful bind
        cerr << "Failed to Bind Socket" << endl;
        _printDebugMessage ("(clientSD = socket (p->ai_family, p->ai_socktype, p->ai_protocol)) failed!");
        close (clientSD);
        return (-1);    //  ERROR point 1
    }
    else {
        _printDebugMessage ("(clientSD = socket (p->ai_family, p->ai_socktype, p->ai_protocol)) successfull!");
        copy_addrinfo (ptr, *p);
        freeaddrinfo (servinfo);
        return (clientSD);
    }
}
