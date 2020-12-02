#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 30
#define HEIGHT 10
#define BUFMAX 20


char *optiuni[] = {
	"new game",
	"resume game",
	"configure map",
	"quit",
};

void destroy_in_advance (FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10]);

void player_turn (FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10], int x, int y);

void computer_turn (FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10], int x, int y, int kill_count, int comp_kill_count);

void gen_map (int h[][10]);

int is_ok (int h1[][10], int i1, int j1);

int rand_om (int min, int max);
 
void conf_map (FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10]);

void print_menu (WINDOW *men, int highlight);

void new_game (FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10]);

void cont_game (FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10]);

void meniu (FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10]);

void draw_map (int h[][10], int h1[][10], int hx[][10], int h1x[][10]);

void wait_for (unsigned int secs);

int status (FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10]);

int check_ship (int h[][10], int hx[][10], int i, int j);

int is_valid (int h[][10]);

int is_ok_2 (int h[][10], int i, int j);

int main ( int argc, char *argv[])
{
	FILE *harta;
	int h[10][10]={0}, i, j, h1[10][10]={0}, hx[10][10]={0}, h1x[10][10]={0};
	harta = fopen (argv[1], "r+");
	if (harta == NULL)
	{
		fprintf(stderr, "[Eroare]: Nu s-au dat argumente de comanda!\n" );
		return 1;
	}

	else
	{
		int i=0, j=0;
		char buffer [BUFMAX];
		while (fgets(buffer, BUFMAX, harta) != NULL)
		{
			for (j=0; j<10; j++)
			{
				if (buffer[j] == '1')
				{
					h[i][j] = 1;
				}
				if (buffer[j] == '0')
				{
					h[i][j] = 0;
				}
			}
			i++;
			if (i == 10)
				break;
		}
		meniu(harta, h, h1, hx, h1x);
		harta = fopen (argv[1], "w+");
		for (i=0; i<10; i++)
		{
			for (j=0; j<10; j++)
			{
				if (h[i][j] == 1)
				{
					fputc ('1', harta);
				}
				else
				{
					fputc( '0', harta);
				}
			}
			fputc ('\n', harta);
		}
		return 0;
		
	}
	
}

void print_menu (WINDOW *men, int highlight)
{
	int x, y, i;
	x = 2;
	y = 2;
	box(men, 0, 0);

	for (i = 0; i < 4; ++i)
	{
		if (highlight == i + 1)
		{
			wattron (men, A_REVERSE);
			mvwprintw(men, y, x, "%s", optiuni[i]);
			wattroff (men, A_REVERSE);
		}
		else
		{
			mvwprintw (men, y, x, "%s", optiuni[i]);
		}
		++y;
	}
	wrefresh(men);
}

void new_game(FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10])
{
	
	initscr();
	clear();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	start_color();
	init_pair (1, COLOR_WHITE, COLOR_RED);
	init_pair (2, COLOR_WHITE, COLOR_GREEN);
	refresh();
	int i, j;
	for (i=0; i<10; i++)
		for (j=0; j<10; j++)
		{
			hx[i][j]=0;
			h1x[i][j]=0;
		}
	draw_map(h, h1, hx, h1x);
	gen_map(h1);
	refresh();
	endwin();
	player_turn(harta, h, h1, hx, h1x, 52, 1);
	
}

