#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int const Letters = 4;
int const NA = -1;

struct Node {
    int next[Letters];
    bool patternEnd;

    Node() {
        fill(next, next + Letters, NA);
        patternEnd = false;
    }
};

int letterToIndex(char letter) {
    switch (letter) {
        case 'A':
            return 0;
            break;
        case 'C':
            return 1;
            break;
        case 'G':
            return 2;
            break;
        case 'T':
            return 3;
            break;
        default:
            assert (false);
            return -1;
    }
}

vector<int> solve(string text, int n, vector<string> &patterns) {
    vector<int> result;
    vector<Node> trie;
    trie.emplace_back();
    int next_node = 1;
    for (auto &pattern: patterns) {
        int t_idx = 0;
        for (char &it: pattern) {
            auto next_idx = letterToIndex(it);
            if (trie[t_idx].next[next_idx] == NA) {
                trie.emplace_back();
                trie[t_idx].next[next_idx] = next_node;
                t_idx = next_node++;
            } else {
                t_idx = trie[t_idx].next[next_idx];
            }
        }
        trie[t_idx].patternEnd = true;
    }

    for (size_t i = 0; i < text.size(); ++i) {
        auto nd_idx = 0;
        for (size_t j = i; j < text.size(); ++j) {
            auto next_idx = trie[nd_idx].next[letterToIndex(text[j])];
            if (next_idx == NA) break;
            nd_idx = next_idx;
            if (trie[nd_idx].patternEnd) {
                result.push_back(static_cast<int>(i));
                break;
            }
        }
    }


    return result;
}

int main() {
    string t;
    cin >> t;

    int n;
    cin >> n;

    vector<string> patterns(n);
    for (int i = 0; i < n; i++) {
        cin >> patterns[i];
    }

    vector<int> ans;
    ans = solve(t, n, patterns);

    for (int i = 0; i < (int) ans.size(); i++) {
        cout << ans[i];
        if (i + 1 < (int) ans.size()) {
            cout << " ";
        } else {
            cout << endl;
        }
    }

    return 0;
}
