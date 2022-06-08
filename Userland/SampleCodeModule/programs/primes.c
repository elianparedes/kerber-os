#include <kstdio.h>

/*
* Prints prime numbers
*/

int isprime(unsigned int n);

void primes(){
    unsigned int n=1;
    while (1){
        if (isprime(n)){
            printf("%d ", n);
        }
        n++;
    }
}

int isprime(unsigned int n){
    if (n == 0 || n==1){
        return 0;
    }
    if (n == 2){
        return 1;
    }
    if (n % 2 == 0){
        return 0;
    }
    for (int divisor = 3; divisor*divisor <= n; divisor +=2){
        if (n % divisor == 0){
            return 0;
        }
    }
    return 1;
}