void player_turn (FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10], int x, int y)
{
	mvprintw(25, 0, "                   ");
	start_color();
	init_pair (1, COLOR_WHITE, COLOR_RED);
	init_pair (2, COLOR_WHITE, COLOR_GREEN);
	init_pair (3, COLOR_RED, COLOR_BLACK);
	int c;
	int ok ;
	curs_set(1);
	int kill_count=0, comp_kill_count=0;
	mvprintw(23, 60, "destroy in advance");
	move (y, x);
	refresh();
	while (c != 'q')
	{
		c = getch();
		ok = 1;
		//curs_set(1);
		// move (y, x);
		// refresh();
		if (c != 10)
		{
			switch (c)
			{
				case KEY_UP:
				{				
					curs_set(1);
					refresh();
					if (y != 1 )
					{
						mvprintw(23, 60, "destroy in advance");
						move (y-=2, x);
					}
					if (y > 19)
					{
						y = 19;
						move( y, x);
					}
					break;
				}
				case KEY_DOWN:
				{
					if (y == 19)
					{
						move (y+=2, x);
						curs_set(0);
						refresh();
						attron (A_REVERSE);
						mvprintw(23, 60, "destroy in advance");
						attroff (A_REVERSE);
					}
					if (y < 19)
					{
						move (y+=2, x);
					}		
					break;
				}
				case KEY_RIGHT:
				{
					if (x != 88 && y<=19)
					{
						move (y, x+=4);
					}
					break;
				}
				case KEY_LEFT:
				{
					if (x != 52 && y <= 19)
					{
						move (y, x-=4);
					}
					break;
				}

			}
		}
		else 
		{
			if (h1x[ (y+1) / 2 - 1][ (x-52)/ 4] == 1 || h1x[ (y+1) / 2 - 1][ (x-52)/ 4] == -1)
			{
				curs_set(0);
				mvprintw (25, 0, "Spatiul a fost deja selectat");
				refresh();
				wait_for(1);
				mvprintw (25, 0, "                            ");
				refresh();
				curs_set(1);
				move(y, x);
			}
			if (h1x[ (y+1) / 2 - 1][ (x-52)/ 4] == 0)
			{
				if ( h1[ (y+1) / 2 - 1][ (x-52)/ 4] == 1)
				{
					attron (COLOR_PAIR(3));
					mvaddch(y, x, 'X');
					attroff (COLOR_PAIR(3));
					h1x[ (y+1) / 2 - 1][ (x-52)/ 4] = 1;
					if (check_ship(h1, h1x, (y+1) / 2 - 1, (x-52)/ 4) == 0)
					{
						kill_count++;
						mvprintw(25, 0, "nava distrusa");
						refresh();
						wait_for(2);
						mvprintw(25, 0, "             ");
						refresh();
						move (y, x);
					}
					refresh();
				}
				else
				{
					attron (COLOR_PAIR(1));
					mvaddch(y, x, ' ');
					attroff(COLOR_PAIR(1));	
					refresh();
					h1x[ (y+1) / 2 - 1][ (x-52)/ 4] = -1;
					ok = 0;
				}
				if (status(harta, h, h1, hx, h1x) == 1)
				{
					mvprintw(23, 2, "ai castigat");
					mvprintw(24, 2, "ai distrus %d nave", kill_count);
					mvprintw(25, 2, "calculatorul a distrus %d nave", comp_kill_count);
					refresh();
				}
				if (ok == 0)
				{
					computer_turn(harta, h, h1, hx, h1x, x, y, kill_count, comp_kill_count);
					move(y,x);
					curs_set(1);
					refresh();
				}
			}
				
			if (y > 19)
			{
				destroy_in_advance(harta, h, h1, hx, h1x);
			}

				
		}
	}
	endwin();
	meniu(harta, h, h1, hx, h1x);
		
}

void computer_turn (FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10], int x, int y, int kill_count, int comp_kill_count)
{
	
	start_color();
	curs_set (0);
	init_pair (1, COLOR_WHITE, COLOR_RED);
	init_pair (3, COLOR_RED, COLOR_BLACK);
	int k, l, ok=1;
	
	while (ok)
	{
		srand(time(0));
		k=rand_om(0, 9);
		l=rand_om(0, 9);
		if (hx[k][l] != 1)
		{
			wait_for(3);
			if (h[k][l] == 1)
			{
				attron (COLOR_PAIR(3));
				mvaddch( (k+1)*2 - 1, 2 + l*4, 'X');
				attroff (COLOR_PAIR(3));
				hx[k][l] = 1;
				refresh();
				if (check_ship(h, hx, (y+1) / 2 - 1, (x-2)/ 4) == 0)
				{
					comp_kill_count++;
				}
			}
			else
			{
				attron (COLOR_PAIR(1));
				mvaddch( (k+1)*2 - 1, 2 + l*4, ' ');
				attroff (COLOR_PAIR(1));
				refresh();
				hx[k][l] = -1;
				ok = 0;
			}
			
		}
		
		
	}
	if (status(harta, h, h1, hx, h1x) == -1)
	{
		mvprintw(23, 2, "calculatorul a castigat");
		mvprintw(24, 2, "ai distrus %d nave", kill_count);
		mvprintw(25, 2, "calculatorul a distrus %d nave", comp_kill_count);
		refresh();			
	}
}

