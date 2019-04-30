#pragma once

#include <iostream>

#define P 982451653

struct ModInt {
public:
    ModInt() = default;

    explicit ModInt(long x) {
        value = x % P;
    }

    ModInt &operator=(const ModInt &other) = default;

    ModInt operator+(const ModInt &other) const {
        return ModInt(mAdd(this->value, other.value));
    }

    ModInt &operator+=(const ModInt &other) {
        this->value = mAdd(this->value, other.value);
        return *this;
    }

    ModInt operator-(const ModInt &other) const {
        return ModInt(mSub(this->value, other.value));
    }

    ModInt &operator-=(const ModInt &other) {
        this->value = mSub(this->value, other.value);
        return *this;
    }

    ModInt operator*(const ModInt &other) const {
        return ModInt(mMul(this->value, other.value));
    }

    ModInt &operator*=(const ModInt &other) {
        this->value = mMul(this->value, other.value);
        return *this;
    }

    ModInt operator/(const ModInt &other) const {
        return ModInt(mDiv(this->value, other.value));
    }

    ModInt &operator/=(const ModInt &other) {
        this->value = mDiv(this->value, other.value);
        return *this;
    }

    bool operator==(ModInt rhs) {
        return this->value == rhs.value;
    }

    bool operator==(long rhs) {
        return this->value == rhs;
    }

    bool operator!=(ModInt rhs) {
        return this->value != rhs.value;
    }

    bool operator!=(long rhs) {
        return this->value != rhs;
    }

    friend std::ostream &operator<<(std::ostream &os, const ModInt &x) {
        os << x.value;
        return os;
    }

private:
    long value;

    static long mAdd(long a, long b) {
        return (a + b) % P;
    }

    static long mSub(long a, long b) {
        return (a + P - b) % P;
    }

    static long mMul(long a, long b) {
        return (a * b) % P;
    }

    static long mInv(long a) {
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

    static long mDiv(long a, long b) {
        return mMul(a, mInv(b));
    }
};
