/*
 * Project: eXtremeServ Sauerbraten
 * File Name: eventhandler.cpp
 * Author: BudSpencer
 * Date: 02, 2017
 * 
 * Purpose: define eventhandler functions
*/


#include "eventhandler.h"

SVAR(event_ident, "eXtreme_EventInterpreter ");
SVAR(event_args, "");
SVAR(event_type, "");

namespace extreme 
{	
	void executeident(char *ident_, const char *eventtype, const char *args)
	{
		event_type = (char *)eventtype;
		event_args = (char *)args;
		execute(event_ident);
		event_type = newstring("");// reset values
		event_args = newstring("");// reset values
	}
	
	void executeevent(const char *eventtype, const char *arguments)
	{
		executeident(event_ident, eventtype, arguments);
	}
	
	const char *concateventargs(int count, ...)
	{
		const char *res = newstring("");
		va_list arguments;
		va_start(arguments, count);
		for(int i = 0; i < count; i++)
		{
			const char *temp = NULL;
			temp = va_arg(arguments, const char *);
			res = strcat((char *)res, temp);
			res = strcat((char *)res, " ");
		}
		va_end(arguments);
		return res;
	}
	
	// write int into char buffer
	const char *int2char(int x)
	{
		char *res = newstring("");
		sprintf(res, "%d", x);
		return res;
	}
	
	// write float into char buffer
	const char *float2char(float x)
	{
		char *res = newstring("");
		sprintf(res, "%g", x);
		return res;
	}
};










