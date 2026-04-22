#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <fstream>

using namespace std;

// ==========================================
// 1. CÁC BẢNG HẰNG SỐ (TABLES)
// ==========================================

const int IP[] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

const int IP_INV[] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

const int E[] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

const int P[] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

const int S[8][4][16] = {
    {{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
    {{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
    {{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
    {{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
    {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
    {{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
    {{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
    {{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}
};

const int PC1[] = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2,
    59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39,
    31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37,
    29, 21, 13, 5, 28, 20, 12, 4
};

const int PC2[] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

const int SHIFTS[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

// ==========================================
// 2. HELPER FUNCTIONS
// ==========================================

class Helper {
public:
    static string stringToBinary(string text) {
        string binary = "";
        for (char& c : text) {
            binary += bitset<8>(c).to_string();
        }
        return binary;
    }

    static string binaryToString(string binary) {
        string text = "";
        for (size_t i = 0; i < binary.length(); i += 8) {
            bitset<8> b(binary.substr(i, 8));
            text += char(b.to_ulong());
        }
        return text;
    }

    static string xorStrings(string a, string b) {
        string res = "";
        for (size_t i = 0; i < a.length(); i++) {
            res += (a[i] == b[i] ? '0' : '1');
        }
        return res;
    }

    static string permute(string input, const int* table, int n) {
        string res = "";
        for (int i = 0; i < n; i++) {
            res += input[table[i] - 1];
        }
        return res;
    }

    static string shiftLeft(string s, int n) {
        return s.substr(n) + s.substr(0, n);
    }
};

// ==========================================
// 3. KEY GENERATOR CLASS
// ==========================================

class KeyGenerator {
public:
    static vector<string> generateRoundKeys(string key64) {
        vector<string> roundKeys;
        // PC-1: 64 bit -> 56 bit
        string key56 = Helper::permute(key64, PC1, 56);
        string L = key56.substr(0, 28);
        string R = key56.substr(28, 28);

        for (int i = 0; i < 16; i++) {
            L = Helper::shiftLeft(L, SHIFTS[i]);
            R = Helper::shiftLeft(R, SHIFTS[i]);
            // PC-2: 56 bit -> 48 bit
            roundKeys.push_back(Helper::permute(L + R, PC2, 48));
        }
        return roundKeys;
    }
};

// ==========================================
// 4. DES CORE CLASS
// ==========================================

class DES {
private:
    vector<string> roundKeys;
    bool debug;

    string feistel(string R, string K) {
        // 1. Expansion: 32 -> 48 bit
        string expandedR = Helper::permute(R, E, 48);
        // 2. XOR with Round Key
        string xored = Helper::xorStrings(expandedR, K);
        // 3. S-Box substitution
        string sBoxOut = "";
        for (int i = 0; i < 8; i++) {
            string block = xored.substr(i * 6, 6);
            int row = (block[0] - '0') * 2 + (block[5] - '0');
            int col = (block[1] - '0') * 8 + (block[2] - '0') * 4 + (block[3] - '0') * 2 + (block[4] - '0');
            sBoxOut += bitset<4>(S[i][row][col]).to_string();
        }
        // 4. P-Permutation: 32 bit
        return Helper::permute(sBoxOut, P, 32);
    }

public:
    DES(string key, bool debugMode = false) : debug(debugMode) {
        roundKeys = KeyGenerator::generateRoundKeys(key);
    }

    string processBlock(string block, bool encrypt) {
        if (block.length() < 64) block.append(64 - block.length(), '0');

        // Initial Permutation
        string permuted = Helper::permute(block, IP, 64);
        string L = permuted.substr(0, 32);
        string R = permuted.substr(32, 32);

        if (!encrypt) reverse(roundKeys.begin(), roundKeys.end());

        for (int i = 0; i < 16; i++) {
            string oldR = R;
            R = Helper::xorStrings(L, feistel(R, roundKeys[i]));
            L = oldR;
            if (debug) cout << "Round " << i + 1 << ": L=" << L << " R=" << R << endl;
        }

        if (!encrypt) reverse(roundKeys.begin(), roundKeys.end()); // Khôi phục thứ tự key

        // Final Permutation (IP inverse) - Lưu ý 32 bit cuối rồi đến 32 bit đầu (L+R hoán đổi)
        return Helper::permute(R + L, IP_INV, 64);
    }

    string run(string input, bool encrypt) {
        string output = "";
        for (size_t i = 0; i < input.length(); i += 64) {
            output += processBlock(input.substr(i, 64), encrypt);
        }
        return output;
    }
};

// ==========================================
// 5. TRIPLE DES CLASS
// ==========================================

class TripleDES {
private:
    DES des1, des2, des3;
public:
    TripleDES(string k1, string k2, string k3) : des1(k1), des2(k2), des3(k3) {}

    string encrypt(string plaintext) {
        // C = E(K3, D(K2, E(K1, P)))
        string res = des1.run(plaintext, true);
        res = des2.run(res, false);
        return des3.run(res, true);
    }

    string decrypt(string ciphertext) {
        // P = D(K1, E(K2, D(K3, C)))
        string res = des3.run(ciphertext, false);
        res = des2.run(res, true);
        return des1.run(res, false);
    }
};

// ==========================================
// 6. MAIN & MENU INTERFACE
// ==========================================

void printMenu() {
    cout << "\n===============================" << endl;
    cout << "     DES & TRIPLE DES TOOL     " << endl;
    cout << "===============================" << endl;
    cout << "1. Ma hoa DES" << endl;
    cout << "2. Giai ma DES" << endl;
    cout << "3. Ma hoa Triple DES" << endl;
    cout << "4. Giai ma Triple DES" << endl;
    cout << "5. Thoat" << endl;
    cout << "Chon chuc nang (1-5): ";
}

int main() {
    int choice;
    string input, k1, k2, k3;

    while (true) {
        printMenu();
        cin >> choice;
        if (choice == 5) break;

        cout << "Nhap plaintext/ciphertext (Binary hoac Text): ";
        cin.ignore();
        getline(cin, input);
        
        // Auto-detect text -> binary
        bool isBinary = true;
        for (char c : input) if (c != '0' && c != '1') isBinary = false;
        if (!isBinary) input = Helper::stringToBinary(input);

        if (choice == 1 || choice == 2) {
            cout << "Nhap Key (64-bit binary): ";
            cin >> k1;
            DES des(k1);
            string result = des.run(input, (choice == 1));
            cout << "\nKET QUA (Binary): " << result << endl;
            cout << "KET QUA (Text):   " << Helper::binaryToString(result) << endl;
        } 
        else if (choice == 3 || choice == 4) {
            cout << "Nhap Key 1 (64-bit binary): "; cin >> k1;
            cout << "Nhap Key 2 (64-bit binary): "; cin >> k2;
            cout << "Nhap Key 3 (64-bit binary): "; cin >> k3;
            TripleDES tdes(k1, k2, k3);
            string result = (choice == 3) ? tdes.encrypt(input) : tdes.decrypt(input);
            cout << "\nKET QUA (Binary): " << result << endl;
            cout << "KET QUA (Text):   " << Helper::binaryToString(result) << endl;
        }
    }
    return 0;
}