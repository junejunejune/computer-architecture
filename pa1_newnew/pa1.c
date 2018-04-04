//---------------------------------------------------------
// 
// Project #1: TinyFP Representation
//
// March 22, 2018
//
// Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
// Systems Software Laboratory
// Dept. of Computer Science and Engineering
// Seoul National University
//
//---------------------------------------------------------


#include "pa1.h"
#include "stdio.h"
typedef unsigned char tinyfp;
int TMin= -2147483648;
tinyfp int2tinyfp(int x)//1
{
    int final_num[8]={0};

    //special cases
    if(x==0)    return 0x00;
    else if(x>240)//+infinity,480??
        return ((1<<3) | (1<<2) | (1<<1) |(1))<<3;
    else if(x<-240)//-infinity
        return  ((1<<4)|(1<<3) | (1<<2) | (1<<1) |(1))<<3;

    //conversion
    if(x<0)//get rid of negative sign
    {
        final_num[0]=1;
        x*=-1;
    }
    
    int binary[32]={0};//make decimal to binary
    int i,k;
    for(i=31;i>=0;i--)
    {
        k=x>>i;
        if(k & 1)
            binary[31-i]=1;
        else
            binary[31-i]=0;
    }
    int first_one=0;//find first one in binary number
    for(i=0;i<32;i++)
    {
        if(binary[i]==1)
        {
            //printf("is one %d: \n",i);
            first_one=i;
            break;
        }
    }
    //mantissa
    final_num[5]=binary[first_one+1];
    final_num[6]=binary[first_one+2];
    final_num[7]=binary[first_one+3];
    int e=31-first_one+7;//get exponent to binary

    for(i=3;i>=0;i--)
    {
        k=e>>i;
        if(k & 1)
            final_num[4-i]=1;
        else
            final_num[4-i]=0;
    }

    tinyfp ret=0;
    for(i=0;i<8;i++)
    {
        if(final_num[i]==1)
        ret+=(1<<(7-i));
    }
    return ret;
}


int tinyfp2int(tinyfp x)//2
{
    if(x==0 || x==(1<<7))   return 0;
    if((x&(1<<6)) && (x&(1<<5)) && (x&(1<<4)) && (x&(1<<3)))    return TMin;

    if(!(x&(1<<6)) && (!(x&(1<<5)) && (x&(1<<4)) && (x&(1<<3)) ))//smaller than 1
        return 0;

    int neg=0;
    if(x&(1<<7))
        neg=1;

    if((x&(1<<5)) && (x&(1<<4)) && (x&(1<<3)))//1
    {
        if(neg==1)  return -1;
        else    return 1;
    }
    
    //turn exp binary to decimal
    int exp=0;
    int pow_2=1;
    for(int i=3;i<=6;i++)
    {
        if(x&(1<<i))
            exp+=pow_2;
        pow_2*=2;
    }
    exp-=7;
    if(exp<0)   return 0;
    
    int ret=1<<exp;
    
    for(int i=0;i<=2;i++)
    {
        if(x&(1<<i) && (exp+i-3)>=0)
            ret+=1<<(exp+i-3);
    }
    if(neg==1)
        ret*=-1;
    return ret;
}


