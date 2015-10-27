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
		int					maxfdp1, connfd;
		fd_set				rset;
		
		FD_ZERO(&rset);

		while(1) {
			if(maxfd == -1) throwError("[server]: no monitor, fd set is empty");
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
					connfd = acceptConnection(it->second);
					cout << "before added, table size: " << conn_table->size() << endl;
					addConnFd(connfd, idAllo++);
					cout << "after added, table size: "<< conn_table->size() << endl;	
					if((pid = fork()) == 0) {
						closeAllFd();
						Service service(connfd);
						service.runGroupChatThread(conn_table); 
						while(1); // block the main thread here
						closeConnFd(connfd);
						exit(0);
					}
					closeConnFd(connfd);
				}
			}
		}
	}

	TCPmonitor() 
	{
		conn_table = new map<int, int>();
		idAllo = 0;
		maxfd = -1;
	}

	~TCPmonitor() 
	{
		delete(conn_table);
	}



private:

	// use to allocate the id for the new coming connection
	int idAllo;

	// dynamic update the max fd in each operation of port_table
	int maxfd; 

	// here use hash map to record the <port num, listen fd> pair
	map<int, int> port_table;	

	// here use hash map to record the <connect fd, port_num> pair
	map<int, int> *conn_table;


	// close all the listen fd in child process
	void closeAllFd()
	{
		traverse(port_table, it) {
			close(it->second);
		}
	}

	void addConnFd(int fd, int port_num)
	{
		conn_table->insert({fd, port_num});
		cout << &fd << " is inserted to table" << endl;
	}

	void closeConnFd(int fd)
	{
		if(conn_table->find(fd) != conn_table->end()) {
			conn_table->erase(conn_table->find(fd));
		}	
		close(fd);
	}

};





#endif
