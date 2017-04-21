/*
 * Project: eXtremeServ Sauerbraten
 * File Name: eventhandler.h
 * Author: BudSpencer
 * Date: 04, 2017
 * 
 * Purpose: define event header
*/

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "game.h"
#include "playerstates.h"

namespace extreme 
{
	char *eventarglist(int count, ...);	
	void executeevent(const char *eventtype, char *arglist);
	void executetextevent(const char *eventtype, int cn, char text[5000]);
	int executeeventident(const char *name);
	const char *int2char(int x);
	const char *float2char(float x);
	inline void test();//test command
};

#endif