tinyfp float2tinyfp(float x)//3
{
    int *b=(int *)&x;
    tinyfp ret=0;

    //sign bit copy
    if((*b)&(1<<31))
        ret+=(1<<7);

    //denormalized
    if(x<(float)8/512 && x>-(float)8/512)
    {
        if((x<(float)1/512) && (x>(float)1/512) )
            return ret;
        else if( ((x>=(float)1/512) && (x<(float)2/512))|| ((x<=-(float)1/512)&&((x>-(float)2/512)) ))
            ret+=1;
        else if( ((x>=(float)2/512) && (x<(float)3/512))|| ((x<=-(float)2/512)&&((x>-(float)3/512)) ))
            ret+=1<<1;
        else if( ((x>=(float)3/512) && (x<(float)4/512))|| ((x<=-(float)3/512)&&((x>-(float)4/512)) ))
            ret+=(1<<1)|(1);
        else if( ((x>=(float)4/512) && (x<(float)5/512))|| ((x<=-(float)4/512)&&((x>-(float)5/512)) ))
            ret+=1<<2;
        else if( ((x>=(float)5/512) && (x<(float)6/512))|| ((x<=-(float)5/512)&&((x>-(float)6/512)) ))
            ret+=(1<<2)|1;
        else if( ((x>=(float)6/512) && (x<(float)7/512))|| ((x<=-(float)6/512)&&((x>-(float)7/512)) ))
            ret+=(1<<2)|(1<<1);
        else if( ((x>=(float)7/512) && (x<(float)8/512))|| ((x<=-(float)7/512)&&((x>-(float)8/512)) ))
            ret+=(1<<2)|(1<<1)|1;
        return ret;
    }
    
    //check special cases
    int flag=1,flag2=1;
    for(int i=23;i<=30;i++)
    {
        if(!((*b)&(1<<i)))
            flag=0;
    }
    if(flag==1)
    {
        //set all exp's to 1
        ret+=(1<<6)|(1<<5)|(1<<4)|(1<<3);
        //and then check bits of frac
        for(int i=0;i<=22;i++)
        {
            if(((*b)&(1<<i)))//if it is one
                flag2=0;
        }
        if(flag2==1)//all bits are zero=>infinity
            return ret;
        else//NaN
        {
            ret+=(1<<1);
            return ret;
        }
    }

    int exp=0;
    int two_23=1;
    for(int i=23;i<=30;i++)
    {
        if((*b)&(1<<i))
            exp+=two_23;
        two_23*=2;
    }
    exp -= 120;
    if(x>240 ||x<-240)//infinity
    {
        ret+=(1<<6)|(1<<5)|(1<<4)|(1<<3);
        return ret;
    }
    else//fits=>convert
    {
        //copy decimal
        for(int i=20;i<=22;i++)
            if((*b)&(1<<i))
                ret+=(1<<(i-20));

        //change exponent to binary
        for(int i=3;i>=0;i--)
        {
            if((exp>>i)&1)
                ret+=1<<(i+3);
        }
        return ret;
    }
}


float tinyfp2float(tinyfp x)//4
{
    float f=0;
    int *b=(int *)&f;
    int ret[32]={0};
    //sign bit
    if(x&(1<<7))
        *b = *b |(1<<(31));
    
    //exp
    int exp=0;
    if((x&(1<<3)) && (x&(1<<4)) && (x&(1<<5)) && (x&(1<<6)))//if 1111
    {
        for(int i=7;i>=0;i--)
            ret[23+i]=1;
    }
    else if(!(!(x&(1<<3)) && (!(x&(1<<4))) && (!(x&(1<<5))) && (!(x&(1<<6)))))//if not 0000
    {
        int pow_2=1;
        for(int i=3;i<=6;i++)
        {
            if(x&(1<<i))
                exp+=pow_2;
            pow_2*=2;
        }
        exp+=120;
        //printf("exp: %d ",exp);
        for(int i=7;i>=0;i--)
           if((exp>>i)&1)
               ret[23+i]=1;
    }
    if((!(x&(1<<3)) && !(x&(1<<4))) && (!(x&(1<<5))) && (!(x&(1<<6))))//if 0000
    {
        exp=120;
        if(!(x&(1<<2)) && (!(x&(1<<1))) && (!(x&(1<<0))))//if 0000000=>then 0
        {
            *b = *b | (ret[31])<<31;//copy sign bit
            return f;
        }
        else//numbers very close to 0.0,was denor
        {
            //find first one in frac, since it's denor,
            //it must be converted to nor
            float tmp;
            if((x&(1)) &&!(x&(1<<2)) && !(x&(1<<1)))//001
                tmp=(float)1/512;
            else if(!(x&(1)) &&!(x&(1<<2)) && (x&(1<<1)))//010
                tmp= (float)2/512;
            else if(!(x&(1<<2))&&(x&(1<<1))&&(x&1))//011
                tmp= (float)3/512;
            else if(x&(1<<2)&&!(x&(1<<1))&&!(x&1))//100
                tmp= (float)4/512;
            else if((x&(1<<2))&&!(x&(1<<1))&&(x&1))//101
                tmp= (float)5/512;
            else if((x&(1<<2))&&(x&(1<<1))&&!(x&1))//110
                tmp= (float)6/512;
            else if ( (x&(1<<2)) && (x&(1<<1)) && (x&1) )//111
                tmp= (float)7/512;

            if(x&(1<<7))
                return tmp*(-1);
                else return tmp;
        }
    }

    //frac
    for(int i=0;i<=2;i++)
        if(x&(1<<i))
            ret[20+i]=1;

    for(int i=0;i<32;i++)
        if(ret[i]==1)
            *b = *b |(1<<(i));
    return f;
}
