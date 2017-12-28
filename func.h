#include <time.h>
#define TRUE 1
#define ROW 7		// Game screen's number of rows
#define COL 60		// Game screen's number of cols
#define UP_ARROW 72
#define DOWN_ARROW 80
#define SPACE 32
#define SCORE_RATE 4 // SCORE_RATE times shift counts as one score
#define CLD 7	// Number of cloud characters
#define GRD 6	// Number of ground characters
#define INITIAL_Y 6

typedef unsigned long long ull;

int shift_time = 50;	// Time of sleep per shift in milliseconds
ull no_shift   = 0;		/* Number of shifts from start of the game
						It's value increases on each shift
						player's score = no_shift / SCORE_RATE
						It's type is considered as unsigned long long */
int trex_jump  = 0;		/* If T-Rex is jumping the value is non-zero otherwise it is zero
						At the start T-Rex is not jumping so the value of start is zero
						It's value should be set zero after jump, again */
int trex_sit   = 0;		/* If T-Rex is sitting the value is non-zero otherwise it is zero
						At the start T-Rex is not sitting so the value of start is zero
						It's value should be set zero after sit, again
						*/
#define STEPSIZE 7		// Obtacle step array size
int obstacle_timer = 0; // Sets a value to check if its time to generate obstacle
int step[STEPSIZE] = { 15, 20, 20, 25, 30, 35, 40 }; /* Random distance of appereace of obstacle */
int rand_step = 1; // At the start appears an obstacle and then this value is chosen by step array
/*	Actually it somehow counts the number of shifts and
	when it reachs the value of rand_step it will generate an obstacle
	rand_step's value is chosen randomly from step array
*/

#define STYLE_NUMBER 3
char trex_style[][3] = {{')', 'v', ':'}, {'$', 'O', 'H'}, {'0', '1', '4'}};

char sky_obs[] = {'<', 174, '=', 254};
int sky_obs_size = sizeof(sky_obs) / sizeof(sky_obs[0]);

char ground_obs[] = {219, 222, '#'};
int ground_obs_size = sizeof(ground_obs) / sizeof(ground_obs[0]);

int initialY = INITIAL_Y;
// Keeps T-Rex' body symbols and coordinates
struct T_Rex{
	char symbol;
	int x;
	int y;
};
struct T_Rex head = {.symbol = '@', .x = ROW - 4, .y = INITIAL_Y};
struct T_Rex mid  = {.symbol = '&', .x = ROW - 3, .y = INITIAL_Y};
struct T_Rex foot = {.symbol = '&', .x = ROW - 2, .y = INITIAL_Y};

// Random number from 0 to m-1; Good for arrays :)
int random_number(int m)
{
	srand(rand() * time(NULL));
	return rand() % m;
}

void choose_your_t_rex()
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < STYLE_NUMBER; j++)
		{
			if(i == 3) printf("%d\t", j+1);
			else{
				printf("%c\t", trex_style[j][i]);
			}
		}
		printf("\n");
		if(i >= 2) printf("\n");
	}
	printf("Choose your T-Rex (-1 : Fully random): ");
	int tr;
	scanf("%d", &tr);
	if(tr == -1)
	{
		head.symbol = 33 + random_number(93);
		mid.symbol = 33 + random_number(93);
		foot.symbol = 33 + random_number(93);
		return;
	}
	tr--;
	head.symbol = trex_style[tr][0];
	mid.symbol = trex_style[tr][1];
	foot.symbol = trex_style[tr][2];
	
}

// Loading page at start :/
void loading()
{
	system("color 70");
	for(int i = 0; i < ROW/2; i++)
		printf("\n");
	printf("%37s\n%20s", "Loading...", "");
	for(int i = 0; i < 21; i++){
		printf("%c", 177);
		_sleep(120);
	}
	system("cls");
}

void change_screen_color()
{
	if(no_shift % 400 == 0){
		system("color 70");
		return;
	}
	if(no_shift % 200 == 0)
		system("color 7");
}

// Produces start screen
/*	Game's screen has no obstacle at start
	It starts with a ground and some moving clouds
	with T-Rex itself
*/
char cloud[CLD]  = {32, 32, 32, 32, 32, 176, 177};	// Clouds are just for fun :))
char ground[GRD] = {196, 196 , 196, 196, 196, 205};	// Ground is random for fun :))
// There are random clouds and grounds to feel moving :D
void load_start_scr(int m, int n, char scr[][n])
{
	for(int i = 0; i < n; i++){
		scr[ 0 ][i] =  cloud[random_number(CLD)];
		scr[m-1][i] = ground[random_number(GRD)];
	}
	
	for(int i = 1; i < m-1; i++)
		for(int j = 0; j < n; j++)
			scr[i][j] = SPACE;
	
	// Place T-Rex on start screen
	scr[head.x][head.y] = head.symbol;
	scr[ mid.x][ mid.y] = mid.symbol;
	scr[foot.x][foot.y] = foot.symbol;
}

