#include<iostream>

using namespace std;

int nzd(int n, int m) {

    if (m == 0) {
        return n;
    } else if(n<m){
        return nzd(m, n);
    }else {

    } {
        return nzd(m, n % m);
    }
}
int fib2_0(int n, int a = 0, int b = 1) {
    if (n == 0) {
        return a;
    } else if (n == 1) {
        return b;
    } else {
        return fib2_0(n - 1, b, a + b);
    }
}

int fib(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fib2_0(n - 1);
    }
}

int main(){

    return 0;
}