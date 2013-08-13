/*
 * main.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: Reuben
 */

#include <SDL/SDL.h>
#include <physim/headers/framer.hpp>
#include <physim/headers/vect.hpp>
#include <aria/headers/global_assets.hpp>
#include <string>
#include <SDL/SDL_ttf.h>
#include <stdlib.h>

class mouse
{
public:
	SDL_Rect pos;
	mouse()
	{
		pos.x=pos.y=0;
	}
	void handle_events(SDL_Event event)
	{
		if(event.type==SDL_MOUSEMOTION)
		{
			pos.x=event.motion.x;
			pos.y=event.motion.y;
		}
	}
};
mouse ms;
framer frm;
void handle_events();
using namespace std;
int main(int argc,char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	font=TTF_OpenFont("physim/Fonts/lazy.ttf",28);
	scr=SDL_SetVideoMode(1080,720,32,SDL_SWSURFACE);
	graphicstring time;
	time.set("test");
	time.set(0,255,0);
	SDL_Delay(100);
	while(!ended)
	{
		handle_events();
		SDL_FillRect(scr,&scr->clip_rect,0x999999);
		time.set(frm.total_elapse());
		time.display();
		SDL_Flip(scr);
		frm.endframe();
		frm.smartwait();
	}
	SDL_Quit();
	return 1;
}
void handle_events()
{
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			ended=true;
		break;
		case SDL_MOUSEMOTION:
			ms.handle_events(event);
		break;
		}
	}
}
