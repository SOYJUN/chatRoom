#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H

#include <iostream>
#include <string>

#include "unp.h"
#include "Exception.h"

#define SERVER_IP		"127.0.0.1"
#define SERVER_PORT		9502

class TCPclient
{

public:
	
	void sendMsg()
	{
		string input;
		cout << "Enter your msg: ";
		cin >> input;
		if(write(sockfd, input.c_str(), strlen(input.c_str())) < 0) {
			throwError("[client]: writen error");
		}	
	}

	TCPclient() {
		connectToServer();
	}

   ~TCPclient() {}


private:

	// the sock fd used to communicate
	int sockfd;
	
	void connectToServer()
	{
		struct sockaddr_in servaddr;		

		if((sockfd = socket(AF_INET, SOCK_STREAM,  0)) < 0) {
			throwError("[client]: socket build error");
		}
		bzero(&servaddr, sizeof(servaddr));	
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(SERVER_PORT);
		if(inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) <= 0) {
			throwError("[client]: inet_pton() translate error");
		}	
		if(connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
			throwError("[client]: connect() error");	
		}
	}


};

#endif
