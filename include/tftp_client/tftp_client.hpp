/*
Author: Kunal Roychoudhury
*/

/*
Refactoring process started on 19-NOV-2020

This header contains definitions that can be used to quickly develop applications
requiring to use tftp protocol to send or receive files.
*/

/*
Using this comment block to come up with a design for the library

The main interface should be a concrete class called 'tftpClient' This must
manage the underlying network access protocols to get to the remote server and
expose a simple interface to client code.

The methods in this interface must allow client code to:
    1) download a file from remote host - 'get'
    2) upload a file to remote host - 'put'
    3) get a list of file names available at remote host (if allowed by the 
        tftp) - 'getFileList'
    4) raise errors as appropriate
*/


#ifndef _TFTP_CLIENT_HPP_
#define _TFTP_CLIENT_HPP_

// standard library imports ---------------------------------------------------
#include <string>


namespace tftp_client
{
    // macro definitions ------------------------------------------------------
    #define __DEBUG__ true
    #define DEFAULT_TFTP_HOST_PORT 69

    /*
    The 'tftpClient' class abstracts all network & protocol access details from
    client code and provides a simple interface to send and receive files.
    */
    class tftpClient
    {
    public:
        // Constructor declarations -------------------------------------------
        tftpClient();

        // Connection Setup Functions -----------------------------------------
        /*
        Initializes the connection to remote host with provided information.
        */
        void connect
        (
            const std::string remote_node_ip, 
            const u_int32_t remote_node_port = DEFAULT_TFTP_HOST_PORT
        );
        
        // Client side interface ----------------------------------------------
        /*
        Downloads a file called 'remote_file_name' from the connected remote
        host server and writes it to a file pointed to by 'target_file_name'. 

        The 'target_file_name' must contain a valid name and appropriate extension
        and can contain the absolute or relative path prepended to it.
        */
        void get
        (
            const std::string remote_file_name,
            // file_name that has to be downloaded from remote server 
            const std::string target_file_name
            // file_name with absolute or relative path to which the downloaded
            // file must be written to
        );

        /*
        Uploads a file called 'file_name' to the connected remote host server.

        The 'file_name' must contain a valid name and appropriate extension and 
        can contain the absolute or relative path prepended to it.
        */
        void put
        (
            const std::string file_name
            // file_name with absolute or relative path on the client that must
            // be written to the remote server
        );

        void getFileList
        (

        ); // need to think about this
    };
} // namespace tftp_client


#endif // _TFTP_CLIENT_HPP_