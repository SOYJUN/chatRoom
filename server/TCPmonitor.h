#ifndef _MONITOR_H
#define _MONITOR_H

#include <iostream>
#include <string.h>
#include <map>
#include <limits.h>

#include "unp.h"
#include "Exception.h"
#include "Service.h"

#define traverse(c, it) for(auto (it) = (c).begin(); (it) != (c).end(); (it)++)


class TCPmonitor
{

public:

	void addMonitor(int port_num)
	{
		int listenfd = initiateMonitor(port_num);	
		port_table.insert({port_num, listenfd});	
		maxfd = max(maxfd, listenfd);
	}

	void monitorRequest()
	{
		int					maxfdp1;
		socklen_t			len;
		char				buff[MAXLINE];
		struct sockaddr_in	cliaddr;
		fd_set				rset;
		
		len = sizeof(cliaddr);
		FD_ZERO(&rset);

		while(1) {
			if(maxfd == -1) throwError("[server]: no monitor fd set is empty");
			maxfdp1 = maxfd + 1;
			// add all the listen fd into set
			traverse(port_table, it) FD_SET(it->second, &rset);	
			cout << "Waiting for incoming request..." << endl;		
			if(select(maxfdp1, &rset, NULL, NULL, NULL) < 0) {
				if(errno == EINTR)	continue;
				else				throwError("[server]: select error");
			}	
			
			// monitor all the added listen fd
			traverse(port_table, it) {
				if(FD_ISSET(it->second, &rset)) {
					pid_t		pid;
					int			connfd;

					bzero(&cliaddr, sizeof(cliaddr));
					if((connfd = accept(it->second, (SA*)&cliaddr, &len)) < 0) {
						throwError("[server]: accept error");
					}
					cout << "Connection from " << inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff));
					cout << ", port: " << ntohs(cliaddr.sin_port) << endl;
					if((pid = fork()) == 0) {
						closeAllFd();
						Service service;
						if(it->first == 9502) service.receiver(connfd); 
					}
					close(connfd);
				}
			}
		}
	}

	TCPmonitor() 
	{
		maxfd = -1;
	}

   ~TCPmonitor() {}



private:

	// dynamic update the max fd in each operation of port_table
	int maxfd; 

	// here use hash map to record the <port num, listen fd> pair
	map<int, int> port_table;	
	// this map record the <listen fd, thread function> pair
	map<int, void*> func_table;

	// return a configured listen fd
	int initiateMonitor(int port_num, const char* interface = "INADDR_ANY")
	{
		int					listenfd;
		const int			on = 1;
		struct sockaddr_in	servaddr;
		
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
			throwError("[server]: bind error");
		}
		if(listen(listenfd, LISTENQ) < 0) {
			throwError("[server]: listen error");
		}

		return listenfd;
	}

	// close all the listen fd in child process
	void closeAllFd()
	{
		traverse(port_table, it) {
			close(it->second);
		}
	}

};





#endif