void wait_for (unsigned int secs) 
{
    unsigned int retTime = time(0) + secs;   
    while (time(0) < retTime);               
}

void cont_game(FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10])
{
	initscr();
	clear();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	start_color();
	init_pair (1, COLOR_WHITE, COLOR_RED);
	init_pair (2, COLOR_WHITE, COLOR_GREEN);
	refresh();
	draw_map(h, h1, hx, h1x);
	player_turn(harta, h, h1, hx, h1x, 52, 1);
	
}

void meniu(FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10])
{
	int startx = 0;
	int starty = 0;
	WINDOW *men;
	int highlight = 1;
	int opt = 0;
	int c;
	initscr();
	clear();
	noecho();
	cbreak();
	startx = (80 - WIDTH) / 2;
	starty = (24 - HEIGHT) / 2;
	men = newwin (HEIGHT, WIDTH, starty, startx);
	keypad(men, TRUE);
	mvprintw(0, 0, "Miscati-va din sageti, pentru selectare apasati Enter");
	refresh();
	print_menu(men, highlight);
	while (c)
	{
		c = wgetch (men);
		switch (c)
		{
			case KEY_UP:
			{																										
				if (highlight == 1)
					highlight = 4;
				else 
					--highlight;
				break;
			}
			case KEY_DOWN:
			{
				if (highlight == 4)
					highlight = 1;
				else
					++highlight;
				break;
			}
			case 10:
			{
				opt = highlight;
				break;
			}
		}
		print_menu(men, highlight);
		if (opt)
		{
			break;
		}
	}
	switch (opt)
	{
		case 1:
		{
			endwin();
			new_game(harta, h, h1, hx, h1x);
			break;
		}
		case 2:
		{
			endwin();
			cont_game(harta, h, h1, hx, h1x);
			break;
		}
		case 3:
		{
			endwin();
			conf_map(harta, h, h1, hx, h1x);
			break;
		}
		case 4:
		{
			endwin();
			return;
			break;
		}
	}
	
}

void draw_map(int h[][10], int h1[][10], int hx[][10], int h1x[][10]) 
{
	
	int i, j;
	start_color();
	init_pair (1, COLOR_WHITE, COLOR_RED);
	init_pair (3, COLOR_RED, COLOR_BLACK);
	for(i=0;i<=20;i++) 
	{
		mvaddch(i, 0, '|');
		mvaddch(i, 4, '|');
		mvaddch(i, 8, '|');
		mvaddch(i, 12,'|');
		mvaddch(i, 16, '|');
		mvaddch(i, 20, '|');
		mvaddch(i, 24, '|');
		mvaddch(i, 28, '|');
		mvaddch(i, 32, '|');
		mvaddch(i, 36, '|');
		mvaddch(i, 40, '|');

		mvaddch(i, 50, '|');
		mvaddch(i, 54, '|');
		mvaddch(i, 58, '|');
		mvaddch(i, 62, '|');
		mvaddch(i, 66, '|');
		mvaddch(i, 70, '|');
		mvaddch(i, 74, '|');
		mvaddch(i, 78, '|');
		mvaddch(i, 82, '|');
		mvaddch(i, 86, '|');
		mvaddch(i, 90, '|');
		if(i%2 == 0)
		{
			for (j=0; j<=40; j++)
			{
				mvaddch(i, j, '-');
			}

			for(j=50; j<=90; j++) 
			{
				mvaddch(i, j, '-');
			}

		}
		
	}
	for (i=0; i<10; i++)
	{
		for (j=0; j<10; j++)
		{
			if (h[i][j] == 1)
			{
				mvaddch( (i+1)*2 -1, 2 + j*4, 'X' );
			}
			if (hx[i][j] == 1)
			{
				attron(COLOR_PAIR(3));
				mvaddch( (i+1)*2 -1, 2 + j*4, 'X' );
				attroff(COLOR_PAIR(3));
			}
			if (hx[i][j] == -1)
			{
				attron(COLOR_PAIR(1));
				mvaddch( (i+1)*2 -1, 2 + j*4, ' ' );
				attroff(COLOR_PAIR(1));
			}

			if (h1x[i][j] == 1)
			{
				attron(COLOR_PAIR(3));
				mvaddch( (i+1)*2 -1, 52 + j*4, 'X' );
				attroff(COLOR_PAIR(3));
				if (check_ship(h1, h1x, i, j) == 0)
				{

				}
			}
			if (h1x[i][j] == -1)
			{
				attron(COLOR_PAIR(1));
				mvaddch( (i+1)*2 -1, 52 + j*4, ' ' );
				attroff(COLOR_PAIR(1));
			}
		}
	}

	for (j=58; j<=82; j++)
	{
		mvaddch(22, j, '-');
		mvaddch(24, j, '-');
	}
	mvaddch(23, 58, '|');
	mvaddch(23, 82, '|');
	mvprintw(23, 60, "destroy in advance");
	refresh();
}

