#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <stack>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>
#include "SDL.h"
#include "SDL_draw.h"
#include "Processor.h"
#include "Tokenizer.h"

using namespace std;

Processor::Processor(int screenWidth, int screenHeight)
{
	m_Register = new unsigned char[16];
	m_regI = 0;
	for(int i = 0; i < 16; i++)
	{
		m_Register[i] = 0;
		m_Input[i] = 0;
	}

	m_ProgramCounter = 0x200;
	m_ScreenData.resize(screenWidth);
	for(int i = 0; i < screenWidth; i++)
	{
		m_ScreenData[i].resize(screenHeight);
	}
	Processor::screenWidth = screenWidth;
	Processor::screenHeight = screenHeight;
	DrawInit();
	

	// "0"
	m_GameMemory[0] = 0xF0;
	m_GameMemory[1] = 0x90;
	m_GameMemory[2] = 0x90;
	m_GameMemory[3] = 0x90;
	m_GameMemory[4] = 0xF0;

	// "1"
	m_GameMemory[5] = 0x20;
	m_GameMemory[6] = 0x60;
	m_GameMemory[7] = 0x20;
	m_GameMemory[8] = 0x20;
	m_GameMemory[9] = 0x70;

	// "2"
	m_GameMemory[10] = 0xF0;
	m_GameMemory[11] = 0x10;
	m_GameMemory[12] = 0xF0;
	m_GameMemory[13] = 0x80;
	m_GameMemory[14] = 0xF0;

	// "3"
	m_GameMemory[15] = 0xF0;
	m_GameMemory[16] = 0x10;
	m_GameMemory[17] = 0xF0;
	m_GameMemory[18] = 0x10;
	m_GameMemory[19] = 0xF0;

	// "4"
	m_GameMemory[20] = 0x90;
	m_GameMemory[21] = 0x90;
	m_GameMemory[22] = 0xF0;
	m_GameMemory[23] = 0x10;
	m_GameMemory[24] = 0x10;

	// "5"
	m_GameMemory[25] = 0xF0;
	m_GameMemory[26] = 0x80;
	m_GameMemory[27] = 0xF0;
	m_GameMemory[28] = 0x10;
	m_GameMemory[29] = 0xF0;

	// "6"
	m_GameMemory[30] = 0xF0;
	m_GameMemory[31] = 0x80;
	m_GameMemory[32] = 0xF0;
	m_GameMemory[33] = 0x90;
	m_GameMemory[34] = 0xF0;

	// "7"
	m_GameMemory[35] = 0xF0;
	m_GameMemory[36] = 0x10;
	m_GameMemory[37] = 0x20;
	m_GameMemory[38] = 0x40;
	m_GameMemory[39] = 0x40;

	// "8"
	m_GameMemory[40] = 0xF0;
	m_GameMemory[41] = 0x90;
	m_GameMemory[42] = 0xF0;
	m_GameMemory[43] = 0x90;
	m_GameMemory[44] = 0xF0;

	// "9"
	m_GameMemory[45] = 0xF0;
	m_GameMemory[46] = 0x90;
	m_GameMemory[47] = 0xF0;
	m_GameMemory[48] = 0x10;
	m_GameMemory[49] = 0xF0;

	// "A"
	m_GameMemory[50] = 0xF0;
	m_GameMemory[51] = 0x90;
	m_GameMemory[52] = 0xF0;
	m_GameMemory[53] = 0x90;
	m_GameMemory[54] = 0x90;

	// "B"
	m_GameMemory[55] = 0xE0;
	m_GameMemory[56] = 0x90;
	m_GameMemory[57] = 0xE0;
	m_GameMemory[58] = 0x90;
	m_GameMemory[59] = 0xE0;

	// "C"
	m_GameMemory[60] = 0xF0;
	m_GameMemory[61] = 0x80;
	m_GameMemory[62] = 0x80;
	m_GameMemory[63] = 0x80;
	m_GameMemory[64] = 0xF0;

	// "D"
	m_GameMemory[65] = 0xE0;
	m_GameMemory[66] = 0x90;
	m_GameMemory[67] = 0x90;
	m_GameMemory[68] = 0x90;
	m_GameMemory[69] = 0xE0;

	// "E"
	m_GameMemory[70] = 0xF0;
	m_GameMemory[71] = 0x80;
	m_GameMemory[72] = 0xF0;
	m_GameMemory[73] = 0x80;
	m_GameMemory[74] = 0xF0;

	// "F"
	m_GameMemory[75] = 0xF0;
	m_GameMemory[76] = 0x80;
	m_GameMemory[77] = 0xF0;
	m_GameMemory[78] = 0x80;
	m_GameMemory[79] = 0x80;



}
Processor::~Processor()
{
	delete[] m_Register;
}

