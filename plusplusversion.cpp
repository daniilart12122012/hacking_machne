#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

const vector<int> S_BOX = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b,
    0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,

    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1,
    0x71, 0xd8, 0x31, 0x15, 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,

    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3,
    0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,

    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f,
    0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,

    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d,
    0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,

    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62,
    0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,

    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f,
    0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,

    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9,
    0xce, 0x55, 0x28, 0xdf, 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

vector<vector<int>> S_box(const vector<vector<int>>& state) {
    vector<vector<int>> new_state;
    for (const auto& row : state) {
        vector<int> new_row;
        for (int byte : row) new_row.push_back(S_BOX[byte]);
        new_state.push_back(new_row);
    }
    return new_state;
}

vector<vector<int>> shift_rows(const vector<vector<int>>& state) {
    vector<vector<int>> new_state;
    for (int i = 0; i < 4; ++i) {
        vector<int> shifted_row;
        for (int j = 0; j < 4; ++j) {
            shifted_row.push_back(state[i][(j + i) % 4]);
        }
        new_state.push_back(shifted_row);
    }
    return new_state;
}

vector<vector<int>> shift_cols(const vector<vector<int>>& state) {
    vector<vector<int>> new_state;
    for (const auto& row : state) {
        new_state.push_back({row[1], row[0], row[3], row[2]});
    }
    return new_state;
}

vector<vector<int>> xor_state(const vector<vector<int>>& state,
                              const vector<vector<int>>& key) {
    vector<vector<int>> new_state;
    for (size_t i = 0; i < state.size(); ++i) {
        vector<int> new_row;
        for (size_t j = 0; j < state[i].size(); ++j) {
            new_row.push_back(state[i][j] ^ key[i][j]);
        }
        new_state.push_back(new_row);
    }
    return new_state;
}

void final_fn(const vector<vector<int>>& state, const vector<vector<int>>& r_k) {
    auto f_res   = S_box(state);
    auto s_res   = shift_rows(f_res);
    auto th_res  = shift_cols(s_res);
    auto fth_res = xor_state(th_res, r_k);

    cout << "Result is:" << endl;
    for (const auto& row : fth_res) {
        for (int byte : row) {
            cout << "0x" << hex << byte << " ";
        }
        cout << endl;
    }
    cout << dec;
}

long long gcd_ll(long long a, long long b) {
    while (b) { a %= b; swap(a, b); }
    return a;
}

void rsa(long long p, long long q) {
    long long n = p * q;
    long long phi_n = (p - 1) * (q - 1);

    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<long long> dist(2, phi_n - 1);

    long long e = 0;
    while (gcd_ll(e, phi_n) != 1) e = dist(gen);

    long long d = 0;
    while ((e * d) % phi_n != 1) ++d;

    cout << "Public key:" << endl;
    cout << "[" << e << ", " << n << "]" << endl;
    cout << "Private key:" << endl;
    cout << "[" << d << ", " << n << "]" << endl;
}

string pbox(const vector<int>& key, const string& text) {
    string new_line;
    int cnt = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        if (cnt >= (int)key.size()) cnt = 0;
        new_line += text[key[cnt] - 1];
        ++cnt;
    }
    return new_line;
}

int main() {
    cout << "Hello! You are using hacking_machine." << endl;
    cout << "Please, choose a function:" << endl;
    cout << "AES, P_box, S_box, XOR, ROT, RSA_key_gen" << endl;

    string a;
    while (a != "exit") {
        if (!(cin >> a)) break;

        if (a == "AES") {
            cout << "Now you have to enter your state to encode." << endl;
            cout << "There will be four lines with four decimal bytes in each." << endl;
            vector<vector<int>> state(4, vector<int>(4));
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j) cin >> state[i][j];

            cout << "Then enter your key using the same format" << endl;
            vector<vector<int>> r_k(4, vector<int>(4));
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j) cin >> r_k[i][j];

            final_fn(state, r_k);
        }
        else if (a == "RSA_key_gen") {
            cout << "Enter prime number p or skip this step" << endl;
            long long p; cin >> p;
            cout << "Enter prime number q or skip this step" << endl;
            long long q; cin >> q;
            cout << "Key generation started..." << endl;
            rsa(p, q);
            cout << "!Use these keys only for education, they are not safe enough to use in real tasks!" << endl;
        }
        else if (a == "P_box") {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter your key to list" << endl;
            string line;
            getline(cin, line);
            istringstream iss(line);
            vector<int> key;
            int k;
            while (iss >> k) key.push_back(k);

            cout << "Enter your text now" << endl;
            string text;
            getline(cin, text);
            cout << pbox(key, text) << endl;
        }
    }
    return 0;
}