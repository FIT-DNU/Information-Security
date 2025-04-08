#include<iostream>
#include<vector>
#include<map>
#include<cstdlib>
#include<ctime>
using namespace std;

// Alphabet array
char alpha[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m',
                  'n','o','p','q','r','s','t','u','v','w','x','y','z'};

// Generate random key
string generateKey(int n) {
    string key = "";
    for (int i = 0; i < n; i++) {
        key += alpha[rand() % 26];
    }
    return key;
}

int main() {
    srand(time(0)); // seed random number generator
    string plainTxt, key, cipherTxt = "";
    map<char, int> alphaCode;
    vector<int> addedCode;

    // Build alphabet map
    for (int i = 0; i < 26; i++) {
        alphaCode[alpha[i]] = i;
    }

    // Input
    cout << "Enter plain text (lowercase, no space): ";
    getline(cin, plainTxt);

    // Generate key
    key = generateKey(plainTxt.size());
    cout << "Generated Key: " << key << endl;

    // Convert to codes and add
    for (int i = 0; i < plainTxt.size(); i++) {
        int sum = alphaCode[plainTxt[i]] + alphaCode[key[i]];
        if (sum >= 26) sum -= 26;
        addedCode.push_back(sum);
    }

    // Convert numeric code to cipher text
    for (int code : addedCode) {
        cipherTxt += alpha[code];
    }

    // Output
    cout << "PLAIN TEXT  : " << plainTxt << endl;
    cout << "CIPHER TEXT : " << cipherTxt << endl;

    return 0;
}
