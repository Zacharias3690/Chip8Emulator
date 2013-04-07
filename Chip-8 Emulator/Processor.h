#include <vector>
#include <stack>

typedef unsigned char BYTE;		//1 byte
typedef unsigned short WORD;	//2 bytes
typedef unsigned long DWORD;	//4 bytes

class Processor
{
public:
	//instructions per cycle, screen width, screen height
	Processor(int,int);
	~Processor();
	void Update();
	void UpdateTimers();
	void Input();
	void Draw();
	void DrawInit();
	void PlayAudio();
	WORD GetNextInstruction();

	void LoadFile(const char*);
	void LoadBlock(int);
	std::vector<std::vector<int>> m_ScreenData;
	int screenWidth;
	int screenHeight;
	int delayTimer;
	int soundTimer;
private:
	std::vector<WORD> program;
	int m_ProgramCounter;
	std::vector<int> m_Stack;
	unsigned char *m_Register;
	unsigned char m_GameMemory[3584];
	unsigned char m_Input[16];
	unsigned long m_regI;
	const char* file_location;

	SDL_Surface *screen;

	//Opcode functions
	void Opcode0NNN(WORD);
	void Opcode00E0(WORD);
	void Opcode00EE(WORD);
	void Opcode1NNN(WORD);
	void Opcode2NNN(WORD);
	void Opcode3XNN(WORD);
	void Opcode4XNN(WORD);
	void Opcode5XY0(WORD);
	void Opcode6XNN(WORD);
	void Opcode7XNN(WORD);
	void Opcode8XY0(WORD);
	void Opcode8XY1(WORD);
	void Opcode8XY2(WORD);
	void Opcode8XY3(WORD);
	void Opcode8XY4(WORD);
	void Opcode8XY5(WORD);
	void Opcode8XY6(WORD);
	void Opcode8XY7(WORD);
	void Opcode8XYE(WORD);
	void Opcode9XY0(WORD);
	void OpcodeANNN(WORD);
	void OpcodeBNNN(WORD);
	void OpcodeCXNN(WORD);
	void OpcodeDXYN(WORD);
	void OpcodeEX9E(WORD);
	void OpcodeEXA1(WORD);
	void OpcodeFX07(WORD);
	void OpcodeFX0A(WORD);
	void OpcodeFX15(WORD);
	void OpcodeFX18(WORD);
	void OpcodeFX1E(WORD);
	void OpcodeFX29(WORD);
	void OpcodeFX33(WORD);
	void OpcodeFX55(WORD);
	void OpcodeFX65(WORD);
};