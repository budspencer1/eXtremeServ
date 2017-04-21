/*
 * Project: eXtremeServ Sauerbraten
 * File Name: eventhandler.cpp
 * Author: BudSpencer
 * Date: 04, 2017
 * 
 * Purpose: define eventhandler functions
*/


#include "eventhandler.h"

SVAR(event_type, "");
SVAR(event_args, "");

namespace extreme 
{	
	char *eventarglist(int count, ...)
	{
		va_list args;
		va_start(args, count);
		char *text = newstring("", MAXTRANS);
		for(int i = 0; i < count; i++)
		{
			text = strcat(text, va_arg(args, const char*));
			text = strcat(text, " ");
		}
		va_end(args);
		return text;
	}

	int executeeventident()
	{
		int ret = execute("do ( getalias eXtreme_EventInterpreter )");
		return ret;
	}

	void executeevent(const char *eventtype, char *arglist)
	{
		event_type = (char *)eventtype;
		event_args = arglist;
		executeeventident();
		event_type = newstring("");
		event_args = newstring("");
	}
	
	// for text events like "ontext" or "onteamchat"
	void executetextevent(const char *eventtype, int cn, char text[5000])
	{
		char txt[5000] = { "do ( eXtreme_EventInterpreter " };
		event_type = (char *)eventtype;
		strcat(txt, int2char(cn));
		strcat(txt, " ");
		strcat(txt, escapestring(text));
		strcat(txt, " )");
		execute(txt);
	}
	
	const char *int2char(int x)
	{
		char *result = newstring("", MAXTRANS);
		sprintf(result, "%d", x);
		return (const char*)result;
	}

	const char *float2char(float x)
	{
		char *result = newstring("", MAXTRANS);
		sprintf(result, "%g", x);
		return (const char*)result;
	}
	
	// only for debugging
	inline void test()
	{
		char txt[5000] = { "<enter test text here>" };
		executetextevent("ontext", 0, txt);
	}
	//COMMAND(test, "");
};







