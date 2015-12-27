#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>


/* UART handler declaration */
extern UART_HandleTypeDef UartHandle;

void initPrint(void);

void printInt(int num);
void printDouble(double num);
void printString(char* phrase);