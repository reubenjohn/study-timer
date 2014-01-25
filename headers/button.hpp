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
	SDL_Surface* scr;
	int state;
	SDL_Rect rect;
	Uint32 color;
public:
	GRAPHIC_STRING* graphic_text;
	bool input_fresh()
	{
		return state>0;
	}
	bool interacted()
	{
		return state!=0;
	}
	bool hovered()
	{
		return state==1;
	}
	bool being_pressed()
	{
		return state==2;
	}
	bool pressed()
	{
		return state==3;
	}
	void set_dimensions(int x,int y,unsigned int w=400,unsigned int h=100)
	{
		if(x>0)
			rect.x=x;
		if(y>0)
			rect.y=y;
		rect.w=w;
		rect.h=h;
		graphic_text->set_position(rect.x+(rect.w/2-graphic_text->rectangle().w/2),rect.y+rect.h/4);
	}
	void set_dimensions(vect pos)
	{
		set_dimensions(pos.x,pos.y);
	}
	void set_dimensions(vect pos,vect dimensions)
	{
		set_dimensions(pos.x,pos.y,dimensions.x,dimensions.y);
	}
	void set_color(short unsigned int r,short unsigned int g,short unsigned int b)
	{
		color=SDL_MapRGB(scr-> format,r,g,b);
	}
	void display()
	{
		if(scr)
			SDL_FillRect(scr,&rect,color);
		else
		{
			ofstream fout("logs/allocation log.txt",ios::app);
			fout<<"button::display scr NULL access\n";
			fout.close();
		}
		if(graphic_text)
			graphic_text->display();
		else
		{
			ofstream fout("logs/allocation log.txt",ios::app);
			fout<<"button::display graphic_text NULL access\n";
			fout.close();
		}
	}
	int handle_events(SDL_Event event)
	{
		if(event.motion.x>rect.x&&event.motion.x<rect.x+rect.w&&event.motion.y>rect.y&&event.motion.y<rect.y+rect.h)
		{
			if(event.type==SDL_MOUSEBUTTONUP&&(state==-2||state==2))
			{
				state=3;
			}
			else if(event.type==SDL_MOUSEBUTTONDOWN)
			{
				state=2;
			}
			else
			{
				state=1;
			}
		}
		else
			state=0;
		return state;
	}
	void refresh()
	{
		if(state>=0)
			state*=-1;
	}
	void events_captured()
	{
		state=0;
	}
	button(SDL_Surface* screen,TTF_Font* U_font=NULL,const char* text="button",unsigned int graphic_update_interval=50)
	{
		if(scr)
		{
			scr=screen;
			color=SDL_MapRGB(scr-> format,255,0,0);
		}
		ofstream fout("logs/allocation log.txt",ios::app);
		graphic_text=new GRAPHIC_STRING(screen,U_font,graphic_update_interval);
		if(graphic_text)
		{
			fout<<"Button->GRAPHIC_STRING allocated\n";
		}
		else
			fout<<"Button->GRAPHIC_STRING allocation failed!\n";
		fout.close();
		state=0;
		graphic_text->set_position(rect.x,(rect.y+rect.h)/2);
		*graphic_text=text;
		set_dimensions(100,100);
	}
};
#endif /* BUTTON_H_ */
