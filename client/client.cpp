#include "TCPclient.h"
#include "PRM.h"


int main()
{
	TCPclient client(COMM_PORT);
	
	client.runWriteThread();
	client.runReadThread();
		
	while(1);
	exit(0);
}
