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

#define wait___ {bool stop=false;while(!stop){if(SDL_WaitEvent(&event))if(event.type==SDL_KEYDOWN||event.type==SDL_QUIT){stop=true;if(event.type==SDL_QUIT)ended=true;}}}

class SDL
{
public:
	SDL_Surface* scr;
	HWND hwnd;
	SDL(SDL_Surface* screen)
	{
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
};
class STUDY_TIMER:public SDL
{
	bool First_run;
	vector<float> laps;
	float avg,target;
	TTF_Font* font;
public:
	Mix_Chunk *beat,*beat_z,*beep;
	SDL_Rect progress;
	graphicstring time,laptime,average,misc;
	graphicstring message;
	graphicstringinput user;
	button toggle;
	button lap;
	button target_b;
	button save_state;
	button load;
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
		message.set_font(40);
		vect pos(-50,299,0),vel(0,0,0),destination(50,300,0);
		Mix_PlayChannel(-1,welcome,0);
		message.set("Welcome! This may be your first time.\n(Press any key to continue)");
		unsigned int loop=0;
		SDL_Delay(50);
		while((vel.mag()>1)||(fabs(pos.x-destination.x)>1))
		{
			vel+=0.01*(destination-pos);
			pos+=vel;
			SDL_FillRect(scr,&scr->clip_rect,0xffff55);
			message.set_position(pos.x,pos.y);
			message.display();
			SDL_Flip(scr);
			SDL_Delay(25);
			loop++;
		}
		wait___
		Mix_FreeChunk(welcome);
		SDL_FillRect(scr,&scr->clip_rect,0x990000);
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
		toggle.graphictext->set_font(40);
		toggle.graphictext->set_color(0,0,255);
		toggle.graphictext->set("start/stop");
		toggle.set(50,200);
		lap.graphictext->set_font(40);
		lap.graphictext->set("lap");
		lap.set(50,500);
		target_b.set(900,50,200,50);
		target_b.graphictext->set("Set Goal");
		save_state.set(900,150,200,50);
		save_state.graphictext->set("save_state");
		load.set(900,250,200,50);
		load.graphictext->set("load");
		average.set_font(40);
		average.set_position(260,380);
		average.set("Average");
		average.display();

		message.set_font(60);
		message.set_position(50,650);
		time.set_font(40);
		time.set("0");
		time.set_position(30,430);
		time.set_color(0,255,0);
		time.set("Elapse");
	}
	void overlay_help()
	{
		misc.set_color(0,150,255);
		misc.set_font(28);
		misc.set_position(10,200-50);
		misc.set("(Start/Stop the current lap time)");
		misc.display();
		misc.set_font(25);
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
		misc.set_font(30);
		misc.set_position(900-820,50+10);
		misc.set("Set your goal(Time you want to take to finish a lap)->");
		misc.display();
		misc.set_font(40);
		misc.set_position(500,550);
		misc.set("Tap any key to continue");
		misc.display();
		average.set_font(34);
		average.set_position(250,430);
		average.set_color(255,0,0);
		laptime.set_color(255,255,0);
		average.set_position(250+180,380+20);
		average.set("<- Shows the average of all your laps");
		average.display();
		average.set_font(40);
		average.set_position(250,430);
		average.set((avg*laps.size()+t.elapse()/1000.f)/(laps.size()+1));
		average.display();
		toggle.display();
		lap.display();
		target_b.display();
		save_state.display();
		load.display();
		user.display();
		time.set_font(40);
		time.set_position(50,380);
		time.set("Elapse");
		time.display();
		time.set_position(0,380-50);
		time.set_font(30);
		time.set("(Time you took for current lap)");
		time.display();
		time.set_font(40);
		time.set_position(50,430);
		time.set((int)t.elapse()/1000.0);
		time.display();
		message.set_font(40);
		message.set("Welcome!...New messages will pop up here...");
		message.display();
		message.set_font(60);
	}
	STUDY_TIMER():SDL(SDL_SetVideoMode(1080,720,32,SDL_SWSURFACE)),time(scr),laptime(scr),average(scr),misc(scr),message(scr),user(scr),toggle(scr),lap(scr),target_b(scr),save_state(scr),load(scr)
	{
		avg=0;
		target=20;
		First_run=Firstrun_file_status();
		/*beat=Mix_LoadWAV("audio/Beat.wav");
		beat_z=Mix_LoadWAV("audio/Beat z.wav");
		beep=Mix_LoadWAV("audio/beep.wav");*/
		beat=beat_z=beep=NULL;
		font=TTF_OpenFont("Fonts/KeraterMedium.ttf",28);
		SDL_FillRect(scr,&scr->clip_rect,0x2468A);
		SDL_Flip(scr);
		progress=scr->clip_rect;
		load_timer_elements();
		if(first_run())
		{
			SDL_FillRect(scr,&scr->clip_rect,0x990000);
			SDL_Flip(scr);
			//overlay_help();
		}
		SDL_Delay(1000);
		SDL_Flip(scr);
	}
	~STUDY_TIMER()
	{
		SDL_FillRect(scr,&scr->clip_rect,0x999999);
		SDL_Flip(scr);
		SDL_Delay(1000);
		SDL_FreeSurface(scr);
		if(font)
			TTF_CloseFont(font);
		if(beat)
			Mix_FreeChunk(beat);
		if(beat_z)
			Mix_FreeChunk(beat_z);
		if(beep)
			Mix_FreeChunk(beep);
	}
};
