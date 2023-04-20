#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::string;
using std::vector;


vector<int> sortCharacters(const vector<uint8_t> &text) {
    vector<int> order(text.size());
    std::vector<int> count(text.size());
    for (auto &i: text)
        ++count[i];
    for (int j = 1; j < 5; ++j)
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


vector<int> FindOccurrences(const string &pattern, const string &text, const vector<int> &suffix_array) {

    auto min_idx = 0;
    auto max_idx = int(text.size());

    auto contains = [&](int suffix_arr_i) {
        if (pattern.size() <= text.size() - suffix_array[suffix_arr_i] and
            pattern == text.substr(suffix_array[suffix_arr_i], pattern.size()))
            return true;
        return false;
    };

    while (min_idx < max_idx) {
        auto mid_idx = (min_idx + max_idx) / 2;
        auto subs = text.substr(suffix_array[mid_idx]);
        if (pattern > text.substr(suffix_array[mid_idx])) {
            min_idx = mid_idx + 1;
        } else {
            max_idx = mid_idx;
        }
    }
    auto start = min_idx;
    if (not contains(start)) return {};

    max_idx = int(text.size());
    while (min_idx < max_idx) {
        auto mid_idx = (min_idx + max_idx) / 2;
        if (pattern < text.substr(suffix_array[min_idx])) {
            if (contains(mid_idx)) {
                min_idx = mid_idx + 1;
            } else {
                max_idx = mid_idx;
            }
        } else {
            min_idx = mid_idx + 1;
        }
    }
    auto end = max_idx;
    vector<int> result;
    for (int i = start; i < end; ++i)
        result.push_back(suffix_array[i]);

    return result;
}

int main() {
    char buffer[100001];
    scanf("%s", buffer);
    string text = buffer;
    text += '$';
    vector<int> suffix_array = BuildSuffixArray(text);
    int pattern_count;
    scanf("%d", &pattern_count);
    vector<bool> occurs(text.length(), false);
    for (int pattern_index = 0; pattern_index < pattern_count; ++pattern_index) {
        scanf("%s", buffer);
        string pattern = buffer;
        vector<int> occurrences = FindOccurrences(pattern, text, suffix_array);
        for (int j = 0; j < occurrences.size(); ++j) {
            occurs[occurrences[j]] = true;
        }
    }
    for (int i = 0; i < occurs.size(); ++i) {
        if (occurs[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
    return 0;
}
