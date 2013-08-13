/*
 * main.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: Reuben
 */

#include <SDL/SDL.h>


int main(int argc,char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* scr=SDL_SetVideoMode(1080,720,32,SDL_SWSURFACE);
	SDL_FillRect(scr,&scr->clip_rect,0x999999);
	SDL_Flip(scr);
	SDL_Delay(1000);
	SDL_Quit();
	return 1;
}
