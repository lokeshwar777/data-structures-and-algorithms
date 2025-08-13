#include <iostream>
using namespace std;

// Counts the number of one’s(set bits) in an integer(long/long long).
int getSetBits(uint32_t x) {
    return __builtin_popcount(x);
}

// Checks the Parity of a number. number has odd parity(odd number of set bits)?
int hasOddSetBits(uint32_t x) {
    return __builtin_parity(x);
}

// Counts the leading (left) number of zeros of the integer
int getLeadingZerosCount(uint32_t x) {
    return __builtin_clz(x);
    // return __builtin_clzll(x); // for long long
}

// Counts the trailing(right) number of zeros of the integer
int getTrailingZerosCount(uint32_t x) {
    return __builtin_ctz(x);
    // return __builtin_ctzll(x); // for long long
}

// highest power of 2 less than or equal to x
int highestPowerOf2(uint32_t x) {
    return 31 - __builtin_clz(x);
}

// isolate leftmost set bit / highest set bit / Most Significant Bit
int msb(int n) {
    return 1 << highestPowerOf2(n);
}

// isolate rightmost set bit / lowest set bit / Least Significant Bit
int lsb(int n) {
    return n == 0 ? 0 : n & -n;
}

int main() {
    // cout << highestPowerOf2(19) << endl;
    return 0;
}

/*************************************************************************/
// <----- leading zeroes ---->|     <--- trailing zeroes --->
// 00000000000000000000000000010011
// u-unsigned

// highest power of 2 less than or equal to x
int highestPowerOf2(int x) {
    // return 32 - getLeadingZerosCount(x) - 1;  // 0-based
    return 31 - getLeadingZerosCount(x);  // 0-based

    // return 32 - getLeadingZerosCount(x); // 1-based
}