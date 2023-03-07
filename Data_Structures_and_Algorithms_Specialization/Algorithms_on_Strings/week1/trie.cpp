#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

typedef map<char, int> edges;
typedef vector<edges> trie;

trie build_trie(vector<string> &patterns) {
    trie t;
    int next_node = 1;
    t.emplace_back();
    for (auto &pattern: patterns) {
        size_t t_idx = 0;
        for (auto c: pattern) {
            auto it = t[t_idx].find(c);
            if (it == t[t_idx].end()) {
                t[t_idx].emplace(c, next_node);
                t.emplace_back();
                t_idx = next_node++;
            } else {
                t_idx = it->second;
            }
        }
    }
    return t;
}

int main() {
    size_t n;
    std::cin >> n;
    vector<string> patterns;
    for (size_t i = 0; i < n; i++) {
        string s;
        std::cin >> s;
        patterns.push_back(s);
    }

    trie t = build_trie(patterns);
    for (size_t i = 0; i < t.size(); ++i) {
        for (const auto &j: t[i]) {
            std::cout << i << "->" << j.second << ":" << j.first << "\n";
        }
    }

    return 0;
}
