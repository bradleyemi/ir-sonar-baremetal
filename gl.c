#include <stdio.h>
#include "bcm.h"
#include "mailbox.h"
#include "fb.h"
#include "gl.h"
#include "font.h"

void gl_init(unsigned width, unsigned height, unsigned mode) {
	fb_init(width, height, 4, mode);
}

void gl_swap_buffer(void) {
	fb_swap_buffer();
}

color gl_color( unsigned char r, unsigned char g, unsigned char b ) {
    color out = 0xff000000 + (b << 16) + (g << 8) + r;
	return out;
}

color gl_color_alpha( unsigned char r, unsigned char g, unsigned char b, unsigned char alpha ) {
    color out = (alpha << 24) + (b << 16) + (g << 8) + r;
	return out;
}

unsigned gl_get_width(void) {
	return fb_get_width();
}
unsigned gl_get_height(void) {
	return fb_get_height();
}

void gl_clear(color c) {
	unsigned int * im = (unsigned int *) fb_get_draw_buffer();
	for(unsigned int pix = 0; pix < (gl_get_width()*gl_get_height()); pix++) {
    	*im++ = c;
    }
}

void gl_draw_pixel(int x, int y, color c) {
	if ((x > gl_get_width()) || (y > gl_get_height())) {
		printf("WARNING: Attempt to draw outside border\n");
		return;
	}
	else {
		unsigned (*im)[gl_get_width()] = (unsigned (*)[gl_get_width()])fb_get_draw_buffer();
        im[y][x] = c;
	}
}

void gl_draw_hline(int x1, int x2, int y, color c) {
	unsigned (*im)[gl_get_width()] = (unsigned (*)[gl_get_width()])fb_get_draw_buffer();
	for (unsigned i = x1; i <= x2; i++) {
        im[y][i] = c;
	}
}

void gl_draw_vline(int y1, int y2, int x, color c) {
	unsigned (*im)[gl_get_width()] = (unsigned (*)[gl_get_width()])fb_get_draw_buffer();
	for (unsigned i = y1; i <= y2; i++) {
		im[i][x] = c;
	}
}

void gl_draw_box(int x1, int x2, int y1, int y2, color c) {
	gl_draw_hline(x1,x2,y1,c);
	gl_draw_hline(x1,x2,y2,c);
	gl_draw_vline(y1,y2,x1,c);
	gl_draw_vline(y1,y2,x2,c);
}

void gl_draw_rect(int x, int y, int w, int h, color c) {
	unsigned (*im)[gl_get_width()] = (unsigned (*)[gl_get_width()])fb_get_draw_buffer();
	for (unsigned i = x; i < x+w; i++) {
		for (unsigned j = y; j < y+h; j++) {
            if ((i > gl_get_width() || j > gl_get_height())) {
            	printf("WARNING: Attempt to draw outside border\n");
            }
            else {
            	im[j][i] = c;
            }
		}
	}
}

void gl_draw_char(int x, int y, char letter, color c) {
	char tempBuf[font_get_size()];
	font_get_char(letter, tempBuf, font_get_size());
	color intTemp[font_get_size()/4];
	for (int i = 0; i < font_get_size(); i += 4) {
      intTemp[i/4] = tempBuf[i];
    }
	unsigned (*im)[gl_get_width()] = (unsigned (*)[gl_get_width()])fb_get_draw_buffer();
	unsigned bufIdx = 0;
	for (unsigned j = y; j < y+font_get_height(); j++) {
		for (unsigned i = x; i < x+font_get_width(); i++) {
			if ((i > gl_get_width() || j > gl_get_height())) {
            	printf("WARNING: Attempt to draw outside border\n");
            }
            else {
            	if (intTemp[bufIdx] != 0xff000000) {
            		unsigned char r = (c & 0x000000ff);
            		unsigned char g = (c & 0x0000ff00) >> 8;
            		unsigned char b = (c & 0x00ff0000) >> 16;
            		unsigned char alpha = intTemp[bufIdx];
            		im[j][i] = gl_color_alpha(r, g, b, alpha);
            	}
            }
            bufIdx++;
		}
	}
}

void gl_draw_string( int x, int y, char* string, color c) {
	while (*string != '\0') {
		gl_draw_char(x, y, *string++, c);
		x = x + font_get_width();
	}
}

unsigned gl_get_char_height(void) {
	return font_get_height();
}
unsigned gl_get_char_width(void) {
	return font_get_width();
}


