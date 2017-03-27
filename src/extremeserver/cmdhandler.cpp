/*
 * Project: eXtremeServ Sauerbraten
 * File Name: cmdhandler.cpp
 * Author: BudSpencer
 * Date: 02, 2017
 * 
 * Purpose: define command handler functions
*/

#include"cmdhandler.h"

SVAR(command_handler,"eXtreme_CommandHandler"); // command handler ident (cubescript)
SVAR(command_exec,"eXtreme_CommandHandler "); // command execute ident (cubescript)

namespace extreme
{
	inline bool checkforcommandhandler()
	{
		ident*id=getident(command_handler);
		if(id&&id->type==ID_ALIAS)
		{
			return true;
		}
		return false;
		printf("Error: Cannot execute Command Handler (%s)",command_handler);
	}

	void executecommandhandler()
	{
		if(!(bool)checkforcommandhandler())return;
		char*command=newstring(command_exec);
		execute(command);
	}
};
