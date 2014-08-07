//main.cpp - Main entry point for the program.
//Programmed by Ed Lu

#include "SDLGame.h"

extern SDLGame theGame = SDLGame();

int main(int argc, char* argv[])
{
	return theGame.Execute();
}