/*	First it removes T-Rex from screen
	Second if will shift all columns to left
	For the last column it will produce the result
	by generating an obstacle if it was possible
*/

void shift(int m, int n, char scr[][n]){
	
	scr[head.x][head.y] = ' ';
	scr[ mid.x][ mid.y] = ' ';
	scr[foot.x][foot.y] = ' ';
	
	for(int i = 0; i < m; i++)
		for(int j = 0; j < n-1; j++)
			scr[i][j] = scr[i][j+1];
	scr[   0   ][COL - 1] = cloud [random_number(CLD)];
	scr[ROW - 1][COL - 1] = ground[random_number(GRD)];
	
	for(int i = 1; i < ROW - 1; i++)
			scr[i][COL - 1] = ' ';
	if( (no_shift - obstacle_timer) % rand_step == 0){
		rand_step = step[random_number(STEPSIZE)];
		obstacle_timer = no_shift;
		int random_obstacle = random_number(4);
		switch(random_obstacle)
		{
			case 0: // Ground single obstacle
				scr[ROW - 2][COL - 1] = ground_obs[random_number(ground_obs_size)];
				break;
			case 1: // Ground double obstacle
				scr[ROW - 2][COL - 1] = ground_obs[random_number(ground_obs_size)];
				scr[ROW - 2][COL - 2] = ground_obs[random_number(ground_obs_size)];
				break;
			case 2: // Middle sky obstacle
				scr[ROW - 3][COL - 1] = sky_obs[random_number(sky_obs_size)];
				break;
			case 3: // Sky obstacle
				scr[ROW - 4][COL - 1] = sky_obs[random_number(sky_obs_size)];
				break;
		}
	}
	no_shift++;
	change_screen_color();
}

// This function is used to print game on screen
void print_scr(int m, int n, char scr[][n])
{
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++)
			printf("%c", scr[i][j]);
		printf("\n");
	}
}

void set_next_trex_coordinates(int m, int n, char scr[][n]){
	if(trex_sit == 1){
		head.x = ROW - 2;
		head.y = initialY + 2;
		mid.x  = ROW - 2;
		mid.y  = initialY + 1;
		foot.x = ROW - 2;
		foot.y = initialY + 0; 
		//trex_sit = 0;
		trex_jump = 0;
		return;
	}
	
	head.x = ROW - 4;
	head.y = initialY;
	mid.x  = ROW - 3;
	mid.y  = initialY;
	foot.x = ROW - 2;
	foot.y = initialY;
	
	if(trex_jump != 0){
		switch(trex_jump){
			case 1:
			case 5:
				head.x--;
				mid.x--;
				foot.x--;
				trex_jump++;
				return;
			case 2:
			case 3:
			case 4:
				head.x -= 2;
				mid.x -= 2;
				foot.x -= 2;
				trex_jump++;
				return;
			case 6:
				trex_jump = 0;
				return;
		}
	}
	return;
}


int game_over(int m, int n, char scr[][n])
{
	if(	scr[head.x][head.y] != ' ' ||
		scr[ mid.x][ mid.y] != ' ' ||
		scr[foot.x][foot.y] != ' ') return 1; // Means GAME OVER
	scr[head.x][head.y] = head.symbol;
	scr[ mid.x][ mid.y] = mid.symbol;
	scr[foot.x][foot.y] = foot.symbol;
	return 0;
}

void change_shift_time_if_needed()
{
	if(no_shift % 70 == 0 && shift_time >= 15)
		shift_time -= 5;
}

void game_over_page()
{
	int cs = 0;
	
	while( TRUE )
	{
		system("cls");
		if( _kbhit() )
			break;
		
		if(cs % 2)
		{
			for(int i = 0; i < ROW/2; i++)
				printf("\n");
			printf("%37s", "!GAME OVER!");
			system("color C0");
		}
		else
		{
			for(int i = 0; i < ROW/2; i++)
				printf("\n");
			printf("%39s", "!!!GAME OVER!!!");
			system("color C");
		}
		_sleep(200);
		cs++;
	}
	
	
	
	
}
