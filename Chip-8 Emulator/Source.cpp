#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include "Tokenizer.h"
#include "SDL.h"
#include "Processor.h"

using namespace std;

void callback();

int main(int argc, char* argv[])
{
	WIN32_FIND_DATA file;
	HANDLE search_handle = FindFirstFile("..\\Chip-8 games\\*", &file);
	
	if(search_handle)
	{
		do
		{
			wcout << file.cFileName << endl;
		}while(FindNextFile(search_handle, &file));
	}
	FindClose(search_handle);

	string file_location = "..\\Chip-8 games\\";
	string gameName;
	cout << "Enter a game name:  ";
	getline(cin,gameName);
	file_location += gameName;
	SDL_Init(SDL_INIT_EVERYTHING);

	Processor processor(1024,768);
	processor.LoadFile(file_location.c_str());

	

	int opcodesPerSecond = 1000;
	int currentTime = GetTickCount();
	int timeSinceUpdate = 0;
	while(true)
	{
		currentTime = GetTickCount();
		if(currentTime - timeSinceUpdate > 17)
		{
			processor.UpdateTimers();
			for(int i = 0; i < opcodesPerSecond/60; i++)
			{
				processor.Update();
			}
			timeSinceUpdate = GetTickCount();
		}
	}
	for(;;)
	{
		for(int i = 0; i < 5000; i++)
		{
			processor.Update();
		}
		if(processor.delayTimer > 0)
		{
			processor.delayTimer--;
		}
		if(processor.soundTimer > 0)
		{
			processor.soundTimer--;
		}
	}
	system("pause");

	SDL_Quit();
	return 0;
}