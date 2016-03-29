#ifndef PRINTF_H
#define PRINTF_H

int exp2(int n);
int exp10(int n);
int mod(int x, int y);
int inttobin(char *buf, int n, unsigned val, int width);
int inttodec(char *buf, int n, int val, int width);
int inttohex(char *buf, int n, unsigned val, int width);

const char conversion[16] = "0123456789ABCDEF"; 
const int TEN = 10;
const int SIXTEEN = 16;

int vsnprintf(char* s, int n, char* format, va_list ap);

#endif