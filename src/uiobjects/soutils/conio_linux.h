#ifndef CONIO_H
#define	CONIO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <ncurses.h>
#include <sys/ioctl.h>
#include <termios.h>

#define BLUE COLOR_BLUE
#define RED COLOR_RED
#define WHITE COLOR_WHITE
#define BLACK COLOR_BLACK
#define MAGENTA COLOR_MAGENTA
#define CYAN COLOR_CYAN
#define GREEN COLOR_GREEN
#define YELLOW COLOR_YELLOW
#define LIGHTBLUE COLOR_BLUE
#define LIGHTRED COLOR_RED
#define LIGHTGREEN COLOR_GREEN

#undef getch

/*void init_screen(void);
void gotoxy(int x,int y);
void clrscr(void);
void clreol(void);
void textbackground(int color);
void textcolor(int color);
void delline(void);
void window(int left, int top, int right, int bottom);
*/
int kbhit(void);
int kbhit2(void);
int kbhit3(void);
int getch(void);
int getche(void);
/*int wherex(void);
int wherey(void);
int cputs(char*);
char* cgets(char*);
int cprintf(const char*, ...);
int cscanf(const char*, ...);
*/
void disable_raw_mode();
void enable_raw_mode();
void resetTermios(void);
void initTermios(int echo);

#ifdef	__cplusplus
}
#endif

#endif	/* CONIO_H */
