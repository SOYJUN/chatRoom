#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H

#include <iostream>
#include <string>

#include "unp.h"
#include "Exception.h"

#define SERVER_IP		"127.0.0.1"

class TCPclient
{

public:
	
	void sendMsg()
	{
		int nwrite = 0;
		char inputline[MAXLINE];

		cout << "Enter your msg: ";
		scanf("%[^\n]%*c", inputline);
		if((nwrite = write(sockfd, inputline, strlen(inputline))) < 0) {
			throwError("[client]: write error");
		}	
		cout << "---Send out " << nwrite << "-bytes data" << endl;	
	}

	TCPclient(int port_num) {
		connectToServer(port_num);
	}

   ~TCPclient() {}


private:

	// the sock fd used to communicate
	int sockfd;
	
	void connectToServer(int port_num)
	{
		struct sockaddr_in servaddr;		

		if((sockfd = socket(AF_INET, SOCK_STREAM,  0)) < 0) {
			throwError("[client]: socket build error");
		}
		bzero(&servaddr, sizeof(servaddr));	
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port_num);
		if(inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) <= 0) {
			throwError("[client]: inet_pton() translate error");
		}	
		if(connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
			throwError("[client]: connect() error");	
		}
	}


};

#endif
