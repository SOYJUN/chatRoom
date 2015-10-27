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
		recvThread = thread(&Service::recvMsg, this);
	}

	void runTimerThread()
	{
		timerThread = thread(&Service::timer, this);	
	}

	void runGroupChatThread(map<int, int> *conn_table)
	{
		groupChatThread = thread(&Service::groupChat, this, conn_table); 
	}

	Service(int fd) 
	{
		connfd = fd;
	}
   
	~Service() 
	{
		recvThread.join();
		timerThread.join();
		groupChatThread.join();
	}

private:

	int		connfd;

	thread	recvThread;
	thread	timerThread;
	thread	groupChatThread;

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

	void recvMsg()
	{
		int		nread = 0;
		char    buff[MAXLINE];

		while(1) {	
			bzero(buff, sizeof(buff));
			nread = read(connfd, buff, sizeof(buff));
			if(nread < 0) {
				throwError("Service<receiver>: read error");
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

	void groupChat(map<int, int> *conn_table)
	{
		int		nwrite = 0, nread = 0;
		char	recvline[MAXLINE], buff[MAXLINE];

		//block in the read, until some message receive from the client then transmit to others
		while(1) {
			bzero(recvline, sizeof(recvline));
			nread = read(connfd, recvline, sizeof(recvline));
			if(nread < 0) {
				throwError("Service<receiver>: read error");
			} else if(nread == 0) {
				cout << "\n[INFO]: Client has shut down" << endl;
				exit(0);
			} 
			snprintf(buff, sizeof(buff), "client<%d>: ", conn_table->find(connfd)->second);	
			strcat(buff, recvline);

			for(auto it=conn_table->begin(); it!=conn_table->end(); it++) {
				if((nwrite = write(it->first, buff, strlen(buff))) < 0) {
					throwError("Service<groupChat>: write error");
				}			
				cout << "Transmit info to the client<" << it->second << ">." << endl;
			}
		}
	}	

	

};





#endif
