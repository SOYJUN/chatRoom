#ifndef _MONITOR_H
#define _MONITOR_H

#include <iostream>
#include <string.h>
#include <map>

#include "unp.h"
#include "readLine.h"
#include "Exception.h"


class TCPmonitor
{

public:

	void addMonitor(int port_num)
	{
		int listenfd = initiateMonitor(port_num);	
		port_table.insert({port_num, listenfd});	
	}

	void removeMonitor(int port_num) 
	{
		port_table.erase(port_num);	
	}

	void clearMonitor() 
	{
		port_table.clear();
	}

	void monitorRequest()
	{
		int connfd;
		socklen_t len;
		char buff[MAXLINE];
		struct sockaddr_in cliaddr;
		
		len = sizeof(cliaddr);
		while(1) {
			cout << "Waiting for incoming request..." << endl;		
			if((connfd = accept(listenfd, (SA*)&cliaddr, &len)) < 0) {
				throwError("[server]: accept error");
			}
			cout << "Connection from " << inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)) << ", port: " << ntohs(cliaddr.sin_port) << endl;
			bzero(buff, sizeof(buff));
			if(readLine(connfd, buff, sizeof(buff)) < 0) {
				throwError("[server]: readLine error");
			}
			cout << "Receive from client: " << buff << endl;	
		}
	}

	TCPmonitor() {}

   ~TCPmonitor() {}



private:

	// here use hash map to record the <port num, listen fd> pair
	map<int, int> port_table;	

	// return a configured listen fd
	int initiateMonitor(int port_num, const char* interface = "INADDR_ANY")
	{
		const int on = 1;
		struct sockaddr_in servaddr;
		
		if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			throwError("[server]: socket build error");
		}
		if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
			throwError("[server]: set socket option<SO_REUSEADDR> error");
		}
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		if(strcmp(interface, "INADDR_ANY") == 0) {
			servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		} else {
			if(inet_pton(AF_INET, interface, &servaddr.sin_addr) <= 0) {
				throwError("[server]: inet_pton() translate error");
			}				
		}
		servaddr.sin_port = htons(port_num);
		if(bind(listenfd, (SA*)&servaddr, sizeof(servaddr)) < 0) {
			throwError("[server]: bind error" + errno);
		}
		if(listen(listenfd, LISTENQ) < 0) {
			throwError("[server]: listen error");
		}

		return listenfd;
	}


};





#endif
