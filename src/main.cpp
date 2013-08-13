/*
 * main.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: Reuben
 */

#include <SDL/SDL.h>
#include <physim/headers/framer.hpp>
#include <physim/headers/vect.hpp>
#include <string>
#include <SDL/SDL_ttf.h>
#include <stdlib.h>
bool ended=false;

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
framer frm;
SDL_Event event;
mouse ms;
void handle_events();
using namespace std;
int main(int argc,char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	TTF_Font* font=TTF_OpenFont("physim/Fonts/lazy.ttf",28);
	SDL_Surface* scr=SDL_SetVideoMode(1080,720,32,SDL_SWSURFACE);
	SDL_Color TextColor;
	TextColor.r=255;
	TextColor.g=0;TextColor.b=0;
	string time;
	time.assign("test");
	while(!ended)
	{
		handle_events();
		SDL_Surface* timming=TTF_RenderText_Solid(font,time.c_str(),TextColor);
		timming->clip_rect.x=scr->clip_rect.w/2;
		timming->clip_rect.y=scr->clip_rect.h/2;
		SDL_FillRect(scr,&scr->clip_rect,0x999999);
		SDL_BlitSurface(timming,NULL,scr,&timming->clip_rect);
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
