// C++ program to demonstrate working of 
// extended Euclidean Algorithm 
#include <bits/stdc++.h> 
using namespace std;

// Function to return
// gcd of a and b
int findGCD(int a, int b) {
    if (a == 0)
        return b;
    return findGCD(b % a, a);
}

int main()  { 
    int a = 35, b = 15;
    int g = findGCD(a, b); 
    cout << g << endl;
    return 0; 
}
