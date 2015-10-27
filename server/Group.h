/*	Here even the group chat or pair chat, use group chat to realize,
	the server is charged for the message transmitting.
*/

#ifndef _GROUP_H
#define _GROUP_H

#include <iostream>
#include <vector>

class Group
{

public:

	int getGroupId()
	{
		return id;
	}
	
	void addGroupMember(int fd)
	{
		fd_set.push_back(fd);
	}

	vector<int> getGroupMembers()
	{
		return fd_set;
	}
	
	Group() {}

	~Group() {} 


private:

	int id;
	
	// record all the connection fd in this group
	vector<int> fd_set;			

};

#endif
