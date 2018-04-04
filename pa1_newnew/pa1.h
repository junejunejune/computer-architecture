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


#ifndef _PA_1_H_
#define _PA_1_H_

typedef unsigned char tinyfp;

// Convert int to tinyfp
tinyfp int2tinyfp(int x);

// Convert tinyfp to int
int tinyfp2int(tinyfp x);

// Convert float to tinyfp
tinyfp float2tinyfp(float x);

// Convert tinyfp to float
float tinyfp2float(tinyfp x);

#endif // _PA_1_H_
