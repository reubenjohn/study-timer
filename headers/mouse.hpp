/*
 * mouse.h
 *
 *  Created on: Aug 15, 2013
 *      Author: Reuben
 */

#ifndef MOUSE_H_
#define MOUSE_H_

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

#endif /* MOUSE_H_ */