void conf_map(FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10])
{
	initscr();
	clear();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	start_color();
	init_pair (1, COLOR_BLACK, COLOR_BLACK);
	init_pair (2, COLOR_GREEN, COLOR_BLACK);
	refresh();
	int i, j, H[10][10]={0};
	mvprintw(4, 50, "apasati Enter pentru selectia unei casute");
	mvprintw(5, 50, "si Backspace pentru eliminarea ei");
	mvprintw(7, 50, "o configuratie corecta contine:");
	mvprintw(8, 50, "1 nava de 4 casute");
	mvprintw(9, 50, "2 nave de 3 casute");
	mvprintw(10, 50, "3 nave de 2 casute");
	mvprintw(11, 50, "4 nave de 1 casuta");
	mvprintw(12, 50, "navele nu trebuie sa se invecineze in colturi");
	for(i=0;i<=20;i++) 
	{
		mvaddch(i, 0, '|');
		mvaddch(i, 4, '|');
		mvaddch(i, 8, '|');
		mvaddch(i, 12,'|');
		mvaddch(i, 16, '|');
		mvaddch(i, 20, '|');
		mvaddch(i, 24, '|');
		mvaddch(i, 28, '|');
		mvaddch(i, 32, '|');
		mvaddch(i, 36, '|');
		mvaddch(i, 40, '|');
		if(i%2 == 0)
		{
			for (j=0; j<=40; j++)
			{
				mvaddch(i, j, '-');
			}
		}
		
	}
	for (i=0; i<10; i++)
	{
		for (j=0; j<10; j++)
		{
			H[i][j] = h[i][j];
			if (h[i][j] == 1)
			{
				attron (COLOR_PAIR(2));
				mvaddch( (i+1)*2 - 1, 2 + j*4, 'X');
				attroff (COLOR_PAIR(2));
			}
		}
	}
	refresh();
	int x, y, c;
	x=2;
	y=1;
	move (y, x);
	while (c )
	{
		c = getch();
		if (c != 10)
		{
			switch (c)
			{
				case KEY_UP:
				{
					if (y != 1 )
					{
						move (y-=2, x);
					}
					break;
				}
				case KEY_DOWN:
				{
					if (y != 19)
					{
						move (y+=2, x);
					}
					break;
				}
				case KEY_RIGHT:
				{
					if (x != 38)
					{
						move (y, x+=4);
					}
					break;
				}
				case KEY_LEFT:
				{
					if (x != 2)
					{
						move (y, x-=4);
					}
					break;
				}
				case KEY_BACKSPACE:
				{
					H[ (y +1)/2 -1 ][ (x-2)/4 ] = 0;
					mvaddch(y, x, ' ');
					refresh();
					break;
				}

			}
			

		}
		else 
		{
			H[ (y +1)/2 -1 ][ (x-2)/4 ] = 1;
			attron (COLOR_PAIR(2));
			mvaddch(y, x, 'X');
			attroff (COLOR_PAIR(2));
			refresh();
		}
		if (c == 'q')
		{
			if (is_valid(H) == 0)
			{
				curs_set(0);
				mvprintw(25, 0, "configuratia nu e valida");
				refresh();
				wait_for(2);
				mvprintw(25, 0, "                        ");
				move(y, x);
				curs_set(1);
				refresh();

			}
			else 
			{
				for (i=0; i<10; i++)
				{
					for (j=0; j<10; j++)
					{
						h[i][j] = H[i][j];
					}
				}
				break;
			}
		}

	}
	endwin();
	meniu(harta, h, h1, hx, h1x);
	
	
}

