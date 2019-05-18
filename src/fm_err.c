#include "fm_global.h"
#include "fm_err.h"


void fm_err (const char *fmt, ...)
{
    int y = getmaxy(stdscr) - 1;
    move(y, 0);
    clrtoeol();
	va_list argp;
	va_start(argp, fmt);
	vwprintw(stdscr, fmt, argp);
	va_end(argp);
    refresh();
}

void fm_errx (const char *fmt, ...)
{
    int y = getmaxy(stdscr) - 1;
    move(y, 0);
    clrtoeol();
	va_list argp;
	va_start(argp, fmt);
	vwprintw(stdscr, fmt, argp);
	va_end(argp);
    refresh();
}

void fm_warn (const char *fmt, ...)
{
    
    int y = getmaxy(stdscr) - 1;
    move(y, 0);
    clrtoeol();
	va_list argp;
	va_start(argp, fmt);
	vwprintw(stdscr, fmt, argp);
	va_end(argp);
    refresh();
}

int err(int code, const char *fmt, ...)
{
    int y = getmaxy(stdscr) - 1;
    move(y, 0);
    clrtoeol();
	va_list argp;
	va_start(argp, fmt);
	vwprintw(stdscr, fmt, argp);
	va_end(argp);
    refresh();
    return code;
}

int errx(int code, const char *fmt, ...)
{
    int y = getmaxy(stdscr) - 1;
    move(y, 0);
    clrtoeol();
	va_list argp;
	va_start(argp, fmt);
	vwprintw(stdscr, fmt, argp);
	va_end(argp);
    refresh();
    return code;
}

void warn (const char *fmt, ...)
{
    
    int y = getmaxy(stdscr) - 1;
    move(y, 0);
    clrtoeol();
	va_list argp;
	va_start(argp, fmt);
	vwprintw(stdscr, fmt, argp);
	va_end(argp);
    refresh();
}