#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
typedef long long ll;

ll modExp(ll base, ll exp, ll mod) {
    ll result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) { 
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

ll modInverse(ll a, ll mod) {
    ll m0 = mod, t, q;
    ll x0 = 0, x1 = 1;

    if (mod == 1) return 0;

    while (a > 1) {
        q = a / mod;
        t = mod;
        mod = a % mod, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

void generateKeys(ll &p, ll &g, ll &x, ll &y) {
    srand(time(0));

    p = 23;  
    g = 5; 
    x = rand() % (p - 2) + 1; 
    y = modExp(g, x, p); 

    cout << "Public Key: (p=" << p << ", g=" << g << ", y=" << y << ")\n";
    cout << "Private Key: (x=" << x << ")\n";
}

void encrypt(ll M, ll p, ll g, ll y, ll &c1, ll &c2) {
    ll k = rand() % (p - 2) + 1;
    c1 = modExp(g, k, p); 
    c2 = (M * modExp(y, k, p)) % p; 
}

ll decrypt(ll c1, ll c2, ll p, ll x) {
    ll s = modExp(c1, x, p); 
    ll s_inv = modInverse(s, p); 
    return (c2 * s_inv) % p; 
}

int main() {
    ll p, g, x, y; 
    generateKeys(p, g, x, y); 

    ll M; 
    cout << "Enter a message (integer): ";
    cin >> M;

    ll c1, c2;
    encrypt(M, p, g, y, c1, c2);
    cout << "Ciphertext: (" << c1 << ", " << c2 << ")\n";

    ll decryptedM = decrypt(c1, c2, p, x);
    cout << "Decrypted Message: " << decryptedM << endl;

    return 0;
}
