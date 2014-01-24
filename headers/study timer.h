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
	void pause(unsigned int pause_time)
	{
		timer T;
		T.start();
		while(SDL_PollEvent(&event));
		SDL_Event e;
		while(!quit&&T.elapse()<pause_time)
		{
			if(SDL_WaitEvent(&e))
			{
				if(e.type==SDL_QUIT)
					quit=true;
			}
		}
	}
	int poll_event()
	{
		return SDL_PollEvent(&event);
	}
	int wait_event()
	{
		return SDL_WaitEvent(&event);
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
	bool First_run,need_target;
	unsigned int next_alarm,laps_font_size,scroll_position;
	vector<float> laps;
	float avg,target,lap_progress;
	vect scrdim;
	framer frm;
	timer t,warn;
	Mix_Chunk *beat,*beat_z,*beep;
	SDL_Rect progress;
	GRAPHIC_STRING elapse_caption,elapse,laptime,average_caption,average,misc,message;
	vect study_timer_pos,elapse_caption_pos,elapse_pos,laptime_pos,average_caption_pos,average_pos,message_pos;
	vect lap_pos,lap_dim,toggle_pos,target_b_pos,target_b_dim,save_state_pos,save_state_dim,load_pos,load_dim;
	GRAPHIC_STRING graphic_laps_caption;
	vector<GRAPHIC_STRING*> graphic_laps;
	vect graphic_laps_caption_pos,graphic_laps_pos;
	GRAPHIC_STRING_INPUT user;
	timer mesaage_update;
	mouse ms;
public:
	button toggle,lap,target_b,save_state,load;
	FONT_POCKET font_pocket;
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
		ofstream fout("logs/allocation log.txt",ios::app);
		fout<<"set message\n";
		fout.close();
						/*TTF_Font* temp=TTF_OpenFont("Fonts/KeraterMedium.ttf",25);
						SDL_Color col={0,250,0};
						SDL_Rect rec={10,10,0,0};
						SDL_Surface* temp2=TTF_RenderText_Solid(temp,"Starting up...",col);
						SDL_BlitSurface(temp2,NULL,scr,&rec);
						update();
						SDL_FreeSurface(temp2);*/
		//SDL_Delay(1000);
		fout.open("logs/allocation log.txt",ios::app);
		ofstream fout2("logs/log.txt",ios::app);
		GRAPHIC_STRING any_key(scr,NULL,5000);
		fout2<<"scr="<<scr<<'\n';
		any_key.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",50));
		any_key="(Tap any key to continue)";
		SDL_Delay(1000);
		message="Welcome! This may be your first time.";
		message.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",50));
		message.set_position(200,100);
		message.display();
		message.set_update_interval(1000);
		SDL_Delay(25);
		fout<<"set message font\n";
		fout.close();
		update();
		vect pos(-75,300,0),vel(0,0,0),destination(50,300,0);
		double damping=0.9;
		Mix_PlayChannel(-1,welcome,0);
		SDL_Event e;
		bool stop=false;
		while((vel.mag()>0.01||(destination-pos).mag()>1)&&!quit&&!stop)
		{
			vel*=damping;
			vel+=0.01*(destination-pos);
			pos+=vel;
			SDL_FillRect(scr,&scr->clip_rect,0xffff55);
			any_key.set_position(pos.x,pos.y+60);
			any_key.display();
			message.set_position(pos);
			message.display();
			update();
			SDL_Delay(25);
			while(SDL_PollEvent(&e))
			{
				if(e.type==SDL_QUIT)
					quit=true;
				else
					if(e.type==SDL_KEYDOWN)
					{
						stop=true;
					}
			}
		}
		if(!stop)
			wait();
		Mix_FreeChunk(welcome);
		update();
		try
		{
			load_timer_elements();
			overlay_help();
			update();
			wait();
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
	void load_timer_element_positions(vect screen_dimensions)
	{
		study_timer_pos=screen_dimensions*0.4;
		toggle_pos=(vect){50,200,0};
		target_b_pos=(vect){900,50,0};
		target_b_dim=(vect){200,50,0};
		save_state_pos=(vect){900,150,0};
		save_state_dim=(vect){200,50,0};
		save_state_pos=(vect){900,150,0};
		save_state_dim=(vect){200,50,0};
		load_pos=(vect){900,250,0};
		load_dim=(vect){200,50,0};
		elapse_caption_pos=(vect){50,380,0};
		elapse_pos=(vect){50,430,0};
		average_caption_pos=(vect){250,380,0};
		average_pos=(vect){250,430,0};
		lap_pos=(vect){50,500,0};
		laptime_pos=(vect){30,430,0};
		average_caption_pos=(vect){260,380,0};
		message_pos=(vect){50,650,0};
		graphic_laps_caption_pos=(vect){750,20,0};
		graphic_laps_pos=(vect){750,50,0};
	}
	void load_timer_elements()
	{
		try
		{
			average_caption.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
			average_caption.set_position(average_pos);
			average_caption="Average";
			message.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",50));
			message.set_position(message_pos);

			elapse_caption.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
			elapse_caption.set_color(0,255,0);
			elapse_caption.set_position(elapse_caption_pos);
			elapse_caption="Elapse";
			elapse.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",35));
			elapse.set_color(0,255,0);
			elapse.set_position(elapse_pos);
			elapse=0;

			graphic_laps_caption.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",laps_font_size));
			graphic_laps_caption.set_color(255,0,0);
			graphic_laps_caption.set_position(graphic_laps_caption_pos);
			graphic_laps_caption="Laps:";

			average_caption.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
			average_caption.set_position(average_caption_pos);
			average_caption.set_color(255,0,0);
			average_caption="Average";
			average.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",35));
			average.set_color(255,0,0);
			average.set_position(average_pos);
			average=0;

			toggle.graphic_text->set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
			toggle.graphic_text->set_color(0,0,255);
			*toggle.graphic_text="Start / Stop";
			toggle.set_dimensions(toggle_pos);

			lap.graphic_text->set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
			lap.graphic_text->set_color(255,0,0);
			*lap.graphic_text="Lap";
			lap.set_dimensions(lap_pos);

			target_b.graphic_text->set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",30));
			target_b.set_dimensions(target_b_pos,target_b_dim);
			*target_b.graphic_text="Set Goal";

			save_state.graphic_text->set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",30));
			save_state.set_dimensions(save_state_pos,save_state_dim);
			*save_state.graphic_text="Save";

			load.graphic_text->set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",30));
			load.set_dimensions(load_pos,load_dim);
			*load.graphic_text="Load";
		}
		catch(...)
		{
			SDL_Delay(2000);
			wait();
		}
	}
	void display_timer_elements()
	{
		average_caption.display(1);
		average.display(1);
		elapse_caption.display(1);
		elapse.display(1);
		graphic_laps_caption.display();
		for(unsigned int i=0;i<graphic_laps.size();i++)
		{
			if(graphic_laps[i])
			{
				if(graphic_laps[i]->rectangle().y>0)
					graphic_laps[i]->display();
			}
		}
		message.display(1);
		toggle.display();
		lap.display();
		target_b.display();
		save_state.display();
		load.display();
		user.display();
	}
	void overlay_help()
	{
		SDL_FillRect(scr,&scr->clip_rect,0x0066EE);
		misc.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",30));
		misc.set_color(70,220,255);
		misc.set_position(10,150);
		misc="(Start/Stop the current lap time)";
		misc.display(1);
		misc.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",25));
		misc.set_color(255,255,0);
		misc.set_position(900-380,150+10);
		misc="Save the current timer state->";
		misc.display(1);
		misc.set_position(900-420,225+40);
		misc="Load a previous timer state->";
		misc.display();
		misc.set_position(900-400,150+40);
		misc="Replaces any previous saves!";
		misc.display(1);
		misc.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",30));
		misc.set_position(70,60);
		misc="Set your goal (Time you want to take to finish a lap)->";
		misc.display(1);
		misc.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",28));
		misc.set_position(500,550);
		misc="Tap any key to continue";
		misc.display(1);
		misc.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",28));
		misc.set_color(255,0,0);
		misc.set_position(450,380+20);
		misc="<- Shows the average of all your laps";
		misc.display(1);
		misc.set_position(0,380-50);
		misc.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",30));
		misc.set_color(0,255,0);
		misc="(Time you took for current lap)";
		misc.display(1);

		elapse_caption.display(1);
		elapse.display(1);

		average_caption.display(1);
		average.display(1);

		message="Welcome! New messages will pop up here";
		message.display(1);
		toggle.display();
		lap.display();
		target_b.display();
		save_state.display();
		load.display();
		user.display();

		wait();
	}
	void updatefpslimits(double min_fps,double max_fps)
	{
		frm.updatefpslimits(min_fps,max_fps);
	}
	void initiate_frame()
	{
		toggle.refresh();
		lap.refresh();
	}
	void terminate_frame()
	{
		update();
		SDL_FillRect(scr,&scr->clip_rect,0x009999);
		SDL_FillRect(scr,&progress,0xFFCC00);
		frm.smartwait();
	}
	void handle_mouse_motion()
	{
		ms.handle_events(event);
	}
	void refresh_lap_positions()
	{
		for(unsigned int i=0;i<graphic_laps.size();i++)
		{
			graphic_laps[i]->set_position(graphic_laps_pos.x,graphic_laps_pos.y+(graphic_laps.size()-i+scroll_position)*laps_font_size);
		}
	}
	void handle_scrolling()
	{
		switch(event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			switch(event.button.button)
			{
			case SDL_BUTTON_WHEELUP:
				scroll_position++;
				refresh_lap_positions();
			break;
			case SDL_BUTTON_WHEELDOWN:
				scroll_position--;
				//if(scroll_position>0)
				refresh_lap_positions();
			break;
			}
		break;
		}
	}
	void handle_button_events()
	{
		toggle.handle_events(event);
		lap.handle_events(event);
		target_b.handle_events(event);
		save_state.handle_events(event);
		load.handle_events(event);
	}
	void handle_timer_key_events()
	{
		switch(event.type)
		{
		case SDL_KEYDOWN:
			switch((unsigned int)event.key.keysym.sym)
			{
			case SDLK_RETURN:
				toggle_timer();
			break;
			case SDLK_SPACE:
				lap_timer();
			break;
			}
		break;
		}
	}
	void handle_all_events()
	{
		SDL_handle_events();
		handle_mouse_motion();
		handle_scrolling();
		handle_timer_key_events();
		handle_button_events();
	}
	bool timer_running()
	{
		return t.running();
	}
	void toggle_timer()
	{
		t.toggle();
		if(beat_z)
			Mix_PlayChannel(-1,beat_z,0);
	}
	void lap_timer()
	{
		if(beat)
			Mix_PlayChannel(-1,beat,0);
		avg*=laps.size();
		laps.push_back(t.elapse()/1000.f);
		graphic_laps.push_back(new GRAPHIC_STRING(scr));
		if(graphic_laps[graphic_laps.size()-1])
		{
			GRAPHIC_STRING* temp=graphic_laps[graphic_laps.size()-1];
			temp->set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",laps_font_size));
			temp->set_color(255,255,255);
			string contents;
			char U[10];
			sprintf(U,"%-4i",laps.size());
			contents.assign(U);
			sprintf(U,"%8.3f",(t.elapse()/1000.f));
			contents.append(") ");
			contents.append(U);
			*graphic_laps[graphic_laps.size()-1]=contents;
		}
		refresh_lap_positions();
		avg/=laps.size();
		avg+=laps[laps.size()-1]/laps.size();
		t.reset();
		t.start();
		next_alarm=target*1000/2;
	}
	void reset_stats()
	{
		avg=0;
		laps.clear();
		t.reset();
		target=60;
	}
	void process_timer_stats()
	{
		elapse.set((int)t.elapse()/1000.0,"%8.3f");
		if(target!=0)
			lap_progress=((double)t.elapse()/(target*1000.0));
		else
			lap_progress=0;
		progress.h=scr->clip_rect.h*lap_progress;
		progress.y=scr->clip_rect.h-progress.h;
		average.set((avg*laps.size()+t.elapse()/1000.f)/(laps.size()+1),"%8.3f");
	}
	void save_to_file()
	{
		ofstream fout("Data/save.txt");
		fout<<target<<' '<<t.elapse()<<' ';
		for(unsigned int i=0;i<laps.size();i++)
			fout<<laps[i]<<' ';
		fout.close();
	}
	void load_from_file()
	{
		avg=0;
		laps.clear();
		for(unsigned int i=0;i<graphic_laps.size();i++)
		{
			if(graphic_laps[i])
			{
				delete graphic_laps[i];
				graphic_laps[i]=NULL;
			}
		}
		graphic_laps.clear();
		ifstream fin("Data/save.txt");
		float temp=0;
		fin>>target>>temp;
		t.set(temp);
		while(fin>>temp)
		{
			laps.push_back(temp);
			avg+=temp;
			graphic_laps.push_back(new GRAPHIC_STRING(scr));
			GRAPHIC_STRING* p=graphic_laps[graphic_laps.size()-1];
			if(p)
			{
				p->set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",laps_font_size));
				p->set_color(255,255,255);
				string contents;
				char U[10];
				sprintf(U,"%-4i",laps.size());
				contents.assign(U);
				sprintf(U,"%8.3f",temp);
				contents.append(") ");
				contents.append(U);
				*p=contents;
			}
		}
		scroll_position=0;
		refresh_lap_positions();
		if(laps.size()>0)
			avg/=laps.size();
	}
	void set_need_target(bool Need_Target)
	{
		need_target=Need_Target;
		user.finished(false);
	}
	STUDY_TIMER(vect screen_dimensions,const char* default_font_location,unsigned int default_font_size)
	:
		SDL(SDL_SetVideoMode(screen_dimensions.x,screen_dimensions.y,screen_dimensions.z,SDL_SWSURFACE)),
		elapse_caption(scr),
		elapse(scr),
		laptime(scr),
		average_caption(scr),
		average(scr),
		misc(scr),
		message(scr),
		graphic_laps_caption(scr),
		user(scr),
		toggle(scr),
		lap(scr),
		target_b(scr),
		save_state(scr),
		load(scr),
		font_pocket(FONT(default_font_location,default_font_size))
	{
		avg=0;
		target=60;
		laps_font_size=20;
		scroll_position=0;
		next_alarm=target/2;
		scrdim=screen_dimensions;
		First_run=Firstrun_file_status();
		need_target=false;
		beat=Mix_LoadWAV("audio/Beat.wav");
		beat_z=Mix_LoadWAV("audio/Beat z.wav");
		beep=Mix_LoadWAV("audio/beep.wav");
		SDL_FillRect(scr,&scr->clip_rect,0x8800DD);
		update();

		load_timer_element_positions(screen_dimensions);

		TTF_Font* temp=TTF_OpenFont("Fonts/KeraterMedium.ttf",40);
		SDL_Color col={0,250,0};
		SDL_Rect rec={10,10,0,0};
		SDL_Surface* temp2=TTF_RenderText_Solid(temp,"Starting up...",col);
		SDL_BlitSurface(temp2,NULL,scr,&rec);
		update();
		TTF_CloseFont(temp);
		SDL_FreeSurface(temp2);
		SDL_FillRect(scr,&scr->clip_rect,0x8800DD);
		GRAPHIC_STRING study_timer(scr);
		font_pocket.new_font("Fonts/KeraterMedium.ttf",40);
		study_timer.set_color(255,0,0);
		study_timer="Study Timer";
		study_timer.set_font(font_pocket.new_font("Fonts/KeraterMedium.ttf",40));
		study_timer.set_position(study_timer_pos);
		study_timer.display();
		SDL_Flip(scr);
		pause(1000);
		progress=scr->clip_rect;
		load_timer_elements();
		ofstream fout("logs/allocation log.txt",ios::app);
		fout<<"STUDY_TIMER object created\n";
		fout.close();
	}
	~STUDY_TIMER()
	{
		ofstream fout("logs/allocation log.txt",ios::app);
		fout<<"Destroying STUDY_TIMER object\n";
		fout.close();
		SDL_FillRect(scr,&scr->clip_rect,0x999999);
		SDL_Flip(scr);
		SDL_Delay(100);
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
