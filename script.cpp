#include <iostream>
#include <algorithm>

using namespace std;

int mod_pow(int base, int exponent, int mod) {
    int result = 1;
    base = base % mod;  // Aggiorna base se è maggiore o uguale a mod
    while (exponent > 0) {
        if (exponent % 2 == 1)  // Se l'esponente è dispari
            result = (result * base) % mod;
        exponent = exponent >> 1;  // exponent = exponent / 2
        base = (base * base) % mod;
    }
    return result;
}

int main() {
    int N = 101;
    int y = 65;
    int z = N - 1;
    int w = 0;

    // Trova z e w tali che N-1 = z * 2^w con z dispari
    while (z % 2 == 0) {
        z /= 2;
        w += 1;
    }

    cout << "z=" << z << endl;

    // Calcola il MCD tra N e y
    bool P1 = __gcd(N, y) == 1;
    cout << "P1: " << boolalpha << P1 << endl;

    // Calcola y^z mod N
    bool P2 = mod_pow(y, z, N) == 1;
    cout << "P2: " << boolalpha << P2 << endl;

    bool P3 = false;
    cout << "w=" << w << endl;
    for (int i = 0; i < w; ++i) {
        int exponent_i = (1 << i) * z;
        int a = mod_pow(y, exponent_i, N);
        cout << "boh " << a << endl;
        if (a == N - 1) {
            P3 = true;
        }
    }
    cout << "P3: " << boolalpha << P3 << endl;

    return 0;
}
