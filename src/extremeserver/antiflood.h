/*
 * Project: eXtremeServ Sauerbraten
 * File Name: antiflood.h
 * Author: BudSpencer
 * Date: 02, 2017
 * 
 * Purpose: define antiflood header
*/

#ifndef ANTIFLOOD_H
#define ANTIFLOOD_H

#include"game.h"
#include"playerstates.h"

//macros to set main vars of flood protection
#define FDELAY 500
#define STRIKE 5
#define FMUTE 10000
#define FTRIGGER 20000 

namespace server
{
	struct clientinfo;
	struct estate;
	struct fstate;
};

namespace extreme
{
	typedef server::clientinfo clientinfo;
	typedef server::estate estate;
	typedef server::fstate fstate;
	enum{FT_OTHER=0,FT_TEXT,FT_NAME,FT_MODEL,FT_TEAM,FT_EDIT,FT_NUMTYPES};
	inline size_t gettype(int type);
	bool isflooding(server::clientinfo*ci,int type);

};

#endif

