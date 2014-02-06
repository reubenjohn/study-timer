/*
 * main.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: Reuben
 */

#include <headers/study timer.h>

HWND hwnd;
SDL_Rect lap_pos={750,10};
Mix_Chunk *beat,*beat_z,*beep,*welcome;
using namespace std;
int main(int argc,char* args[])
{
	STUDY_TIMER T((vect){1080,720,32},"Fonts/KeraterMedium.ttf",32);
	T.updatefpslimits(10,20);
	if(T.first_run())
	{
		T.perform_first_run();
		T.mark_as_run_before();
	}
	else
	{
		T.load_timer_elements();
	}
	while(!T.quit)
	{
		//..........................................................initialisation
		T.initiate_frame();
		//..........................................................

		//..........................................................user
		while(T.poll_event()&&!T.quit)
		{
			T.handle_all_events();
		}
		//save processing when pause
		/*else
		{
			do
			{
				T.initiate_frame();
				T.display_timer_elements();
				while(T.wait_event()&&!T.quit)
					T.handle_all_events();
				if(T.toggle.being_pressed())
				{
					T.toggle_timer();
					T.initiate_frame();
				}
				T.terminate_frame();
			}while(!T.quit&&!T.timer_running());
		}
		*/
		if(T.target_b.being_pressed())
		{
			T.set_need_target(true);
			if(!T.user_completed())
			{
				T.target_b.set_color(150,150,255);
			}
		}
		if(T.save_state.pressed())
		{
			T.save_to_file();
		}
		if(T.load.being_pressed())
		{
			T.load_from_file();
		}
		if(T.toggle.being_pressed())
		{
			T.toggle_timer();
		}
		if(T.lap.being_pressed())
		{
			T.lap_timer();
		}
		if(T.reset_b.pressed())
		{
			T.reset_stats();
		}
		//..........................................................

		//..........................................................processing
		T.process_timer_stats();
		if(T.alarm_time())
		{
			T.play_alarm();
		}
		if(T.message_time())
		{
			T.set_message();
		}
		//..........................................................

		//..........................................................graphics
		T.display_timer_elements();
		//..........................................................

		//..........................................................termination
		T.terminate_frame();
		//..........................................................
	}
	return 0;
}
