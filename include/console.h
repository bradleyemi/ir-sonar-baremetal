#ifndef CONSOLE_H
#define CONSOLE_H

void console_init( unsigned nrows, unsigned ncols );

void console_putc( int ch );

void console_printf( const char *format, ... );

char console_get_char( const unsigned row, unsigned col );

#endif