void gen_map(int h1[][10])
{
	mvprintw(25, 0, "se incarca harta...");
	mvprintw(26, 0, "poate dura 20-30 de secunde");
	refresh();
	int i1, j1, k, ok;
	int min, max;
	for (k=0; k<1; k++)
	{
		min=0;
		max=6;
		srand(time(0));
		i1=rand_om(0, 9);
		j1=rand_om (min, max);
		if (is_ok(h1, i1, j1))
		{
			int l;
			for (l=0; l<4; l++)
			{
				h1[i1][j1+l] = 1;
			}
		}
	}
	for (k=0; k<2; k++)
	{
		ok=0;
		while (ok != 1)
		{
			min=2;
			max=7;
			srand(time(0));
			i1=rand_om(min, max);
			j1=rand_om(min, max);
			if (is_ok(h1, i1, j1) && is_ok(h1, i1+1, j1) && is_ok (h1, i1+2, j1))
			{
				h1[i1][j1]=1;
				h1[i1+1][j1]=1;
				h1[i1+2][j1]=1;
				ok=1;
			}
			else 
			{
				if (is_ok(h1, i1, j1) && is_ok(h1, i1-1, j1) && is_ok(h1, i1-2, j1))
				{
					h1[i1][j1]=1;
					h1[i1-1][j1]=1;
					h1[i1-2][j1]=1;
					ok=1;
					
				}
				else
				{
					if (is_ok(h1, i1, j1) && is_ok(h1, i1, j1+1) && is_ok(h1, i1, j1+2))
					{
						h1[i1][j1]=1;
						h1[i1][j1+1]=1;
						h1[i1][j1+2]=1;
						ok=1;
						
					}
					else
					{
						if (is_ok(h1, i1, j1) && is_ok(h1, i1, j1-1) && is_ok(h1, i1, j1-2))
						{
							h1[i1][j1]=1;
							h1[i1][j1-1]=1;
							h1[i1][j1-2]=1;
							ok=1;
							
						}
					}
				}
			}
		}
	}
	for (k=0; k<3; k++)
	{
		ok=0;
		while (ok != 1)
		{
			min=1;
			max=8;
			srand(time(0));
			i1=rand_om(min, max);
			j1=rand_om(min, max);
			if (is_ok(h1, i1, j1) && is_ok(h1, i1, j1+1))
			{
				h1[i1][j1]=1;
				h1[i1][j1+1]=1;
				ok=1;
				
			}
			else 
			{
				if (is_ok(h1, i1, j1) && is_ok(h1, i1, j1-1))
				{
					h1[i1][j1]=1;
					h1[i1][j1-1]=1;
					ok=1;
					
				}
				else
				{	
					if (is_ok(h1, i1, j1) && is_ok(h1, i1+1, j1))
					{
						h1[i1][j1]=1;
						h1[i1+1][j1]=1;
						ok=1;
						
					}
					else
					{
						if (is_ok(h1, i1, j1) && is_ok(h1, i1-1, j1) )
						{
							h1[i1][j1]=1;
							h1[i1-1][j1]=1;
							ok=1;
							
						}
					}
				}
			}
		}

	}
	for (k=0; k<4; k++)
	{
		ok=0;
		while (ok != 1)
		{
			min=0;
			max=9;
			srand(time(0));
			i1=rand_om(min, max);
			j1=rand_om(min, max);
			if (is_ok(h1, i1, j1))
			{
				h1[i1][j1] = 1;
				ok=1;
			}
		}
	}
	mvprintw(25, 0, "                    ");
	mvprintw(26, 0, "                               ");
}

