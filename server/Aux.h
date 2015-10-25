#ifndef _AUX_H
#define _AUX_H

#include "unp.h"

void sig_chld(int signo)
{
	pid_t		pid;
	int			stat;

	while((pid = waitpid(-1, &stat, WNOHANG)) > 0);
}




#endif
