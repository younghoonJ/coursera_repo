#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::string;
using std::vector;

// Find all occurrences of the pattern in the text and return a
// vector with all positions in the text (starting from 0) where
// the pattern starts in the text.

vector<int> find_pattern(const string &pattern, const string &text) {
    if (pattern.size() > text.size())
        return {};

    auto computePrefixFunction = [](const std::string &p) {
        std::vector<int> s(p.size(), 0);
        s[0] = 0;
        int border = 0;
        for (int i = 1; i < p.size(); ++i) {
            while (border > 0 and p[i] != p[border])
                border = s[border - 1];
            if (p[i] == p[border])
                ++border;
            else
                border = 0;
            s[i] = border;
        }
        return s;
    };

    auto cc = pattern + '$' + text;
    auto pfn = computePrefixFunction(cc);
    vector<int> result;
    auto psize = static_cast<int>(pattern.size());
    for (int i = psize + 1; i < cc.size(); ++i) {
        if (pfn[i] == psize)
            result.push_back(i - 2 * psize);
    }
    return result;
}

int main() {
    string pattern, text;
    cin >> pattern;
    cin >> text;
    vector<int> result = find_pattern(pattern, text);
    for (int i = 0; i < result.size(); ++i) {
        printf("%d ", result[i]);
    }
    printf("\n");
    return 0;
}