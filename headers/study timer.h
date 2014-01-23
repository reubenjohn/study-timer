/*
 * Study Timer.h
 *
 *  Created on: Dec 26, 2013
 *      Author: Reuben
 */

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <physim/headers/framer.hpp>
#include <physim/headers/vect.hpp>
#include <aria/headers/global_assets.hpp>
#include <headers/button.hpp>
#include <headers/mouse.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <SDL/SDL_ttf.h>
#include <windows.h>

class SDL
{
public:
	SDL_Surface* scr;
	SDL_Event event;
	HWND hwnd;
	bool quit;
	SDL(SDL_Surface* screen)
	{
		quit=false;
		hwnd=FindWindowA("ConsoleWindowClass",NULL);
		ShowWindow(hwnd,false);
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_EnableUNICODE(SDL_ENABLE);
		TTF_Init();
		Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
		Mix_Init(MIX_INIT_MP3);
		scr=screen;
	}
	~SDL()
	{
		Mix_Quit();
		TTF_Quit();
		SDL_Quit();
	}
	void wait()
	{
		bool stop=false;
		while(SDL_PollEvent(&event));
		SDL_Event e;
		while(!quit&&!stop)
		{
			if(SDL_WaitEvent(&e))
			{
				if(e.type==SDL_KEYDOWN)
					stop=true;
				if(e.type==SDL_QUIT)
					quit=true;
			}
		}
	}
	int poll_event()
	{
		return SDL_PollEvent(&event);
	}
	void SDL_handle_events()
	{
		if(event.type==SDL_QUIT)
		{
			quit=true;
		}
	}
	void update()
	{
		if(scr)
			SDL_Flip(scr);
	}
};
#define stop___ {bool stop=false;while(!stop){if(SDL_WaitEvent(&event))if(event.type==SDL_KEYDOWN||event.type==SDL_QUIT){stop=true;if(event.type==SDL_QUIT)ended=true;}}}

class STUDY_TIMER:public SDL
{
	bool First_run;
	vector<float> laps;
	float avg,target;
	vect scrdim;
public:
	FONT_POCKET font_pocket;
	Mix_Chunk *beat,*beat_z,*beep;
	SDL_Rect progress;
	graphicstring time,laptime,average,misc;
	graphicstring message;
	graphicstringinput user;
	button toggle,lap,target_b,save_state,load;
	timer mesaage_update;
	mouse ms;
	framer frm;
	timer t,warn;
	bool first_run()
	{
		return First_run;
	}
	bool Firstrun_file_status()
	{
		if(remove("Data/First_run")==0)
		{
			mark_as_run_before();
			return false;
		}
		else return true;
	}
	void perform_first_run()
	{
		Mix_Chunk* welcome=Mix_LoadWAV("audio/welcome.wav");
		message.set_color(0,255,0);
		message.set_position(10,300);
		message.set("Welcome! This may be your first time.\n(Press any key to continue)");
		ofstream fout("logs/allocation log.txt",ios::app);
		fout<<"set message\n";
		fout.close();
						TTF_Font* temp=TTF_OpenFont("Fonts/KeraterMedium.ttf",28);
						SDL_Color col={0,250,0};
						SDL_Rect rec={10,10,0,0};
						SDL_Surface* temp2=TTF_RenderText_Solid(temp,"Starting up...",col);
						SDL_BlitSurface(temp2,NULL,scr,&rec);
						update();
						TTF_CloseFont(temp);
						SDL_FreeSurface(temp2);
		SDL_Delay(100);
		fout.open("logs/allocation log.txt",ios::app);
		message.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",50));
		fout<<"set message font\n";
		fout.close();
		vect pos(-75,300,0),vel(0,0,0),destination(50,300,0);
		double damping=0.9;
		Mix_PlayChannel(-1,welcome,0);
		SDL_Event e;
		while((vel.mag()>0.01||(destination-pos).mag()>1)&&!quit)
		{
			vel*=damping;
			vel+=0.01*(destination-pos);
			pos+=vel;
			SDL_FillRect(scr,&scr->clip_rect,0xffff55);
			message.set_position(pos);
			message.display();
			SDL_Flip(scr);
			SDL_Delay(25);
			while(SDL_PollEvent(&e))
			{
				if(e.type==SDL_QUIT)
					quit=true;
			}
		}
		wait();
		Mix_FreeChunk(welcome);
		SDL_FillRect(scr,&scr->clip_rect,0x000099);
		update();
		try
		{
			load_timer_elements();
		}
		catch(...)
		{
			SDL_Delay(5000);
			wait();
		}
		update();
		wait();
		SDL_Flip(scr);
		SDL_Delay(500);
	}
	void mark_as_run_before()
	{
		ofstream preferences("Data/First_run");
		preferences<<"This file exists... which means this is not the first run of this program(delete me to show the welcome message)...";
		preferences.close();
		First_run=false;
	}
	void load_timer_elements()
	{
		try
		{
			if(toggle.graphictext)
			{
				toggle.graphictext->set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
				toggle.graphictext->set_color(0,0,255);
				toggle.graphictext->set("start/stop");
				toggle.set(50,200);
			}
			if(lap.graphictext)
			{
				lap.graphictext->set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
				lap.graphictext->set("lap");
				lap.set(50,500);
			}
			if(target_b.graphictext)
			{
				target_b.set(900,50,200,50);
				target_b.graphictext->set("Set Goal");
			}
			if(save_state.graphictext)
			{
			save_state.set(900,150,200,50);
			save_state.graphictext->set("save_state");
			}
			if(load.graphictext)
			{
				load.set(900,250,200,50);
				load.graphictext->set("load");
			}
			average.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
			average.set_position(260,380);
			average.set("Average");
			message.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",60));
			message.set_position(50,650);
			time.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
			time.set("0");
			time.set_position(30,430);
			time.set_color(0,255,0);
			time.set("Elapse");
		}
		catch(...)
		{
			SDL_Delay(2000);
			wait();
		}
	}

