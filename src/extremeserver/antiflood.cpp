/*
 * Project: eXtremeServ Sauerbraten
 * File Name: antiflood.c
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
		if(((int)totalmillis-fs.event)<FDELAY)fs.strikes++;
		if(fs.strikes>=STRIKE)
		{
			fs.limit=totalmillis+FMUTE;
			fs.strikes=0;
		}
		if(((int)totalmillis-fs.limit)<0)
		{
			flood=true;
			if(((int)totalmillis-fs.warning)>FTRIGGER)
			{
				fs.warning=totalmillis;
				server::sendservmsg("fUkIN FLOODA SPAST,oio,xd,lol");
			}
		}
		fs.event=totalmillis;
		return(bool)flood;
	}
};
