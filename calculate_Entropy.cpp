// entropy.cpp
#include <iostream>
#include <map>
#include <cmath>
#include <string>
using namespace std;
// calculate entropy
double calculate_entropy(const string& text) {
map<char, int> freq;
for (char c : text) freq[c]++;
double entropy = 0.0;
for (auto& pair : freq) {
double p = (double)pair.second / text.size();
entropy -= p * log2(p);
}
return entropy;
}
int main() {
string input;
cout << "Enter a string of characters: ";
getline(cin, input);
double H = calculate_entropy(input);
cout << "Entropy: " << H << endl;
return 0;
}
