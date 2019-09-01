#include <iostream>
#include "CChess.hpp"
/*!
	\mainpage Semestrial project Chess.
	This project is a variation of classic game Chess.
	Game is played throw command line.
	Game Fully observes all rules.
	More precise description see in 'zadani.txt' file.
	
	\warning 
	Saved games are moved into your current directory.
	
	\file Root of the program.
*/

/// Create new instance of CChess and launch it by calling 'Start()' on it.
int main (int argc, char* argv[]) {
	CChess chess(argv[1]);
	chess.Start();
return 0;
}
