#ifndef _MONITOR_H
#define _MONITOR_H

#include <iostream>
#include <string.h>
#include <map>
#include <limits.h>

#include "unp.h"
#include "Aux.h"
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
		fd_set				rset;
		
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
					int connfd = acceptConnection(it->second);
					
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
	//map<int, void*> func_table;

	// close all the listen fd in child process
	void closeAllFd()
	{
		traverse(port_table, it) {
			close(it->second);
		}
	}

};





#endif
