// author: Kunal Roychoudhury
// CS&T IIEST, Shibpur

# ifndef _SUPPORT_FUNCTIONS_H_
# define _SUPPORT_FUNCTIONS_H_

// headers required
# include <iostream>
# include <string.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>    //for close()

# include "encoderDecoder.h"

// macros required by these functions
# define COMMAND_BUFFER_SIZE 100
# define COMMAND_LIST_WORD_SIZE 20

using namespace std;

void printByte (char *);
void printBitMap (void *, int);
void print_addrinfo (struct addrinfo *);
void copy_addrinfo (struct addrinfo &, struct addrinfo &);
void takeCommandInput (char *);
void free2D (void **, int);
char ** breakCommandString (char *, int *);
void printCommandList (char **, int);
struct addrinfo * getRemoteNodeInfo (char *, char *, short, short, short);
int setupClientSocket (struct addrinfo *, struct addrinfo &);

# endif // _SUPPORT_FUNCTIONS_H_
