#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.

constexpr int NUMCHAR = 5;

vector<int> sortCharacters(const vector<uint8_t> &text) {
    vector<int> order(text.size());
    std::vector<int> count(text.size());
    for (auto &i: text)
        ++count[i];
    for (int j = 1; j < NUMCHAR; ++j)
        count[j] += count[j - 1];
    for (int i = text.size() - 1; i >= 0; --i)
        order[--count[text[i]]] = i;
    return order;
}

vector<int> computeCharClasses(const vector<uint8_t> &text, const vector<int> &order) {
    vector<int> cls(text.size(), 0);
    cls[order[0]] = 0;
    for (int i = 1; i < text.size(); ++i) {
        if (text[order[i]] != text[order[i - 1]])
            cls[order[i]] = cls[order[i - 1]] + 1;
        else
            cls[order[i]] = cls[order[i - 1]];
    }
    return cls;
}

vector<int> sortDoubled(const vector<uint8_t> &text, int L, const vector<int> &order, const vector<int> &cls) {
    auto tsize = static_cast<int>(text.size());
    vector<int> cnt(tsize, 0);
    vector<int> newOrder(tsize);

    for (auto &c: cls)
        ++cnt[c];
    for (int j = 1; j < tsize; ++j)
        cnt[j] += cnt[j - 1];
    for (int i = tsize - 1; i >= 0; --i) {
        auto start = (order[i] - L + tsize) % tsize;
        newOrder[--cnt[cls[start]]] = start;
    }
    return newOrder;
}

vector<int> updateClasses(const vector<int> &newOrder, const vector<int> &cls, int L) {
    auto n = static_cast<int>(newOrder.size());
    std::vector<int> newCls(n);
    newCls[newOrder[0]] = 0;
    for (int i = 1; i < n; ++i) {
        auto cur = newOrder[i];
        auto prev = newOrder[i - 1];
        auto mid = (cur + L);
        auto midPrev = (prev + L) % n;
        if (cls[cur] != cls[prev] or cls[mid] != cls[midPrev])
            newCls[cur] = newCls[prev] + 1;
        else
            newCls[cur] = newCls[prev];
    }
    return newCls;
}

vector<int> BuildSuffixArray(const string &text) {
    vector<uint8_t> S;
    S.reserve(text.size());
    for (auto &c: text) {
        if (c == '$') S.emplace_back(0);
        else if (c == 'A')S.emplace_back(1);
        else if (c == 'C')S.emplace_back(2);
        else if (c == 'G')S.emplace_back(3);
        else if (c == 'T')S.emplace_back(4);
    }

    auto order = sortCharacters(S);
    auto cls = computeCharClasses(S, order);
    auto L = 1;
    while (L < text.size()) {
        order = sortDoubled(S, L, order, cls);
        cls = updateClasses(order, cls, L);
        L *= 2;
    }
    return order;
}

int main() {
    string text;
    cin >> text;
    vector<int> suffix_array = BuildSuffixArray(text);
    for (int i = 0; i < suffix_array.size(); ++i) {
        cout << suffix_array[i] << ' ';
    }
    cout << endl;
    return 0;
}
