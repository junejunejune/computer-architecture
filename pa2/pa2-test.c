//---------------------------------------------------------
// 
// Project #2: TinyFP Arithmetic
//
// April 5, 2018
//
// Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
// Systems Software Laboratory
// Dept. of Computer Science and Engineering
// Seoul National University
//
//---------------------------------------------------------


#include <stdio.h>
#include "pa2.h"

#define BYTE_FMT "%c%c%c%c%c%c%c%c"
#define BITS(byte)  (byte & 0x80 ? '1' : '0'), \
  					(byte & 0x40 ? '1' : '0'), \
  					(byte & 0x20 ? '1' : '0'), \
  					(byte & 0x10 ? '1' : '0'), \
  					(byte & 0x08 ? '1' : '0'), \
  					(byte & 0x04 ? '1' : '0'), \
  					(byte & 0x02 ? '1' : '0'), \
  					(byte & 0x01 ? '1' : '0') 

#define isZero(t)			(((t) & 0x7f) == 0)
#define isNaN(t)			(((((t) >> 3) & 0x0f) == 0x0f) && ((t) & 0x07))
#define PRINT_TINYFP(s,t1,t2,a,r)	\
							printf(BYTE_FMT" "s" "BYTE_FMT" = "BYTE_FMT" ("BYTE_FMT") %s\n",\
								BITS(t1), BITS(t2), BITS(a), BITS(r), \
								((isZero(a) && isZero(r)) || (isNaN(a) && isNaN(r)) || (a) == (r))? \
								"CORRECT" : "WRONG") 
#define PRINT_INT(s,t1,t2,a,r)	\
							printf(BYTE_FMT" "s" "BYTE_FMT" = %d (%d) %s\n",\
								BITS(t1), BITS(t2), a, r, (a) == (r)? "CORRECT" : "WRONG")

#define N 2


tinyfp literal[N] = {0b00111100, 0b01001010};
tinyfp addAnswer[N][N] = {0b01000100, 0b01001101, 0b01001101, 0b01010010};
tinyfp mulAnswer[N][N] = {0b01000001, 0b01001111, 0b01001111, 0b01011100};
int gtAnswer[N][N] = {0, 0, 1, 0};
int eqAnswer[N][N] = {1, 0, 0, 1};

int main(void){
	int i,j;
	tinyfp tf1, tf2, tf3;
	int result;

	printf("Test 1: Addition\n");
	for (i = 0; i < N; i++)
	{
		tf1 = literal[i];
		for (j = 0; j < N; j++)
		{
			tf2 = literal[j];
			tf3 = add(tf1, tf2);
			PRINT_TINYFP("+", tf1, tf2, tf3, addAnswer[i][j]);
		}
	}

	printf("Test 2: Multiplication\n");
	for (i = 0; i < N; i++)
	{
		tf1 = literal[i];
		for (j = 0; j < N; j++)
		{
			tf2 = literal[j];
			tf3 = mul(tf1, tf2);
			PRINT_TINYFP("*", tf1, tf2, tf3, mulAnswer[i][j]);
		}
	}

	printf("Test 3: Greater than\n");
	for (i = 0; i < N; i++)
	{
		tf1 = literal[i];
		for (j = 0; j < N; j++)
		{
			tf2 = literal[j];
			tf3 = gt(tf1, tf2);
			PRINT_INT(">", tf1, tf2, tf3, gtAnswer[i][j]);
		}
	}

	printf("Test 4: Equal to\n");
	for (i = 0; i < N; i++)
	{
		tf1 = literal[i];
		for (j = 0; j < N; j++)
		{
			tf2 = literal[j];
			tf3 = eq(tf1, tf2);
			PRINT_INT("==", tf1, tf2, tf3, eqAnswer[i][j]);
		}
	}

	return 0;
}