void Processor::LoadFile(const char* file_path)
{
	ifstream file(file_path, ifstream::binary);
	if(file)
	{
		file.seekg(0,ios::end);
		int file_size = file.tellg();
		file.seekg(0,ios::beg);
	
		if(file_size > 0)
		{
			file.read((char*)(m_GameMemory+0x200),file_size);

			if(!file)
			{
				cout << "only " << file.gcount() << " could be read" << endl;;
			}
			file.close();
		}
	}
}

WORD Processor::GetNextInstruction()
{
	WORD res = 0;
	res = m_GameMemory[m_ProgramCounter];
	res = res << 8;
	res |= m_GameMemory[m_ProgramCounter+1];
	m_ProgramCounter += 2;
	return res;
}

void Processor::DrawInit()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cerr << "Unable to initialize SDL: " << SDL_GetError();
		exit(1);
	}
	atexit(SDL_Quit);

	screen = SDL_SetVideoMode(screenWidth,screenHeight,8, SDL_SWSURFACE | SDL_ANYFORMAT);


}

void Processor::Draw()
{
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
	for(int i = 0; i < 64; i++)
	{
		for(int j = 0; j < 32; j++)
		{
			if(m_ScreenData[i][j] == 1)
			{
				SDL_Rect rect;
				rect.x = float((float)i / 64 * screenWidth);
				rect.y = float((float)j / 32  * screenHeight);
				rect.w = 20;
				rect.h = 20;
				SDL_FillRect(screen,&rect, SDL_MapRGB(screen->format,255,255,255));
			}
		}
	}
	SDL_Flip(screen);
}

void Processor::Input()
{
	SDL_Event keyevent;
	while(SDL_PollEvent(&keyevent))
	{
		if(keyevent.key.type == SDL_KEYDOWN)
		{
			switch(keyevent.key.keysym.sym)
			{
			case SDLK_1:
				m_Input[0x1] = 1;
				break;
			case SDLK_2:
				m_Input[0x2] = 1;
				break;
			case SDLK_3:
				m_Input[0x3] = 1;
				break;
			case SDLK_4:
				m_Input[0xC] = 1;
				break;
			case SDLK_q:
				m_Input[0x4] = 1;
				break;
			case SDLK_w:
				m_Input[0x5] = 1;
				break;
			case SDLK_e:
				m_Input[0x6] = 1;
				break;
			case SDLK_r:
				m_Input[0xD] = 1;
				break;
			case SDLK_a:
				m_Input[0x7] = 1;
				break;
			case SDLK_s:
				m_Input[0x8] = 1;
				break;
			case SDLK_d:
				m_Input[0x9] = 1;
				break;
			case SDLK_f:
				m_Input[0xE] = 1;
				break;
			case SDLK_z:
				m_Input[0xA] = 1;
				break;
			case SDLK_x:
				m_Input[0x0] = 1;
				break;
			case SDLK_c:
				m_Input[0xB] = 1;
				break;
			case SDLK_v:
				m_Input[0xF] = 1;
				break;
			case SDLK_ESCAPE:
				exit(1);
				break;
			default:
				break;
			}
		}
		if(keyevent.key.type == SDL_KEYUP)
		{
			switch(keyevent.key.keysym.sym)
			{
			case SDLK_1:
				m_Input[0x1] = 0;
				break;
			case SDLK_2:
				m_Input[0x2] = 0;
				break;
			case SDLK_3:
				m_Input[0x3] = 0;
				break;
			case SDLK_4:
				m_Input[0xC] = 0;
				break;
			case SDLK_q:
				m_Input[0x4] = 0;
				break;
			case SDLK_w:
				m_Input[0x5] = 0;
				break;
			case SDLK_e:
				m_Input[0x6] = 0;
				break;
			case SDLK_r:
				m_Input[0xD] = 0;
				break;
			case SDLK_a:
				m_Input[0x7] = 0;
				break;
			case SDLK_s:
				m_Input[0x8] = 0;
				break;
			case SDLK_d:
				m_Input[0x9] = 0;
				break;
			case SDLK_f:
				m_Input[0xE] = 0;
				break;
			case SDLK_z:
				m_Input[0xA] = 0;
				break;
			case SDLK_x:
				m_Input[0x0] = 0;
				break;
			case SDLK_c:
				m_Input[0xB] = 0;
				break;
			case SDLK_v:
				m_Input[0xF] = 0;
				break;
			default:
				break;
			}
		}
	}
}

