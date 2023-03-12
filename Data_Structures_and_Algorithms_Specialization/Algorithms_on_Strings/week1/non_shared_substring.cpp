#include <iostream>
#include <map>
#include <queue>
#include<stack>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::queue;
using std::string;
using std::vector;
using std::stack;

struct CNode {
    std::map<char, int> next;

    CNode() = default;
};

struct Node {
    map<string, int> next;
    int parent;
    bool left;
    bool right;

    explicit Node(int parent, bool left, bool right) : parent(parent),
                                                       left(left), right(right) {};
};

void buildTrie(vector<CNode> &trie, const string &text) {
    if (trie.empty())
        trie.emplace_back();
    for (int i = 0; i < text.size(); ++i) {
        int t_idx = 0;
        for (int j = i; j < text.size(); ++j) {
            auto it = trie[t_idx].next.find(text[j]);
            if (it == trie[t_idx].next.end()) {
                trie[t_idx].next.insert({text[j], static_cast<int>(trie.size())});
                trie.emplace_back();
                t_idx = static_cast<int>(trie.size() - 1);
            } else {
                t_idx = it->second;
            }
        }
    }
};

void buildTree(vector<CNode> &trie, vector<Node> &tree) {
    queue<int> q;
    queue<int> q2;
    q.push(0);
    q2.push(0);
    tree.emplace_back(-1, true, true);

    while (not q.empty()) {
        auto trie_idx = q.front();
        auto curr_parent = q2.front();
        q.pop();
        q2.pop();
        for (auto &it: trie[trie_idx].next) {
            string s{it.first};
            int next_idx = it.second;
            while (true) {
                auto next_size = trie[next_idx].next.size();
                if (next_size == 0) {
                    tree[curr_parent].next.insert({s, tree.size()});
                    if (s.back() == '$')
                        tree.emplace_back(curr_parent, true, false);
                    else
                        tree.emplace_back(curr_parent, false, true);
                    break;
                } else if (next_size == 1) {
                    auto iit = trie[next_idx].next.begin();
                    s.push_back(iit->first);
                    next_idx = iit->second;
                } else {
                    tree[curr_parent].next.insert({s, tree.size()});
                    q2.push(static_cast<int>(tree.size()));
                    q.push(next_idx);
                    for (auto iit = trie[next_idx].next.begin();
                         iit != trie[next_idx].next.end(); ++iit) {
                    }
                    tree.emplace_back(curr_parent, false, false);
                    break;
                }
            }
        }
    }

    stack<int> stk;
    vector<bool> visited(tree.size(), false);
    stk.push(0);
    while (not stk.empty()) {
        int now = stk.top();
        stk.pop();
        if (not visited[now]) {
            visited[now] = true;
            if (tree[now].next.empty()) {
                int idx = now;
                int prev = now;
                while (idx != 0) {
                    prev = idx;
                    idx = tree[idx].parent;
                    tree[idx].left |= tree[prev].left;
                    tree[idx].right |= tree[prev].right;
                }
            }
            for (auto &it: tree[now].next) {
                if (not visited[it.second]) {
                    stk.push(it.second);
                }
            }
        }
    }

}

string solve(string &string1, string &string2) {
    string1 += '$';
    string2 += '#';

    vector<CNode> trie;
    buildTrie(trie, string1);
    buildTrie(trie, string2);

    vector<Node> tree;
    buildTree(trie, tree);

//    vector<string> candidates;

    queue<int> q;
    q.push(0);
    string result(string1);
    while (not q.empty()) {
        int now = q.front();
        q.pop();
        for (auto it = tree[now].next.begin(); it != tree[now].next.end(); ++it) {
            if (tree[it->second].left and tree[it->second].right) {
                q.push(it->second);
            } else if (tree[it->second].left and not tree[it->second].right) {
                int idx = it->second;
                vector<string> ss;
                while (idx != 0) {
                    for (auto iit = tree[tree[idx].parent].next.begin();
                         iit != tree[tree[idx].parent].next.end();
                         ++iit) {
                        if (iit->second == idx) {
                            ss.push_back(iit->first);
                        }
                    }
                    idx = tree[idx].parent;
                }
                string a;
                for (auto sit = ss.rbegin(); sit != ss.rend(); ++sit) {
                    if (sit == std::prev(ss.rend()))
                        a.push_back((*sit).at(0));
                    else a.append(*sit);
                }
                if (a.back() != '$') {
//                candidates.push_back(a);
                    if (a.size() < result.size()) {
                        result = a;
                    }
                }

            }
        }
    }


    return result;
}

int main() {
    string p;
    cin >> p;
    string q;
    cin >> q;

    string ans = solve(p, q);

    cout << ans << endl;

    return 0;
}
