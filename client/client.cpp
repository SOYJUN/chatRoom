#include "TCPclient.h"
#include "PRM.h"

#include <iostream>

int main()
{
	cout << "-------------------------------------------" << endl;
	cout << " W E L C O M E         C H A T R O O M     " << endl;
	cout << "-------------------------------------------" << endl;

	TCPclient client(COMM_PORT);
	
	client.runWriteThread();
	client.runReadThread();
		
	while(1);
	exit(0);
}