void Processor::PlayAudio()
{
	if(soundTimer > 0)
	{
		//Beep(523,200);
	}
}

void Processor::Update()
{
	Input();
	//PlayAudio();
	WORD opcode = GetNextInstruction();

	switch(opcode & 0xF000)
	{
	case 0x0000:
		switch(opcode & 0x000F)
		{
		case 0x0000: Opcode00E0(opcode); 
			break;
		case 0x000E: Opcode00EE(opcode); 
			break;
		}
		break;

	case 0x1000:
		Opcode1NNN(opcode);
		break;
	case 0x2000:
		Opcode2NNN(opcode);
		break;
	case 0x3000:
		Opcode3XNN(opcode);
		break;
	case 0x4000:
		Opcode4XNN(opcode);
		break;
	case 0x5000:
		Opcode5XY0(opcode);
		break;
	case 0x6000:
		Opcode6XNN(opcode);
		break;
	case 0x7000:
		Opcode7XNN(opcode);
		break;
	case 0x8000:
		switch(opcode & 0x000F)
		{
		case 0x0000:
			Opcode8XY0(opcode);
			break;
		case 0x0001:
			Opcode8XY1(opcode);
			break;
		case 0x0002:
			Opcode8XY2(opcode);
			break;
		case 0x0003:
			Opcode8XY3(opcode);
			break;
		case 0x0004:
			Opcode8XY4(opcode);
			break;
		case 0x0005:
			Opcode8XY5(opcode);
			break;
		case 0x0006:
			Opcode8XY6(opcode);
			break;
		case 0x0007:
			Opcode8XY7(opcode);
		}
		break;
	case 0x9000:
		Opcode9XY0(opcode);
		break;
	case 0xA000:
		OpcodeANNN(opcode);
		break;
	case 0xB000:
		OpcodeBNNN(opcode);
		break;
	case 0xC000:
		OpcodeCXNN(opcode);
		break;
	case 0xD000:
		OpcodeDXYN(opcode);
		break;
	case 0xE000:
		switch(opcode & 0x000F)
		{
		case 0x000E:
			OpcodeEX9E(opcode);
			break;
		case 0x0001:
			OpcodeEXA1(opcode);
			break;
		}
		break;
	case 0xF000:
		switch(opcode & 0x000F)
		{
		case 0x0007:
			OpcodeFX07(opcode);
			break;
		case 0x000A:
			OpcodeFX0A(opcode);
			break;
		case 0x0008:
			OpcodeFX18(opcode);
			break;
		case 0x000E:
			OpcodeFX1E(opcode);
			break;
		case 0x0009:
			OpcodeFX29(opcode);
			break;
		case 0x0003:
			OpcodeFX33(opcode);
			break;
		case 0x0005:
			switch(opcode & 0x00F0)
			{
			case 0x0010:
				OpcodeFX15(opcode);
				break;
			case 0x0050:
				OpcodeFX55(opcode);
				break;
			case 0x0060:
				OpcodeFX65(opcode);
				break;
			}
			break;
		}
		break;
	}
}

