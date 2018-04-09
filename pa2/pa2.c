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

typedef unsigned char tinyfp;
int check_infinity(tinyfp tf);
int check_NaN(tinyfp tf);
int check_zero(tinyfp tf);

tinyfp add(tinyfp tf1, tinyfp tf2)
{
    //special cases
    if(check_NaN(tf1)) return tf1;
    if(check_NaN(tf2)) return tf2;
    if(check_infinity(tf1) & (!(tf1&(1<<7))))//tf1 == +infinity
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
    if(check_infinity(tf2))
        return tf2;
    
    //general cases
    if(check_zero(tf1))//0+x=x
        return tf2;
    if(check_zero(tf2))
        return tf1;
    
    //2 cases: addition(same sign bit) or subtraction(different sign bit)
    //get exponents from binary to decimal
    int exp=0,exp1=0,exp2=0;
    int pow_two=1;
    for(int i=3;i<=6;i++)
    {
        if(tf1 & (1<<i))
            exp1 += pow_two;
        if(tf2 & (1<<i))
            exp2 += pow_two;
        pow_two *= 2;
    }
    
    int frac=0,frac1=0,frac2=0;
    frac1=tf1&((1<<2)|(1<<1)|1);//xxx
    frac2=tf2&((1<<2)|(1<<1)|1);
    
    //normalized values=>add 1
    if(exp1!=0)
        frac1+=(1<<3);//1xxx
    if(exp2!=0)
        frac2+=(1<<3);
    
    if(tf1&(1<<7))  frac1*=-1;//to do subtraction just by adding two fractions
    if(tf2&(1<<7))  frac2*=-1;

    int exp_diff=0;
    if(exp1==exp2)
    {
        exp=exp1;
        frac=frac1+frac2;
        //carry always exists on 5th bit, so get rid of it and shift right
        exp++;
        if(exp>=15)//overflow
        {
            if((tf1&(1<<7)) == (tf2&(1<<7)))//same sign bit
                return 1<<6 | 1<<5 | 1<<4 | 1<<3; //return +infinity
            else
                return 1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3; //return -infinity
        }
        frac=(frac>>1)&((1<<2)|(1<<1)|1);
    }
    else if(exp1>exp2)
    {
        exp=exp1;
        exp_diff=exp1-exp2;
        frac2=frac2>>exp_diff;
        frac=frac1+frac2;
        //find first 1, and take bits(total 3) after it
        if(frac>=(1<<4))
        {
            exp++;
            frac=(frac>>1)&((1<<2)|(1<<1)|1);
        }
        else if(frac>=(1<<3))
        {
            frac=frac&((1<<2)|(1<<1)|1);
        }
        else if(frac>=(1<<2))
        {
            frac=frac&((1<<1)|1);
        }
        else if(frac>=(1<<1))
        {
            frac=frac&1;
        }
    }
    else
    {
        exp=exp2;
        exp_diff=exp2-exp1;
        frac1=frac1>>exp_diff;
        frac=frac1+frac2;
        if(frac>=(1<<4))
        {
            exp++;
            frac=(frac>>1)&((1<<2)|(1<<1)|1);
        }
        else if(frac>=(1<<3))
        {
            frac=frac&((1<<2)|(1<<1)|1);
        }
        else if(frac>=(1<<2))
        {
            frac=frac&((1<<1)|1);
        }
        else if(frac>=(1<<1))
        {
            frac=frac&1;
        }
    }
    tinyfp ret=0;
    //convert exp from decimal to binary
    int k;
    for(int i=3;i>=0;i--)
    {
        k=exp>>i;
        if(k & 1)
            ret += (1<<(i+3));
    }
    
    if(frac<0)//sign bit
    {
        ret+=(1<<7);
        frac*=-1;
    }
    ret+=frac;
	return ret;
}

