/*
 * Project: eXtremeServ Sauerbraten
 * File Name: eventhandler.h
 * Author: BudSpencer
 * Date: 02, 2017
 * 
 * Purpose: define event header
*/

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "game.h"
#include "playerstates.h"

extern void concat(tagval *v, int n);

namespace extreme 
{
	void executeident(const char *ident_, const char *eventtype, const char *args);
	void executeevent(const char *eventtype, const char *arguments);
	const char *concateventargs(int count, ...);

	const char *int2char(const int x);
	const char *float2char(const float x);
};

#endif