int is_ok (int h1[][10], int i, int j)
{
	if (h1[i][j] == 1 || h1[i-1][j-1] == 1 || h1[i-1][j] == 1 || h1[i-1][j+1] == 1 || h1[i][j+1] == 1 || h1[i+1][j+1] == 1 || h1[i+1][j] == 1 || h1[i+1][j-1] ==1 || h1[i][j-1]==1)
		return 0;
	return 1;
}

int rand_om(int min, int max) 
{ 
    int nr; 
    nr = (rand() % (max - min + 1)) + min; 
    return nr; 
} 

void destroy_in_advance (FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10])
{
	int i, j, k, ok;
	start_color();
	init_pair (1, COLOR_WHITE, COLOR_RED);
	init_pair (3, COLOR_RED, COLOR_BLACK);
	srand(time(0));
	int nr=0;
	for (i=0; i<10; i++)
	{
		for (j=0; j<10; j++)
		{
			if (hx[i][j] == 0)
			{
				nr++;
			}
		}
	}
	if (nr >= 10)
	{
		for (k=0; k<10; k++)
		{
			ok=0;
			while (ok != 1)
			{
				i=rand_om(0, 9);
				j=rand_om(0, 9);
				if (hx[i][j] == 0)
				{
					ok = 1;
					if ( h[i][j] == 1)
					{
						attron (COLOR_PAIR(3));
						mvaddch( (i+1)*2 - 1, 2+ j*4, 'X');
						attroff (COLOR_PAIR(3));
						hx[i][j] = 1;
					}
					else
					{
						attron (COLOR_PAIR(1));
						mvaddch( (i+1)*2 - 1, 2+ j*4, ' ');
						attroff(COLOR_PAIR(1));	
						hx[i][j] = -1;
					}
				}
			}
		
		}

		for (k=0; k<10; k++)
		{
			ok = 0;
			while (ok != 1)
			{	
				i=rand_om(0, 9);
				j=rand_om(0, 9);
				if (h1x[i][j] == 0)
				{
					ok = 1;
					if ( h1[i][j] == 1)
					{
						attron (COLOR_PAIR(3));
						mvaddch( (i+1)*2 - 1, 52 + j*4, 'X');
						attroff (COLOR_PAIR(3));
						h1x[i][j] = 1;
						if (check_ship (h1, h1x, i, j) == 0)
						{
							refresh();
						}
					}
					else
					{
						attron (COLOR_PAIR(1));
						mvaddch( (i+1)*2 - 1, 52 + j*4, ' ');
						attroff(COLOR_PAIR(1));	
						h1x[i][j] = -1;
					}
				}
			}
		
		}
	}
	else
	{
		for (i=0; i<10; i++)
		{
			for (j=0; j<10; j++)
			{
				if (hx[i][j] == 0)
				{
					if ( h[i][j] == 1)
					{
						attron (COLOR_PAIR(3));
						mvaddch( (i+1)*2 - 1, 2+ j*4, 'X');
						attroff (COLOR_PAIR(3));
						hx[i][j] = 1;
					}
					else
					{
						attron (COLOR_PAIR(1));
						mvaddch( (i+1)*2 - 1, 2+ j*4, ' ');
						attroff(COLOR_PAIR(1));	
						hx[i][j] = -1;
					}
				}

				if (h1x[i][j] == 0)
				{
					if ( h1[i][j] == 1)
					{
						attron (COLOR_PAIR(3));
						mvaddch( (i+1)*2 - 1, 52 + j*4, 'X');
						attroff (COLOR_PAIR(3));
						h1x[i][j] = 1;
						if (check_ship (h1, h1x, i, j) == 0)
						{
							refresh();
						}
					}
					else
					{
						attron (COLOR_PAIR(1));
						mvaddch( (i+1)*2 - 1, 52 + j*4, ' ');
						attroff(COLOR_PAIR(1));	
						h1x[i][j] = -1;
					}
				}
			}
		}
	}
	
	// if (status(harta, h, h1, hx, h1x) == -1)
	// {
	// 	mvprintw(23, 2, "calculatorul a castigat");
	// 	refresh();			
	// }

	// if (status(harta, h, h1, hx, h1x) == 1)
	// {
		
	// 	mvprintw(23, 2, "ai castigat");
	// 	refresh();			
	// }
	
	refresh();
}

