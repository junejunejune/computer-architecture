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

#include "pa2.h"
#include <stdio.h>
// If this file contains any "float" or "double" word,
// it will be rejected by the automatic grader.
// (Do not use those words even in comments.)

typedef unsigned char tinyfp;
int check_infinity(tinyfp tf);
int check_NaN(tinyfp tf);
int check_zero(tinyfp tf);

tinyfp add(tinyfp tf1, tinyfp tf2)
{
    //special cases
    if(check_infinity(tf1) & !(tf1&(1<<7)))//tf1 == +infinity
    {
        if(check_infinity(tf2) & (tf2&(1<<7)))//tf2 == -infinity
            return 1<<6 | 1<<5 | 1<<4 | 1<<3| 1<<2;//return NaN
        else
            return tf1;//return +infinity
    }
    
    if(check_infinity(tf1) & (tf1&(1<<7)))//tf1 == -infinity
    {
        if(check_infinity(tf2) & (!(tf2&(1<<7))))//tf2 == +infinity
            return 1<<6 | 1<<5 | 1<<4 | 1<<3| 1<<2;//return NaN
        else
            return tf1;
    }
    if(check_NaN(tf1)) return tf1;
    if(check_NaN(tf2)) return tf2;
    //general cases
    //2 cases: addition(same sign bit) or subtraction(different sign bit)
    
    //get exponents from binary to decimal
    tinyfp ret=0;
    int exp=0,exp1=0,exp2=0;
    int pow_two=1;
    for(int i=3;i<=6;i++)
    {
        if(tf1 & (1<<i))
        {
            
        }
    }
 
	return 9;
}

tinyfp mul(tinyfp tf1, tinyfp tf2){
	return 9;
}

int gt(tinyfp tf1, tinyfp tf2){
	return 9;
}

int eq(tinyfp tf1, tinyfp tf2)
{
    //special cases
    if(check_NaN(tf1) | check_NaN(tf2))  return 0;
    if(check_zero(tf1) & check_zero(tf2)) return 1;
    if(check_infinity(tf1) & check_infinity(tf2))
    {
        if((tf1&(1<<7)) == (tf2&(1<<7)))
            return 1;
        return 0;
    }
    
    //general cases
    for(int i=7;i>=0;i--)
    {
        if((tf1&(1<<i)) != (tf2&(1<<i)))
            return 0;
    }
	return 1;
}

int check_infinity(tinyfp tf)
{
    //if infinity, return 1
    //else return 0
    for(int i=6;i>=3;i--)
    {
        if(!(tf&(1<<i)))
            return 0;
    }
    for(int i=2;i>=0;i--)
    {
        if((tf&(1<<i)))
            return 0;
    }
    return 1;
}
int check_NaN(tinyfp tf)
{
    //if infinity, return 1
    //else return 0
    for(int i=6;i>=3;i--)
    {
        if(!(tf&(1<<i)))
            return 0;
    }
    for(int i=2;i>=0;i--)
    {
        if((tf&(1<<i)))//there's at least one 1 in fracs
            return 1;
    }
    return 0;//frac's are all zero
}

int check_zero(tinyfp tf)
{
    //if infinity, return 1
    //else return 0
    for(int i=6;i>=0;i--)
    {
        if((tf&(1<<i)))
            return 0;
    }
    return 1;
}
