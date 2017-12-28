/*	In the name of GOD
	Project name : Chrome T-Rex
	Developer : Mohammad Mahdi Khodabandeh
	Programming Language : C
	Language Standard : ISO C99
	TDM-GCC 4.9.2 64-bit Release
	Operating System : Windows
	IDE : Dev C++
	Fall 2017
*/
// You may recieve a warning about _sleep function... Ignore it ;)
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "func.h"	/* To keep it simple, just put our macros, typedef, functions,
					struct(s) and global variables in a header file named func.h */

int main( void )
{
	char scr[ROW][COL], key;
	ull score = no_shift / SCORE_RATE;
	choose_your_t_rex();
	load_start_scr(ROW, COL, scr); // Initializing scr for the start
	
	loading();
	
	print_scr(ROW, COL, scr);
	while( TRUE ){
		
		if( _kbhit() ){
			key = _getch();
			
			if( key == DOWN_ARROW ) trex_sit = 1;
			
			else if( (key == UP_ARROW || key == SPACE) && trex_jump == 0 ) {
				if( trex_sit == 1 ) trex_sit = -1;
				else trex_jump = 1;
			}
			
			else {} // Other keys are pressed so nothing happens
		}
		
		shift(ROW, COL, scr);
		set_next_trex_coordinates(ROW, COL, scr);
		if( game_over(ROW, COL, scr) ){
			if(initialY == 0)
				break;
			initialY--;
		}
		
		score = no_shift / SCORE_RATE;
		system("cls");
		print_scr(ROW, COL, scr);
		printf("\tScore : %llu\n", score);
		printf("\tLives : ");
		for(int i = 0; i < initialY; i++)
			printf("%c", 220);
		if(initialY == 0)
			printf("It's your last chance... Try your best :))");
		_sleep(shift_time);
		change_shift_time_if_needed();
	}
	// If we are here it means player lost
	printf("\n\t%s", "Oops...! You didn't try your best!'");
	_sleep(1500);
	game_over_page();
	return 0;
}