int status (FILE *harta, int h[][10], int h1[][10], int hx[][10], int h1x[][10])
{
	int i, j, ok;
	ok = -1;
	for (i=0; i<10; i++)
	{
		for (j=0; j<10; j++)
		{
			if (h[i][j] == 1 && hx[i][j] != 1)
			{
				ok = 0;
			}
		}
	}
	if (ok == -1)
	{
		return ok;
	}

	ok = 1;
	for (i=0; i<10; i++)
	{
		for (j=0; j<10; j++)
		{
			if (h1[i][j] == 1 && h1x[i][j] != 1)
			{
				ok = 0;
			}
		}
	}
	if (ok == 1)
	{
		return ok;
	}
}


int check_ship (int h[][10], int hx[][10], int i, int j)
{
	int l1=0, l2=0, k;
	start_color();
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	if (h[i][j] == 1)
	{
		l1++;
		l2++;
		if (h[i-1][j] != 1 && h[i][j+1] != 1 && h[i+1][j] != 1 && h[i][j-1] != 1)
		{
			if (hx[i][j] == 1)
			{
				attron (COLOR_PAIR(4));
				mvaddch((i+1)*2 - 1, 52 + j*4, 'X');
				attroff (COLOR_PAIR(4));
				return 0;
			}
			else
			{
				return 1;
			}
		}

		else
		{
			if (h[i-1][j] == 1)
			{
				l1++;
				if (h[i-2][j] == 1)
				{
					l1++;
					if (h[i-3][j] == 1)
					{
						l1++;
					}
				}
				if (h[i+1][j] == 1)
				{
					l2++;
					if (h[i+2][j] == 1)
					{
						l2++;
					}
				}
				for (k=0; k<l1; k++)
				{
					if (hx[i-k][j] != h[i-k][j])
						return 1;
				}
				for (k=0; k<l2; k++)
				{
					if (hx[i+k][j] != h[i+k][j])
						return 1;
				}

				for (k=0; k<l1; k++)
				{
					attron (COLOR_PAIR(4));
					mvaddch((i-k+1)*2 - 1, 52 + j*4, 'X');
					attroff (COLOR_PAIR(4));
				}
				for (k=0; k<l2; k++)
				{
					attron (COLOR_PAIR(4));
					mvaddch((i+k+1)*2 - 1, 52 + j*4, 'X');
					attroff (COLOR_PAIR(4));
				}
				return 0;
			}

			if (h[i][j+1] == 1)
			{
				l1++;
				if (h[i][j+2] == 1)
				{
					l1++;
					if (h[i][j+3] == 1)
					{
						l1++;
					}
				}
				if (h[i][j-1] == 1)
				{
					l2++;
					if (h[i][j-2] == 1)
					{
						l2++;
					}
				}
				for (k=0; k<l1; k++)
				{
					if (hx[i][j+k] != h[i][j+k])
						return 1;
				}
				for (k=0; k<l2; k++)
				{
					if (hx[i][j-k] != h[i][j-k])
						return 1;
				}
				
				for (k=0; k<l1; k++)
				{
					attron (COLOR_PAIR(4));
					mvaddch((i+1)*2 - 1, 52 + (j+k)*4, 'X');
					attroff (COLOR_PAIR(4));
				}
				for (k=0; k<l2; k++)
				{
					attron (COLOR_PAIR(4));
					mvaddch((i+1)*2 - 1, 52 + (j-k)*4, 'X');
					attroff (COLOR_PAIR(4));
				}
				return 0;
			}

			if (h[i+1][j] == 1)
			{
				l1++;
				if (h[i+2][j] == 1)
				{
					l1++;
					if (h[i+3][j] == 1)
					{
						l1++;
					}
				}
				if (h[i-1][j] == 1)
				{
					l2++;
					if (h[i-2][j] == 1)
					{
						l2++;
					}
				}
				for (k=0; k<l1; k++)
				{
					if (hx[i+k][j] != h[i+k][j])
						return 1;
				}
				for (k=0; k<l2; k++)
				{
					if (hx[i-k][j] != h[i-k][j])
						return 1;
				}
				
				for (k=0; k<l1; k++)
				{
					attron (COLOR_PAIR(4));
					mvaddch((i+k+1)*2 - 1, 52 + j*4, 'X');
					attroff (COLOR_PAIR(4));
				}
				for (k=0; k<l2; k++)
				{
					attron (COLOR_PAIR(4));
					mvaddch((i-k+1)*2 - 1, 52 + j*4, 'X');
					attroff (COLOR_PAIR(4));
				}
				return 0;
			}
			if (h[i][j-1] == 1)
			{
				l1++;
				if (h[i][j-2] == 1)
				{
					l1++;
					if (h[i][j-3] == 1)
					{
						l1++;
					}
				}
				if (h[i][j+1] == 1)
				{
					l2++;
					if (h[i][j+2] == 1)
					{
						l2++;
					}
				}
				for (k=0; k<l1; k++)
				{
					if (hx[i][j-k] != h[i][j-k])
						return 1;
				}
				for (k=0; k<l2; k++)
				{
					if (hx[i][j+k] != h[i][j+k])
						return 1;
				}
				
				for (k=0; k<l1; k++)
				{
					attron (COLOR_PAIR(4));
					mvaddch((i+1)*2 - 1, 52 + (j-k)*4, 'X');
					attroff (COLOR_PAIR(4));
				}
				for (k=0; k<l2; k++)
				{
					attron (COLOR_PAIR(4));
					mvaddch((i+1)*2 - 1, 52 + (j+k)*4, 'X');
					attroff (COLOR_PAIR(4));
				}
				return 0;
			}
		}
	}
}

