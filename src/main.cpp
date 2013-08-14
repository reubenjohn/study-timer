/*
 * main.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: Reuben
 */

#include <SDL/SDL.h>
#include <physim/headers/framer.hpp>
#include <physim/headers/vect.hpp>
#include <headers/global_assets.hpp>
#include <headers/button.hpp>
#include <headers/mouse.hpp>
#include <string>
#include <SDL/SDL_ttf.h>


mouse ms;
framer frm;
timer t;
void handle_events(SDL_Event event);
using namespace std;
int main(int argc,char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	font=TTF_OpenFont("physim/Fonts/lazy.ttf",28);
	scr=SDL_SetVideoMode(1080,720,32,SDL_SWSURFACE);

	graphicstring time;
	button toggle,lap;
	toggle.graphictext.set("start/stop");
	lap.graphictext.set("lap");	lap.set(500,500);
	time.set("0");
	time.set(0,255,0);
	SDL_Delay(500);

	while(!ended)
	{
		toggle.refresh();
		lap.refresh();
		while(SDL_PollEvent(&event))
		{
			handle_events(event);
			toggle.handle_events(event);
			lap.handle_events(event);
		}
		if(toggle.state==-2)
			t.toggle();
		if(lap.state==-2)
			t.reset();

		SDL_FillRect(scr,&scr->clip_rect,0x999999);
		time.set(t.elapse());
		time.display();
		toggle.display();
		lap.display();

		SDL_Flip(scr);
		frm.endframe();
		frm.smartwait();
	}
	TTF_Quit();
	SDL_Quit();
	return 1;
}
void handle_events(SDL_Event event)
{
	switch(event.type)
	{
	case SDL_KEYDOWN:
		switch((unsigned int)event.key.keysym.sym)
		{
		case SDLK_RETURN:
			t.toggle();
		break;
		}
	break;
	case SDL_MOUSEMOTION:
		ms.handle_events(event);
	break;
	case SDL_QUIT:
		ended=true;
	break;
	}
}
