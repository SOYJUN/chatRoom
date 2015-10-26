#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H

#include <iostream>
#include <string>
#include <thread>

#include "unp.h"
#include "Aux.h"
#include "Exception.h"


class TCPclient
{

public:
	
	void runWriteThread()
	{
		writeThread = thread(&TCPclient::sendMsg, this);
	}

	void runReadThread()
	{
		readThread = thread(&TCPclient::recvMsg, this);
	}

	TCPclient(int port_num) 
	{
		sockfd = connectToServer(port_num);
	}

   ~TCPclient() 
	{
	    writeThread.join();
		readThread.join();	
		close(sockfd);
	}


private:

	// the sock fd used to communicate
	int sockfd;
	
	thread readThread; 
	thread writeThread;
	
	void sendMsg()
	{
		int nwrite = 0;
		char sendline[MAXLINE];
		
		while(1) {
			cout << "Enter your msg: ";
			scanf("%[^\n]%*c", sendline);
			if((nwrite = write(sockfd, sendline, strlen(sendline))) < 0) {
				throwError("[client]: write error");
			}	
			cout << "---Send out " << nwrite << "-bytes data" << endl;	
		}
	}

	void recvMsg()
	{
		int nread = 0;	
		char recvline[MAXLINE];
	
		while(1) {
			bzero(recvline, sizeof(recvline));
			nread = read(sockfd, recvline, sizeof(recvline));
			if(nread < 0) {
				throwError("[client]: readline error");
			} else if(nread == 0) {
				cout << "\n[WARNING]: Server has shut down" << endl;
				exit(0);
			}
			cout << "Receive data from server: " << recvline << endl; 
		}
	}

};

#endif
