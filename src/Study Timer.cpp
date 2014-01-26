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
/*void init()
{
	hwnd=FindWindowA("ConsoleWindowClass",NULL);
	ShowWindow(hwnd,false);
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_EnableUNICODE(SDL_ENABLE);
	TTF_Init();
	Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
	Mix_Init(MIX_INIT_MP3);
	beat=Mix_LoadWAV("audio/Beat.wav");
	beat_z=Mix_LoadWAV("audio/Beat z.wav");
	beep=Mix_LoadWAV("audio/beep.wav");
	font=TTF_OpenFont("Fonts/lazy.ttf",28);
	scr=SDL_SetVideoMode(1080,720,32,SDL_SWSURFACE);
}
void quit()
{
	SDL_FreeSurface(scr);
	TTF_CloseFont(font);
	Mix_FreeChunk(beat);
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
	ShowWindow(hwnd,true);
}
void introduce()
{
	SDL_FillRect(scr,&scr->clip_rect,0xffff55);
	SDL_Flip(scr);
	welcome=Mix_LoadWAV("audio/welcome.wav");
	Mix_PlayChannel(-1,welcome,0);
	wait___
}
void handle_events(SDL_Event event);*/
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
	/*init();
	SDL_Rect progress=scr->clip_rect;
	graphicstring message;
	timer mesaage_update;
	bool first_run=true;
	if(remove("Data/first_run")==0)
		first_run=false;
	ofstream preferences("Data/first_run");
	preferences<<"This file exists... which means this is not the first run of this program(delete me to show the welcome message)...";
	preferences.close();
	if(first_run==true)
	{
		introduce();
		message.set(0,255,0);
		message.set(10,300);
		message.set_font(40);
		message.set("Welcome! This may be your first time.\n(Press any key to continue)");
		message.display();
	}
	SDL_FillRect(scr,&scr->clip_rect,0x990000);
	SDL_Flip(scr);
	SDL_Delay(500);
	vector<float> laps;
	float avg=0,target=60;
	graphicstring time,laptime,average,misc;
	graphicstringinput user;
	button toggle,lap,target_b,save_state,load;
	reload:
	toggle.graphictext.set_font(40);
	toggle.graphictext.set(0,0,255);
	toggle.graphictext.set("start/stop");
	toggle.set(50,200);
	lap.graphictext.set_font(40);
	lap.graphictext.set("lap");
	lap.set(50,500);
	target_b.set(900,50,200,50);
	target_b.graphictext.set("Set Goal");
	save_state.set(900,150,200,50);
	save_state.graphictext.set("save_state");
	load.set(900,250,200,50);
	load.graphictext.set("load");
	average.set_font(40);
	average.set(260,380);
	average.set("Average");
	average.display();

	message.set_font(60);
	message.set(50,650);
	time.set_font(40);
	time.set("0");
	time.set(30,430);
	time.set(0,255,0);
	time.set("Elapse");
	if(first_run==true)
	{
		first_run=false;
		misc.set(0,150,255);
		misc.set_font(28);
		misc.set(10,200-50);
		misc.set("(Start/Stop the current lap time)");
		misc.display();
		misc.set_font(25);
		misc.set(255,255,0);
		misc.set(900-400,150+10);
		misc.set("Save the current timer state->");
		misc.display();
		misc.set(900-420,225+40);
		misc.set("Load a previous timer state->");
		misc.display();
		misc.set(900-400,150+40);
		misc.set("Replaces any previous saves!");
		misc.display();
		misc.set_font(30);
		misc.set(900-820,50+10);
		misc.set("Set your goal(Time you want to take to finish a lap)->");
		misc.display();
		misc.set_font(40);
		misc.set(500,550);
		misc.set("Tap any key to continue");
		misc.display();
		average.set_font(34);
		average.set(250,430);
		average.set(255,0,0);
		laptime.set(255,255,0);
		average.set(250+180,380+20);
		average.set("<- Shows the average of all your laps");
		average.display();
		average.set_font(40);
		average.set(250,430);
		average.set((avg*laps.size()+t.elapse()/1000.f)/(laps.size()+1));
		average.display();
		toggle.display();
		lap.display();
		target_b.display();
		save_state.display();
		load.display();
		user.display();
		time.set_font(40);
		time.set(50,380);
		time.set("Elapse");
		time.display();
		time.set(0,380-50);
		time.set_font(30);
		time.set("(Time you took for current lap)");
		time.display();
		time.set_font(40);
		time.set(50,430);
		time.set((int)t.elapse()/1000.0);
		time.display();
		message.set_font(40);
		message.set("Welcome!...New messages will pop up here...");
		message.display();
		message.set_font(60);
		SDL_Flip(scr);
		wait___
		goto reload;
	}
	*/
	/*
	message.set("Welcome!");
	misc.set_font(30);
	misc.set(300,600);
	user.done=true;
	warn.start();
	mesaage_update.start();
	while(!ended)
	{
		toggle.refresh();
		lap.refresh();
		while(SDL_PollEvent(&event))
		{
			handle_events(event);
			toggle.handle_events(event);
			lap.handle_events(event);
			target_b.handle_events(event);
			save_state.handle_events(event);
			load.handle_events(event);
			if(!user.done)
			{
				user.handle_input(event);
			}
			else
			{
				if(need_target)
				{
					string a;
					user.get(a);
					target=atoi(a.c_str());
					warning=(target*1000+t.elapse())/2;
					need_target=false;
				}
			}
		}
		if(target_b.state==-2)
		{
			need_target=true;
			user.finished(false);
		}
		if(save_state.state==-3)
		{
			ofstream fout("Data/save.txt");
			fout<<target<<' '<<t.elapse()<<' ';
			for(unsigned int i=0;i<laps.size();i++)
				fout<<laps[i]<<' ';
			fout.close();
		}
		if(load.state==-3)
		{
			laps.clear();
			ifstream fin("Data/save.txt");
			float temp=0;
			fin>>target>>temp;
			t.set(temp);
			while(fin>>temp)
			{
				laps.push_back(temp);
				avg+=temp;
			}
			if(laps.size()>0)
				avg/=laps.size();
		}
		if(toggle.state==-2)
		{
			Mix_PlayChannel(-1,beat_z,0);
			t.toggle();
		}
		if(lap.state==-2||user_lapped)
		{
			Mix_PlayChannel(-1,beat,0);
			user_lapped=false;
			avg*=laps.size();
			laps.push_back(t.elapse()/1000.f);
			avg/=laps.size();
			avg+=laps[laps.size()-1]/laps.size();
			t.reset();
			t.start();
			warning=target*1000/2;
		}
		*/
