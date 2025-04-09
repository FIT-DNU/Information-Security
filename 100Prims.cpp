#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

// Hàm kiểm tra số nguyên tố
bool isPrime(int n) {
    if (n <= 1)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;

    int limit = sqrt(n);
    for (int i = 3; i <= limit; i += 2) {
        if (n % i == 0)
            return false;
    }
    return true;
}

int main() {
    vector<int> primes;
    int num = 100000;

    // Tìm 100 số nguyên tố có 6 chữ số
    while (primes.size() < 100) {
        if (isPrime(num)) {
            primes.push_back(num);
        }
        num++;
    }

    // In ra danh sách
    for (int p : primes) {
        cout << p << endl;
    }

    return 0;
}
