#include <bits/stdc++.h>

using namespace std;
set<int>
    prime; // a set will be the collection of prime numbers,
           // where we can select random primes p and q
int public_key;
int private_key;
int n;
// we will run the function only once to fill the set of
// prime numbers
void primefiller()
{
    // method used to fill the primes set is seive of
    // eratosthenes(a method to collect prime numbers)
    vector<bool> seive(250, true);
    seive[0] = false;
    seive[1] = false;
    for (int i = 2; i < 250; i++) {
        for (int j = i * 2; j < 250; j += i) {
            seive[j] = false;
        }
    } // filling the prime numbers
    for (int i = 0; i < seive.size(); i++) {
        if (seive[i])
            prime.insert(i);
    }
}
// picking a random prime number and erasing that prime
// number from list because p!=q
int pickrandomprime()
{
    int k = rand() % prime.size();
    auto it = prime.begin();
    while (k--)
        it++;
    int ret = *it;
    prime.erase(it);
    return ret;
}
void setkeys()
{
    int prime1 = pickrandomprime(); // first prime number
    int prime2 = pickrandomprime(); // second prime number
    // to check the prime numbers selected
    // cout<<prime1<<" "<<prime2<<endl;
    n = prime1 * prime2;
    int fi = (prime1 - 1) * (prime2 - 1);
    int e = 2;
    while (1) {
        if (__gcd(e, fi) == 1)
            break;
        e++;
    } // d = (k*F(n) + 1) / e for some integer k
    public_key = e;
    int d = 2;
    while (1) {
        if ((d * e) % fi == 1)
            break;
        d++;
    }
    private_key = d;
}
// to encrypt the given number
long long int encrypt(double message)
{
    int e = public_key;
    long long int encrpyted_text = 1;
    while (e--) {
        encrpyted_text *= message;
        encrpyted_text %= n;
    }
    return encrpyted_text;
}
// to decrypt the given number
long long int decrypt(int encrpyted_text)
{
    int d = private_key;
    long long int decrypted = 1;
    while (d--) {
        decrypted *= encrpyted_text;
        decrypted %= n;
    }
    return decrypted;
}
// first converting each character to its ASCII value and
// then encoding it then decoding the number to get the
// ASCII and converting it to character
vector<int> encoder(string message)
{
    vector<int> form;
    // calling the encrypting function in encoding function
    for (auto& letter : message)
        form.push_back(encrypt((int)letter));
    return form;
}
string decoder(vector<int> encoded)
{
    string s;
    // calling the decrypting function decoding function
    for (auto& num : encoded)
        s += decrypt(num);
    return s;
}
int main()
{
    primefiller();
    setkeys();
    string message = "KHOA CNTT";
    // uncomment below for manual input
    // cout<<"enter the message\n";getline(cin,message);
    // calling the encoding function
    vector<int> coded = encoder(message);
    cout << "Initial message:\n" << message;
    cout << "\n\nThe encoded message(encrypted by public "
            "key)\n";
    for (auto& p : coded)
        cout << p;
    cout << "\n\nThe decoded message(decrypted by private "
            "key)\n";
    cout << decoder(coded) << endl;
    return 0;
}