	void overlay_help()
	{
		SDL_FillRect(scr,&scr->clip_rect,0x990000);
		update();
		misc.set_color(0,150,255);
		misc.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",30));
		misc.set_position(10,200-50);
		misc.set("(Start/Stop the current lap time)");
		misc.display();
		misc.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",25));
		misc.set_color(255,255,0);
		misc.set_position(900-400,150+10);
		misc.set("Save the current timer state->");
		misc.display();
		misc.set_position(900-420,225+40);
		misc.set("Load a previous timer state->");
		misc.display();
		misc.set_position(900-400,150+40);
		misc.set("Replaces any previous saves!");
		misc.display();
		misc.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",30));
		misc.set_position(900-820,50+10);
		misc.set("Set your goal(Time you want to take to finish a lap)->");
		misc.display();
		misc.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",28));
		misc.set_position(500,550);
		misc.set("Tap any key to continue");
		misc.display();
		update();
		SDL_Delay(100);
		average.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",28));
		average.set_position(250,430);
		average.set_color(255,0,0);
		average.set_position(250+180,380+20);
		average.set("<- Shows the average of all your laps");
		average.display();
		average.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",35));
		average.set_position(250,430);
		average.set(0);
		average.display();
		update();
		SDL_Delay(100);
		time.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
		time.set_position(50,380);
		time.set("Elapse");
		time.display();
		time.set_position(0,380-50);
		time.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",30));
		time.set("(Time you took for current lap)");
		time.display();
		update();
		SDL_Delay(100);
		time.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
		time.set_position(50,430);
		time.set(0);
		time.display();
		message.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",35));
		message.set("Welcome!...New messages will pop up here...");
		message.display();
		message.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",60));
		update();
		SDL_Delay(500);
		toggle.display();
		lap.display();
		target_b.display();
		save_state.display();
		load.display();
		user.display();
		update();
		SDL_Delay(500);
	}
	STUDY_TIMER(vect screen_dimensions,const char* default_font_location,unsigned int default_font_size)
	:
		SDL(SDL_SetVideoMode(screen_dimensions.x,screen_dimensions.y,screen_dimensions.z,SDL_SWSURFACE)),
		font_pocket(FONT(default_font_location,default_font_size)),
		time(scr),
		laptime(scr),
		average(scr),
		misc(scr),
		message(scr),
		user(scr),
		toggle(scr),
		lap(scr),
		target_b(scr),
		save_state(scr),
		load(scr)
	{
		avg=0;
		target=20;
		First_run=Firstrun_file_status();
		/*beat=Mix_LoadWAV("audio/Beat.wav");
		beat_z=Mix_LoadWAV("audio/Beat z.wav");
		beep=Mix_LoadWAV("audio/beep.wav");*/
		beat=beat_z=beep=NULL;
		SDL_FillRect(scr,&scr->clip_rect,0x8800DD);
		update();
		TTF_Font* temp=TTF_OpenFont("Fonts/KeraterMedium.ttf",40);
		SDL_Color col={0,250,0};
		SDL_Rect rec={10,10,0,0};
		SDL_Surface* temp2=TTF_RenderText_Solid(temp,"Starting up...",col);
		SDL_BlitSurface(temp2,NULL,scr,&rec);
		update();
		SDL_FillRect(scr,&scr->clip_rect,0x8800DD);
		graphicstring study_timer(scr);
		font_pocket.new_font("Fonts/KeraterMedium.ttf",40);
		study_timer.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
		study_timer="Study Timer";
		study_timer.set_position(screen_dimensions*0.4);
		study_timer.display();
		SDL_Flip(scr);
		SDL_Delay(1000);
		progress=scr->clip_rect;
		load_timer_elements();
		if(first_run())
		{
			SDL_FillRect(scr,&scr->clip_rect,0x990000);
			SDL_Flip(scr);
		}
		ofstream fout("logs/allocation log.txt",ios::app);
		fout<<"STUDY_TIMER object created\n";
		fout.close();
		TTF_CloseFont(temp);
		SDL_FreeSurface(temp2);
	}
	~STUDY_TIMER()
	{
		ofstream fout("logs/allocation log.txt",ios::app);
		fout<<"Destroying STUDY_TIMER object\n";
		fout.close();
		SDL_FillRect(scr,&scr->clip_rect,0x999999);
		SDL_Flip(scr);
		SDL_Delay(1000);
		if(beat)
		{
			Mix_FreeChunk(beat);
			fout.open("logs/allocation log.txt",ios::app);
			fout<<"Freed beat\n";
			fout.close();
		}
		if(beat_z)
		{
			Mix_FreeChunk(beat_z);
			fout.open("logs/allocation log.txt",ios::app);
			fout<<"Freed beat_z\n";
			fout.close();
		}
		if(beep)
		{
			Mix_FreeChunk(beep);
			fout.open("logs/allocation log.txt",ios::app);
			fout<<"Freed beep\n";
			fout.close();
		}
		SDL_FreeSurface(scr);
		fout.open("logs/allocation log.txt",ios::app);
		fout<<"Freed beep\n";
		fout.close();
		fout.open("logs/allocation log.txt",ios::app);
		fout<<"STUDY_TIMER object destroyed\n";
		fout.close();
	}
};
