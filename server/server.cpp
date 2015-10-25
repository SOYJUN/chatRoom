#include "TCPmonitor.h"
#include "Aux.h"

#define COMM_PORT   9502

int main()
{
	signal(SIGCHLD, sig_chld);

	TCPmonitor monitors;
	
	monitors.addMonitor(COMM_PORT);
	
	monitors.monitorRequest();

}
