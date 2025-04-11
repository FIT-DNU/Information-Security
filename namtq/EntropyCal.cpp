#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double log2_custom(double x) {
    return log(x) / log(2);
}

bool isTwoDecimalPlaces(double x) {
    int temp = static_cast<int>(x * 100);
    return (x * 100 == temp);
}

int main() {
    const int MAX = 100;
    double probs[MAX];
    int n;
    double sum = 0.0;

    cout << "Nhap so ky tu: ";
    cin >> n;

    if (n <= 0 || n > MAX) {
        cout << "So ky tu khong hop le.\n";
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        cout << "Nhap xac suat cua ky tu thu " << i + 1 << ": ";
        cin >> probs[i];

        if (probs[i] < 0.0 || probs[i] > 1.0 || !isTwoDecimalPlaces(probs[i])) {
            cout << "Xac suat khong hop le (chi cho phep den 2 chu so thap phan).\n";
            return 1;
        }
        sum += probs[i];
    }

    if (float(sum) != float(1.0)) {
        cout << "Tong xac suat khong bang 1.\n";
        return 1;
    }

    double entropy = 0.0;
    for (int i = 0; i < n; ++i) {
        if (probs[i] > 0.0)
            entropy -= probs[i] * log2_custom(probs[i]);
    }

    cout << fixed << setprecision(6);
    cout << "Entropy cua nguon thong tin la: " << entropy << " bits\n";

    return 0;
}