void Processor::UpdateTimers()
{
	if(delayTimer > 0)
		delayTimer--;
	if(soundTimer > 0)
		soundTimer--;
}

void Processor::Opcode00E0(WORD opcode)
{
	for(int i = 0; i < screenWidth; i++)
	{
		for(int j = 0; j < screenHeight; j++)
		{
			m_ScreenData[i][j] = 0;
		}
	}
}
void Processor::Opcode00EE(WORD opcode)
{
	m_ProgramCounter = m_Stack.back();
	m_Stack.pop_back();

}
void Processor::Opcode1NNN(WORD opcode)
{
	int addr = opcode & 0x0FFF;
	m_ProgramCounter = opcode & 0x0FFF;;
}
void Processor::Opcode2NNN(WORD opcode)
{
	m_Stack.push_back(m_ProgramCounter);
	int addr = opcode & 0x0FFF;
	m_ProgramCounter = addr;
}
void Processor::Opcode3XNN(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx  = regx >> 8;
	int addr = opcode & 0xFF;
	if(m_Register[regx] == addr)
	{
		m_ProgramCounter += 2;
	}
}
void Processor::Opcode4XNN(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int addr = opcode & 0xFF;
	if(m_Register[regx] != addr)
	{
		m_ProgramCounter += 2;
	}
}
void Processor::Opcode5XY0(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int regy = opcode & 0x00F0;
	regy = regy >> 4;
	if(m_Register[regx] == m_Register[regy])
	{
		m_ProgramCounter += 2;
	}
}
void Processor::Opcode6XNN(WORD opcode)
{
	int regx = opcode & 0xF00;
	regx = regx >> 8;
	int addr = opcode & 0xFF;
	m_Register[regx] = addr;
}
void Processor::Opcode7XNN(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int addr = opcode & 0xFF;
	m_Register[regx] += addr;
}
void Processor::Opcode8XY0(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int regy = opcode & 0x00F0;
	regy = regy >> 4;
	m_Register[regx] = m_Register[regy];
}
void Processor::Opcode8XY1(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int regy = opcode & 0x00F0;
	regy = regy >> 4;
	m_Register[regx] = m_Register[regx] | m_Register[regy];
}
void Processor::Opcode8XY2(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int regy = opcode & 0x00F0;
	regy = regy >> 4;
	m_Register[regx] = m_Register[regx] & m_Register[regy];
}
void Processor::Opcode8XY3(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int regy = opcode & 0x00F0;
	regy = regy >> 4;
	m_Register[regx] = m_Register[regx] ^ m_Register[regy];
}
void Processor::Opcode8XY4(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int regy = opcode & 0x00F0;
	regy = regy >> 4;

	if(m_Register[regx] + m_Register[regy] > 0xFF)
	{
		m_Register[0xF] = 1;
	}
	else
	{
		m_Register[0xF] = 0;
	}
	m_Register[regx] += m_Register[regy];
}
void Processor::Opcode8XY5(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int regy = opcode & 0x00F0;
	regy = regy >> 4;

	if(m_Register[regy] > m_Register[regx])
	{
		m_Register[0xF] = 0;
	}
	else
	{
		m_Register[0xF] = 1;
	}

	m_Register[regx] -= m_Register[regy];
}
void Processor::Opcode8XY6(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	m_Register[0xF] = regx & 0x000F;
	m_Register[regx] = m_Register[regx] >> 1;
}
void Processor::Opcode8XY7(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int regy = opcode & 0x00F0;
	regy = regy >> 4;

	if(m_Register[regx] > m_Register[regy])
	{
		m_Register[0xF] = 0;
	}
	else
	{
		m_Register[0xF] = 1;
	}

	m_Register[regx] = m_Register[regy] - m_Register[regx];
}
void Processor::Opcode8XYE(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	m_Register[0xF] = regx & 0xF000;
	m_Register[regx] = m_Register[regx] << 1;
}
void Processor::Opcode9XY0(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int regy = opcode & 0x00F0;
	regy = regy >> 4;

	if(m_Register[regx] != m_Register[regy])
	{
		m_ProgramCounter += 2;
	}
}
void Processor::OpcodeANNN(WORD opcode)
{
	int addr = opcode & 0x0FFF;
	m_regI = addr;
}
void Processor::OpcodeBNNN(WORD opcode)
{
	int addr = opcode & 0x0FFF;
	m_ProgramCounter = addr + m_Register[0];
}
void Processor::OpcodeCXNN(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int addr = opcode & 0x00FF;
	m_Register[regx] = (rand() % 0xFF) & (addr);
}
void Processor::OpcodeDXYN(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int regy = opcode & 0x00F0;
	regy = regy >> 4;
	int height = opcode & 0x000F;

	int coordx = m_Register[regx];
	int coordy = m_Register[regy];

	m_Register[0xF] = 0;

	for(int yline = 0; yline < height; yline++)
	{
		BYTE data = m_GameMemory[m_regI+yline];
		int xpixelinv = 7;
		int xpixel = 0;

		for(xpixel = 0; xpixel < 8; xpixel++, xpixelinv--)
		{
			int mask = 1 << xpixelinv;
			if(data & mask)
			{
				BYTE x = coordx + xpixel;
				BYTE y = coordy + yline;
				if(m_ScreenData[x][y] == 1)
				{
					m_Register[0xF] = 1;
				}

				m_ScreenData[x][y] ^= 1;
			}
		}
	}
	Draw();
}
void Processor::OpcodeEX9E(WORD opcode)
{
	Input();
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int key = m_Register[regx];
	if(m_Input[key] == 1)
	{
		m_ProgramCounter += 2;
	}
}
void Processor::OpcodeEXA1(WORD opcode)
{
	Input();
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	int key = m_Register[regx];
	if(m_Input[key] != 1)
	{
		m_ProgramCounter += 2;
	}
}
void Processor::OpcodeFX07(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	m_Register[regx] = delayTimer;
}
void Processor::OpcodeFX0A(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;
	m_ProgramCounter -= 2;
	for(int i = 0x0; i < 0xF; i++)
	{
		if(m_Input[i] == 1)
		{
			m_Register[regx] = i;
			m_ProgramCounter += 2;
		}
	}
}
void Processor::OpcodeFX15(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;

	delayTimer = m_Register[regx];
}
void Processor::OpcodeFX18(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;

	soundTimer = m_Register[regx];
}
void Processor::OpcodeFX1E(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;

	m_regI += m_Register[regx];
}
void Processor::OpcodeFX29(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;

	m_regI = m_Register[regx] * 5;
}
void Processor::OpcodeFX33(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;

	int val = m_Register[regx];

	m_GameMemory[m_regI] = val / 100;
	m_GameMemory[m_regI+1] = val / 10 % 10;
	m_GameMemory[m_regI+2] = val  % 10;
	

}
void Processor::OpcodeFX55(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;

	for(int i = 0; i <= regx; i++)
	{
		m_GameMemory[m_regI+i] = m_Register[i];
	}

	m_regI = m_regI + regx + 1;
}
void Processor::OpcodeFX65(WORD opcode)
{
	int regx = opcode & 0x0F00;
	regx = regx >> 8;

	for(int i = 0; i <= regx; i++)
	{
		m_Register[i] = m_GameMemory[m_regI+i];
	}

	m_regI = m_regI + regx + 1;
}