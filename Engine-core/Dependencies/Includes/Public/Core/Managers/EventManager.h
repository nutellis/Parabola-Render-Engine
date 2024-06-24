#pragma once
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H


enum EventType
{
	// Mouse button events. Each of the next six events gets passed
	// the absolute position of the mouse
	E_MOUSELEFTBUTTONPRESS,
	E_MOUSELEFTBUTTONRELEASE,

	// Start new games
	E_NEWGAMEEASY,
	E_NEWGAMENORMAL,
	E_NEWGAMEHARD,

	// Game play related
	E_INCREMENTSCORE,
	E_PAUSEGAME,
	E_CONTINUEGAME,
	E_GAMEOVER,

	// Close the App
	E_APPCLOSE
};


struct Event 
{
	EventType Type;
	int arg1, arg2;
};






#endif // EVENTMANAGER_H