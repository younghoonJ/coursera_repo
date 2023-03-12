#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <queue>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;
using std::queue;

// Build a suffix tree of the string text and return a vector
// with all of the labels of its edges (the corresponding
// substrings of the text) in any order.

struct CNode {
    std::map<char, int> next;

    CNode() = default;
};

struct Node {
    map<string, int> next;

    Node() = default;
};

vector<string> ComputeSuffixTreeEdges(const string &text) {
    vector<CNode> trie;

    trie.emplace_back();
    int next_node = 1;
    for (int i = 0; i < text.size(); ++i) {
        int t_idx = 0;
        for (int j = i; j < text.size(); ++j) {
            auto it = trie[t_idx].next.find(text[j]);
            if (it == trie[t_idx].next.end()) {
                trie.emplace_back();
                trie[t_idx].next.insert({text[j], next_node});
                t_idx = next_node++;
            } else {
                t_idx = it->second;
            }
        }
    }

    vector<string> result;
    vector<Node> tree;
    queue<int> q;
    q.push(0);

    while (not q.empty()) {
        auto trie_idx = q.front();
        q.pop();
        for (auto &it: trie[trie_idx].next) {
            string s{it.first};
            int next_idx = it.second;
            while (true) {
                auto next_size = trie[next_idx].next.size();
                if (next_size == 0) {
                    result.push_back(s);
                    break;
                } else if (next_size == 1) {
                    auto iit = trie[next_idx].next.begin();
                    s.push_back(iit->first);
                    next_idx = iit->second;
                } else {
                    result.push_back(s);
                    q.push(next_idx);
                    break;
                }
            }
        }

    }

    return result;
}

int main() {
    string text;
    cin >> text;
    vector<string> edges = ComputeSuffixTreeEdges(text);
    for (int i = 0; i < edges.size(); ++i) {
        cout << edges[i] << endl;
    }
    return 0;
}