/*
		if(mesaage_update.elapse()>1000&&t.state()==1)
		{
			if(avg>target)
			{
				message.set(255,0,0);
				message.set("Hurry up!");
			}
			else
			{
				message.set(0,255,0);
				message.set("You are doing good!");
			}
			mesaage_update.reset();
			mesaage_update.start();
		}
*/
	/*
		SDL_FillRect(scr,&scr->clip_rect,0x009999);
		float temp=((double)t.elapse()/(target*1000.0));
		progress.h=scr->clip_rect.h*temp;
		progress.y=scr->clip_rect.h-progress.h;
		SDL_FillRect(scr,&progress,0xFFCC00);
		time.set(50,380);
		time.set("Elapse");
		time.display();
		time.set(30,430);
		time.set((int)t.elapse()/1000.0);
		time.display();
		if(laps.size()>0)
		{
			laptime.set("Laps:");
			laptime.set(lap_pos.x,lap_pos.y);
			laptime.display();
		}
		for(unsigned int i=0;i<laps.size();i++)
		{
			laptime.set((int)i+1);
			laptime.set(lap_pos.x-75,lap_pos.y+(laps.size()-i)*global_font_size);
			laptime.display();
			laptime.set(laps[i]);
			laptime.set(lap_pos.x,lap_pos.y+(laps.size()-i)*global_font_size);
			laptime.display();
		}
		if(t.elapse()>warning&&warn.elapse()>500&&t.state()==1)
		{
			Mix_PlayChannel(-1,beep,0);
			warning=(target*1000+t.elapse())/2;
			warn.reset();
			warn.start();
		}
		misc.set(warning/1000.f);
		misc.display();
		average.set(320,380);
		average.set("Average");
		average.display();
		average.set(310,430);
		average.set((avg*laps.size()+t.elapse()/1000.f)/(laps.size()+1));
		average.display();
		message.display();
		toggle.display();
		lap.display();
		target_b.display();
		save_state.display();
		load.display();
		user.display();
		SDL_Flip(scr);
		frm.endframe();
		frm.smartwait();
	}
	quit();*/
	return 0;
}
/*
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
		case SDLK_SPACE:
			user_lapped=true;
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
*/
