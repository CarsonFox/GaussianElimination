#pragma once

//#define P 982451653
#define P 11

long mAdd(long a, long b)
{
    return (a + b) % P;
}

long mSub(long a, long b)
{
    return (a + P - b) % P;
}

long mMul(long a, long b)
{
    return (a * b) % P;
}

long mInv(long a)
{
    long r = P;
    long old_r = a;
    long s = 0;
    long old_s = 1;
    while (r != 0) {
        long q = old_r / r;
        long r0 = r;
        r = old_r - q * r;
        old_r = r0;
        long s0 = s;
        s = old_s - q * s;
        old_s = s0;
    }
    return old_s >= 0 ? old_s : old_s + P;
}

long mDiv(long a, long b)
{
    return mMul(a, mInv(b));
}
