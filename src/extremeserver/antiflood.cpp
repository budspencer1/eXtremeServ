/*
 * Project: eXtremeServ Sauerbraten
 * File Name: antiflood.cpp
 * Author: BudSpencer
 * Date: 02, 2017
 * 
 * Purpose: define antiflood functions
*/


#include"antiflood.h"

namespace server
{
	void estate::standard()
	{
		loopi(FT_NUMTYPES)
		{
			floodstate[i].event=0;
			floodstate[i].warning=0;
			floodstate[i].limit=0;
			floodstate[i].strikes=0;
		}
	}
};

namespace extreme
{
	inline size_t gettype(int type)
	{
		size_t ftype=FT_OTHER;
		switch((int)type)
		{
			case N_SAYTEAM:
			case N_TEXT:
				ftype=FT_TEXT;
				break;
			case N_SWITCHTEAM:
				ftype=FT_TEAM;
				break;
			case N_SWITCHMODEL:
				ftype=FT_MODEL;
				break;
			case N_SWITCHNAME:
				ftype=FT_NAME;
				break;
			case N_EDITVAR:
				ftype=FT_EDIT;
				break;
			default:
				break;
		}
		return ftype;
	}
	
	bool isflooding(server::clientinfo*ci,int type)
	{
		bool flood=false;
		size_t msg=gettype(type);
		fstate&fs=ci->state.exts.floodstate[msg];
		if((totalmillis2-fs.event)<FDELAY)fs.strikes++;
		if(fs.strikes>=STRIKE)
		{
			fs.limit=totalmillis2+FMUTE;
			fs.strikes=0;
		}
		if((totalmillis2-fs.limit)<0)
		{
			flood=true;
			if((totalmillis2-fs.warning)>FTRIGGER)
			{
				fs.warning=totalmillis2;
				string message;
				formatstring(message)("\f6Flood Protection: \f7You're muted for %d seconds due to spamming.", FMUTE / 1000);
				sendf(ci->clientnum, 1, "ris", N_SERVMSG, message);
			}
		}
		fs.event=totalmillis2;
		return flood;
	}
};
