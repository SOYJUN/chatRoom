#ifndef _SERVICE_H
#define _SERVICE_H

#include <iostream>

#include "unp.h"

using namespace std;

class Service
{

public:

	void receiver(int connfd)
	{
		int nread;
		char recvline[MAXLINE];

		cout << "Enter receiver service: " << endl;
		
		while(1) {
			bzero(recvline, sizeof(recvline));
			nread = read(connfd, recvline, sizeof(recvline));
			if(nread < 0) {
				throwError("[server]: readline error");
			} else if(nread == 0) {
				cout << "[INFO]: Client has shut down" << endl;
				exit(0);
			} 
			cout << "Receive " << nread <<  "-bytes message from client: " <<  recvline << endl;
		}
		close(connfd);
		exit(0);
	}

	Service() {}
   
   ~Service() {}

private:



};





#endif