tinyfp mul(tinyfp tf1, tinyfp tf2)
{
    //special cases
    if(check_NaN(tf1)) return tf1;
    if(check_NaN(tf2)) return tf2;
    if((check_infinity(tf1) & check_zero(tf2)) | (check_infinity(tf2) & check_zero(tf1)))
    {
        return 1<<6 | 1<<5 | 1<<4 | 1<<3 | 1<<2;//return NaN
    }
    if(check_infinity(tf1) | check_infinity(tf2))
    {
        if((tf1&(1<<7)) == (tf2&(1<<7)))//same sign bit infinity
            return 1<<6 | 1<<5 | 1<<4 | 1<<3; //return +infinity
        else
            return 1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3; //return -infinity
    }
    
    //general cases
    if(check_zero(tf1) | check_zero(tf2))
    {
        if((tf1&(1<<7)) != (tf2&(1<<7)))
            return 1<<7;
        return 0x00;
    }
    
    int exp=0,exp1=0,exp2=0;
    int frac=0,frac1=0,frac2=0;
    int pow_two=1;
    for(int i=3;i<=6;i++)
    {
        if(tf1 & (1<<i))
            exp1 += pow_two;
        if(tf2 & (1<<i))
            exp2 += pow_two;
        pow_two *= 2;
    }
    exp=exp1+exp2-7;
    
    //copy fractional bits
    frac1=tf1&((1<<2)|(1<<1)|1);//xxx
    frac2=tf2&((1<<2)|(1<<1)|1);
    
    //add one to normalized values
    if(exp1 != 0)
        frac1+=(1<<3);//1xxx
    if(exp2 != 0)
    frac2+=(1<<3);
    
    frac=frac1*frac2;
    
    if(frac>=(1<<7))//highest bit(7th bit) is set
    {
        int temp=!(1<<7);
        frac=frac & (~(1<<7));//turn the highest bit off
        frac= frac>>4;
        exp++;
    }
    else if(frac>=(1<<6))
    {
        frac=frac & (~(1<<6));
        frac= frac>>3;
    }
    
    if(exp>=15)//overflow
    {
        if((tf1&(1<<7)) == (tf2&(1<<7)))//same sign bit
            return 1<<6 | 1<<5 | 1<<4 | 1<<3; //return +infinity
        else
            return 1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3; //return -infinity
    }
    tinyfp ret=0;
    exp=exp<<3;
    ret+=exp+frac;
    if((tf1&(1<<7)) != (tf2&(1<<7)))//handle sign bit
        ret+=(1<<7);
	return ret;
}

int gt(tinyfp tf1, tinyfp tf2)
{
    //special cases
    if(check_NaN(tf1) | check_NaN(tf2))  return 0;
    if(check_zero(tf1) & check_zero(tf2)) return 0;
    if(check_infinity(tf1) & !(tf1&(1<<7)))//tf1 is +infinity
    {
        if(check_infinity(tf2) & !(tf2&(1<<7)))//tf2 is +infinity
            return 0;
        return 1;
    }
    if(check_infinity(tf1) & (tf1&(1<<7)))//tf1 is -infinity
        return 0;
    if(check_infinity(tf2))
    {
        if(tf2&(1<<7))//tf2 is -infinity
           return 1;
        else
           return 0;
    }
    
    //general cases
    if((!(tf1&(1<<7))) & (tf2&(1<<7)))    return 1;//tf1:+, tf2:-
    else if((tf1&(1<<7)) & (!(tf2&(1<<7))))   return 0;//tf1:-, tf2:+
    else//same sign
    {
        //compare exponents first
        int exp1=0,exp2=0;
        int pow_two=1;
        for(int i=3;i<=6;i++)
        {
            if(tf1 & (1<<i))
                exp1 += pow_two;
            if(tf2 & (1<<i))
                exp2 += pow_two;
            pow_two *= 2;
        }
        
        //be careful of negative nums!!!
        if(exp1>exp2)
        {
            if(tf1&(1<<7))//both are negative numbers
                return 0;
            else//both are positive numbers
                return 1;
        }
        else if(exp1<exp2)
        {
            if(tf1&(1<<7))//both are negative numbers
                return 1;
            else//both are positive numbers
                return 0;
        }
        else//same exponents
        {
            
            int frac1=0,frac2=0;
            pow_two=1;
            for(int i=0;i<=2;i++)
            {
                if(tf1 & (1<<i))
                    frac1 += pow_two;
                if(tf2 & (1<<i))
                    frac2 += pow_two;
                pow_two *= 2;
                //printf("frac1: %d, frac2: %d\n", frac1, frac2);
            }
            
            if(tf1&(1<<7))//both are negative numbers
            {
                if(frac1>=frac2) return 0;
                else    return 1;
            }
            else//both are positive numbers
            {
                //printf("frac1: %d, frac2: %d\n", frac1, frac2);
                if(frac1>frac2) return 1;
                else    return 0;
            }
        }
    }
	return 0;
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
    }//all exps are 1
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
    }//all exps are 1
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
