/*
 * button.h
 *
 *  Created on: Aug 13, 2013
 *      Author: Reuben
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <aria/headers/global_assets.hpp>

using namespace std;
class button
{
public:
	int state;
	graphicstring* graphictext;
	SDL_Rect rect;
	void set(int x,int y,unsigned int w=400,unsigned int h=100)
	{
		if(x>0)
			rect.x=x;
		if(y>0)
			rect.y=y;
		rect.w=w;
		rect.h=h;
		graphictext->set_position(rect.x,rect.y+rect.h/4);
	}
	button(SDL_Surface* screen,const char* U="button")
	{
		graphictext=new graphicstring(screen);
		state=0;
		graphictext->set_position(rect.x,(rect.y+rect.h)/2);
		graphictext->set(U);
		set(100,100);
	}
	void display()
	{
		SDL_FillRect(scr,&rect,0x00FF00);
		graphictext->display();
	}
	int handle_events(SDL_Event event)
	{
		if(event.motion.x>rect.x&&event.motion.x<rect.x+rect.w&&event.motion.y>rect.y&&event.motion.y<rect.y+rect.h)
		{
			if(event.type==SDL_MOUSEBUTTONUP&&(state==-2||state==2))
			{
				state=-3;
			}
			else if(event.type==SDL_MOUSEBUTTONDOWN)
			{
				state=-2;
			}
			else
			{
				state=-1;
			}
		}
		else
			state=0;
		return state;
	}
	void refresh()
	{
		if(state<0)
			state*=-1;
	}
	void events_captured()
	{
		state=0;
	}
};
#endif /* BUTTON_H_ */
