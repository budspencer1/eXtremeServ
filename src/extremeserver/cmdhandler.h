/*
 * Project: eXtremeServ Sauerbraten
 * File Name: cmdhandler.h
 * Author: BudSpencer
 * Date: 03, 2017
 * 
 * Purpose: define command handler functions
*/

#ifndef CMDHANDLER_H
#define CMDHANDLER_H	

#include"game.h"
#include"playerstates.h"

namespace extreme
{
	inline bool checkforcommandhandler();
	void executecommandhandler();
};

#endif

