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
#include <headers/global_assets.hpp>

using namespace std;
class button
{
public:
	int state;
	graphicstring graphictext;
	SDL_Rect rect;
	void set(int x,int y)
	{
		rect.x=x;
		rect.y=y;
		graphictext.set(rect.x,rect.y);
	}
	button(const char* U="button")
	{
		state=0;
		graphictext.set(U);
		rect.w=500;
		rect.h=100;
		set(100,100);
	}
	void display()
	{
		SDL_FillRect(scr,&rect,0x00FF00);
		graphictext.display();
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
