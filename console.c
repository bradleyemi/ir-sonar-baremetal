#include "fb.h"
#include "gl.h"
#include "font.h"
#include "malloc.h"
#include <stdarg.h>

extern int vsnprintf(char *, int, const char *, va_list);
extern int printf(const char *, ...);

extern color WHITE;
extern const unsigned WIDTH;
extern const unsigned HEIGHT;

unsigned int rows = 0;
unsigned int cols = 0;
char* buffer;
unsigned int cursorX = 0;
unsigned int cursorY = 0;

unsigned int noRefresh = 0;

void console_init( unsigned nrows, unsigned ncols ) {
	if (nrows*font_get_height() > HEIGHT) {
		printf("ERROR: The number of rows is too large for the display and font size specifications.\n");
		return;
	}
	if (ncols*font_get_width() > WIDTH) {
		printf("ERROR: The number of columns is too large for the display and font size specifications.\n");
		return;
	}
	printf("initializing\n");
	gl_init(WIDTH, HEIGHT, FB_DOUBLEBUFFER);
	char buf[10000]; 
	buffer = buf;
	rows = nrows;
	cols = ncols;
}


void console_update() {
	gl_clear(0);
	//printf("updating console at %d, %d\n", cursorX, cursorY);
	//printf("rflag %d\n", rFlag);
	//printf("tempbuf contents:\n");
	char (*tempBuf)[cols] = (char (*)[cols]) buffer;
    for (unsigned y = 0; y < cursorY; y++) {
    	//printf("executing\n");
    	for (unsigned x = 0; x < cols; x++) {
    		//printf("%c at %d,%d\n", tempBuf[y][x], x, y);
    		gl_draw_char(x*font_get_width(), y*font_get_height(), tempBuf[y][x], WHITE);
    	}
    }
    for (unsigned x = 0; x < cursorX; x++) {
    	//printf("%c at %d,%d\n", tempBuf[cursorY][x], x, cursorY);
    	gl_draw_char(x*font_get_width(), cursorY*font_get_height(), tempBuf[cursorY][x], WHITE);
    }
    gl_swap_buffer();
}


void console_scroll(void) {
	char (*tempBuf)[cols] = (char (*)[cols]) buffer;
	char (*newBuf)[cols] = tempBuf;
    for (unsigned y = 1; y < rows; y++) {
    	for (unsigned x = 0; x < cols; x++) {
    		tempBuf[y-1][x] = newBuf[y][x];
    	}
    }
    cursorX = 0;
    cursorY--;
}


void console_putc( unsigned char ch ) {
	char (*tempBuf)[cols] = (char (*)[cols]) buffer;
	int flag = 0;

	if (ch == '\0') {
		flag = 1;
	}
    
	if (ch == '\r') {
		if (cursorX == 0) {
			cursorY--;
		}
		else {
			cursorX = 0;
		}
		flag = 1;
	}
    
	if (ch == '\n') {
		for (int x = cursorX; x < cols; x++) {
			tempBuf[cursorY][x] = ' ';
		}
		cursorX = 0;
        cursorY++;
        flag = 1;
	}

	//use the cursor if no return flag is set
	if (ch == '\b') {
		if (cursorX == 0) {
			cursorY--;
			cursorX = rows-1;
		}
		else {
			cursorX--;
		}
		tempBuf[cursorY][cursorX] = ' ';
		flag = 1;
	}
 
	if (ch == '\f') {
		for (int y = 0; y < rows; y++) {
			for (int x = 0; x < cols; x++) {
				tempBuf[y][x] = ' ';
			}
		}
		cursorX = 0;
		cursorY = 0;
		flag = 1;
	}
	
	if (cursorY >= rows) console_scroll();
    if (flag == 0) {
		tempBuf[cursorY][cursorX] = ch;
	    //printf("tempbuf in putc %c at %d, %d\n", tempBuf[cursorY][cursorX], cursorX, cursorY);
        cursorX++;
        if (cursorX >= cols) {
    	    cursorX = 0;
    	    cursorY++;
        }
    }
    buffer = (char *) tempBuf;
    if (!noRefresh) console_update();
}

void console_printf( const char *format, ... ) {
	noRefresh = 1;
	//printf("running console printf on %s\n", format);
	char printBuf[1000];
    va_list ap;
	va_start(ap, format);

	int length = vsnprintf(printBuf, 10000, format, ap);

	unsigned bufIdx = 0;
	while (bufIdx != length) {
		console_putc(printBuf[bufIdx]);
		bufIdx++;
	}
	//printf("\n");
	noRefresh = 0;
	console_update();
}

char console_get_char( const unsigned row, unsigned col );