int is_valid (int h[][10])
{
	int i, j, l, k;
	int c[10][10];
	int nr_nave=0, nave[4]={0};
	for (i=0; i<10; i++)
	{
		for (j=0; j<=10; j++)
		{
			c[i][j] = h[i][j];
		}
	}
	for (i=0; i<10; i++)
	{
		for (j=0; j<10; j++)
		{
			if (c[i][j] == 1 && is_ok_2(h, i, j) == 1)
			{
				l=1;
				nr_nave++;
				if (c[i+1][j] == 1 && is_ok_2(h, i+1, j) == 1)
				{
					l++;
					if (c[i+2][j] == 1 && is_ok_2(h, i+2, j) == 1)
					{
						l++;
						if (c[i+3][j] == 1 && is_ok_2(h, i+3, j) == 1)
						{
							l++;
						}
					}
					for (k=0; k<l; k++)
					{
						c[i+k][j] = 0;
					}
					
				}
				
				if (c[i][j+1] == 1 && is_ok_2(h, i, j+1) == 1)
				{
					l++;
					if (c[i][j+2] == 1 && is_ok_2(h, i, j+2) == 1)
					{
						l++;
						if (c[i][j+3] == 1 && is_ok_2(h, i, j+3) == 1)
						{
							l++;
						}
					}
					for (k=0; k<l; k++)
					{
						c[i][j+k] = 0;
					}
					
				}
				if (l > 4)
				{
					return 0;
				}
				nave[l]++;
			}
		}
	}

	if (nr_nave == 10 && nave[1] == 4 && nave[2] == 3 && nave[3] == 2 && nave[4] == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


int is_ok_2 (int h[][10], int i, int j)
{
	if (h[i-1][j-1] == 1 || h[i-1][j+1] == 1 || h[i+1][j+1] == 1 || h[i+1][j-1] ==1)
		return 0;
	return 1;
}