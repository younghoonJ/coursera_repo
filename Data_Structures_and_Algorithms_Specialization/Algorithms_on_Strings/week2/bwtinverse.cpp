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

    std::vector<int> a_before(bwt.size() + 1, 0);
    std::vector<int> c_before(bwt.size() + 1, 0);
    std::vector<int> g_before(bwt.size() + 1, 0);
    std::vector<int> t_before(bwt.size() + 1, 0);

    int cnt_a = 0, cnt_c = 0, cnt_g = 0, cnt_t;
    for (size_t i = 0; i < bwt.size(); ++i) {
        a_before[i + 1] = a_before[i];
        c_before[i + 1] = c_before[i];
        g_before[i + 1] = g_before[i];
        t_before[i + 1] = t_before[i];

        auto c = bwt[i];
        if (c == 'A') {
            ++cnt_a;
            ++a_before[i + 1];
        } else if (c == 'C') {
            ++cnt_c;
            ++c_before[i + 1];
        } else if (c == 'G') {
            ++cnt_g;
            ++g_before[i + 1];
        } else if (c == 'T') {
            ++t_before[i + 1];
        }

    }
    cnt_t = 1 + cnt_a + cnt_c + cnt_g;
    cnt_g = 1 + cnt_a + cnt_c;
    cnt_c = 1 + cnt_a;
    cnt_a = 1;

    std::string text;
    text.reserve(bwt.size());
    text.push_back('$');
    auto cursor = 0;
    while (text.size() < bwt.size()) {
        auto c = bwt[cursor];
        text.push_back(c);
        if (c == 'A') cursor = cnt_a + a_before[cursor];
        else if (c == 'C') cursor = cnt_c + c_before[cursor];
        else if (c == 'G') cursor = cnt_g + g_before[cursor];
        else if (c == 'T') cursor = cnt_t + t_before[cursor];
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
