#include "TCPclient.h"

#define COMM_PORT 9502

int main()
{
	TCPclient client(COMM_PORT);
	
	while(1) {	
		client.sendMsg();
	}
	exit(0);
}
