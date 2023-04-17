#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

string InverseBWT(const string &bwt) {
    if (bwt.size() == 1) return bwt;

    size_t pos_a = 0, pos_c = 0, pos_g = 0, pos_t;
    for (auto &c: bwt) {
        if (c == 'A') ++pos_a;
        else if (c == 'C') ++pos_c;
        else if (c == 'G') ++pos_g;
    }
    pos_t = 1 + pos_a + pos_c + pos_g;
    pos_g = 1 + pos_a + pos_c;
    pos_c = 1 + pos_a;
    pos_a = 1;

    std::string text;
    text.reserve(bwt.size());
    text.push_back('$');
    text.push_back(bwt[0]);
    while (text.size() < bwt.size()) {
        if (text.back() == 'A') text.push_back(bwt[pos_a++]);
        else if (text.back() == 'C') text.push_back(bwt[pos_c++]);
        else if (text.back() == 'G') text.push_back(bwt[pos_g++]);
        else if (text.back() == 'T') text.push_back(bwt[pos_t++]);
    }
    std::reverse(text.begin(), text.end());
    return text;
}

int main() {
    string bwt;
    cin >> bwt;
    cout << InverseBWT(bwt) << endl;
    return 0;
}
