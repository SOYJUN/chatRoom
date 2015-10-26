#ifndef _SERVICE_H
#define _SERVICE_H

#include <iostream>
#include <thread>
#include <time.h>

#include "unp.h"
#include "Aux.h"

using namespace std;

class Service
{

public:

	void runRecvThread()
	{
		recvThread = thread(&Service::receiver, this);
	}

	void runTimerThread()
	{
		timerThread = thread(&Service::timer, this);	
	}

	Service(int fd) 
	{
		connfd = fd;
	}
   
   ~Service() 
	{
		recvThread.join();
	}

private:

	int connfd;

	thread recvThread;
	thread timerThread;
	
	// not to use util the semaphore added
	void echor() 
	{
		int		nwrite = 0;
		char	buff[MAXLINE];
		
		if((nwrite = write(connfd, buff, strlen(buff))) < 0) {
			throwError("Service<echor>: write error");
		}			
		cout << "Echo to the client" << endl;
	}

	void receiver()
	{
		int		nread = 0;
		char	buff[MAXLINE];

		cout << "Enter receiver service: " << endl;
		while(1) {	
			bzero(buff, sizeof(buff));
			nread = read(connfd, buff, sizeof(buff));
			if(nread < 0) {
				throwError("Service<receiver>: readline error");
			} else if(nread == 0) {
				cout << "\n[INFO]: Client has shut down" << endl;
				exit(0);
			} 
			cout << "Receive " << nread <<  "-bytes message from client: " << buff << endl;
		}
	}

	void timer()
	{
		int			n;
		char		buff[MAXLINE];
		fd_set		rset;
		struct timeval	timeout;
	
		FD_ZERO(&rset);

		//loop to report time every 5 sec
		while(1){
			FD_SET(connfd, &rset);
			timeout.tv_sec = 5;
			timeout.tv_usec = 0;

			n = select(connfd+1, &rset, NULL, NULL, &timeout);
			if(n < 0) {
				throwError("Service<timer>: select error...");
			} else if(n == 0) {
				getDayTime(buff, sizeof(buff));
				if(write(connfd, buff, strlen(buff)) < 0){
					throwError("Service<timer>: write error...");
				}		
			}
		}
	}

};





#endif
