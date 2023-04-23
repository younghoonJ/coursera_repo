#include <algorithm>
#include <cstdio>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

using std::make_pair;
using std::map;
using std::pair;
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


// Data structure to store edges of a suffix tree.
struct Edge {
    // The ending node of this edge.
    int node;
    // Starting position of the substring of the text
    // corresponding to the label of this edge.
    int start;
    // Position right after the end of the substring of the text
    // corresponding to the label of this edge.
    int end;

    Edge(int node_, int start_, int end_) : node(node_), start(start_), end(end_) {}

    Edge(const Edge &e) = default;

    inline int len() { return end - start; }
};

// Build suffix tree of the string text given its suffix array suffix_array
// and LCP array lcp_array. Return the tree as a mapping from a node ID
// to the vector of all outgoing edges of the corresponding node. The edges in the
// vector must be sorted in the ascending order by the first character of the edge label.
// Root must have node ID = 0, and all other node IDs must be different
// nonnegative integers.
//
// For example, if text = "ACACAA$", an edge with label "$" from root to a node with ID 1
// must be represented by Edge(1, 6, 7). This edge must be present in the vector tree[0]
// (corresponding to the root node), and it should be the first edge in the vector 
// (because it has the smallest first character of all edges outgoing from the root).

map<int, vector<Edge> > SuffixTreeFromSuffixArrayRef(
        const vector<int> &suffix_array,
        const vector<int> &lcp_array,
        const string &text) {
    map<int, vector<Edge> > tree;
    int slen = int(text.size());
    int next_new_node = 0;
    tree.insert({next_new_node++, {}});
    tree.at(0).emplace_back(next_new_node++, suffix_array[0], slen);

    for (int i = 1; i < text.size(); ++i) {
//        auto c = text.substr(suffix_array[i]);
        auto lcp = lcp_array[i - 1];
        if (lcp == 0) {
            tree.at(0).emplace_back(next_new_node++, suffix_array[i], slen);
            continue;
        } else {
            int head = 0;
            int cnt = tree.at(head).back().end - tree.at(head).back().start;
            while (cnt < lcp) {
                head = tree.at(head).back().node;
                cnt += tree.at(head).back().end - tree.at(head).back().start;
            }
            if (cnt == lcp) {
                head = tree.at(head).back().node;
                tree.at(head).emplace_back(next_new_node++, suffix_array[i] + lcp, slen);
            } else {
                cnt -= tree.at(head).back().end - tree.at(head).back().start;
                auto tmp = tree.at(head).back();
                tree.at(head).pop_back();
                tmp.start += (lcp - cnt);
                auto cut_node = next_new_node++;
                tree.insert({cut_node, {tmp}});
                tree.at(head).emplace_back(cut_node, suffix_array[i] + cnt, suffix_array[i] + lcp);
                tree.at(cut_node).emplace_back(next_new_node++, suffix_array[i] + lcp, slen);
            }
        }

    }
    return tree;
}

map<int, vector<Edge> > SuffixTreeFromSuffixArray(
        const vector<int> &suffix_array,
        const vector<int> &lcp_array,
        const string &text) {
    map<int, vector<Edge> > tree;
    int slen = int(text.size());
    int next_new_node = 0;
    tree.insert({next_new_node++, {}});
    tree.at(0).emplace_back(next_new_node++, suffix_array[0], slen);


    auto find_node = [&](int common_len) -> std::pair<int, int> {
        int cnt = 0;
        int root = 0;
        while (true) {
            auto &nd = tree.at(root).back();
            if (cnt + nd.len() > common_len)
                return {root, cnt + nd.len() - common_len};
            else if (cnt + nd.len() == common_len)
                return {nd.node, 0};
            root = nd.node;
            cnt += nd.len();
        }
    };

    bool is_ready = false;
    int ready_head = 0;
    for (int i = 1; i < text.size(); ++i) {
        auto c = text.substr(suffix_array[i]);
        auto lcp = lcp_array[i - 1];
        auto next_lcp = (i < lcp_array.size()) ? lcp_array[i] : 0;
        if (lcp == 0) {
            if (next_lcp == 0) {
                tree.at(0).emplace_back(next_new_node++, suffix_array[i], slen);
                continue;
            }
            auto mid_node = next_new_node++;
            auto mid_pos = suffix_array[i] + next_lcp;
            tree.at(0).emplace_back(mid_node, suffix_array[i], mid_pos);
            tree[mid_node] = {{next_new_node++, mid_pos, slen}};
            is_ready = true;
            ready_head = mid_node;
        } else {
            if (next_lcp == 0) {
                if (is_ready) {
                    tree.at(ready_head).emplace_back(next_new_node++, suffix_array[i] + lcp, slen);
                } else {
                    auto root = find_node(lcp);
                    if (root.second == 0) {
                        tree.at(root.first).emplace_back(next_new_node++, suffix_array[i] + lcp, slen);
                    } else {
                        auto &tmp = tree.at(root.first).back();
                        auto cut_node = next_new_node++;
                        // fixed
                        tree.insert({cut_node, {{tmp.node, tmp.end - root.second, tmp.end}}});
                        tmp = {cut_node, suffix_array[i], suffix_array[i] + lcp};
                        tree.at(cut_node).emplace_back(next_new_node++, suffix_array[i] + lcp, slen);
                    }
                }
                continue;
            }
            if (lcp == next_lcp) { // start, lcp = next_lcp, end
                if (is_ready) {
                    tree.at(ready_head).emplace_back(next_new_node++, suffix_array[i] + lcp, slen);
                } else {
                    auto root = find_node(lcp);
                    if (root.second == 0) {
                        tree.at(root.first).emplace_back(next_new_node++, suffix_array[i] + lcp, slen);
                        is_ready = true;
                        ready_head = root.first;
                    } else {
                        // cut
                        auto &tmp = tree.at(root.first).back();
                        auto cut_node = next_new_node++;
                        // fixed
                        tree.insert({cut_node, {{tmp.node, tmp.end - root.second, tmp.end}}});
                        tmp = {cut_node, suffix_array[i], suffix_array[i] + lcp};
                        tree.at(cut_node).emplace_back(next_new_node++, suffix_array[i] + lcp, slen);
                        is_ready = true;
                        ready_head = cut_node;
                    }
                }
            } else if (lcp < next_lcp) {
                if (is_ready) {
                    auto mid_node = next_new_node++;
                    tree.at(ready_head).emplace_back(mid_node, suffix_array[i] + lcp, suffix_array[i] + next_lcp);
                    tree.insert({mid_node, {{next_new_node++, suffix_array[i] + next_lcp, slen}}});
                    is_ready = true;
                    ready_head = mid_node;
                } else {
                    auto root = find_node(lcp);
                    if (root.second == 0) {
                        auto mid_node = next_new_node++;
                        tree.at(root.first).emplace_back(mid_node, suffix_array[i] + lcp,
                                                         suffix_array[i] + next_lcp);
                        tree.insert({mid_node, {{next_new_node++, suffix_array[i] + next_lcp, slen}}});
                        is_ready = true;
                        ready_head = mid_node;
                    } else {
                        //cut
                        auto &tmp = tree.at(root.first).back();
                        auto cut_node = next_new_node++;
                        auto mid_node = next_new_node++;
                        tree.insert({cut_node, {{tmp.node, tmp.end - root.second, tmp.end}}});
                        tmp = {cut_node, suffix_array[i], suffix_array[i] + lcp};
                        tree.at(cut_node).emplace_back(mid_node, suffix_array[i] + lcp, suffix_array[i] + next_lcp);
                        tree.insert({mid_node, {{next_new_node++, suffix_array[i] + next_lcp, slen}}});
                        is_ready = true;
                        ready_head = mid_node;
                    }
                }
            } else { // lcp > next_lcp
                if (is_ready) {
                    tree.at(ready_head).emplace_back(next_new_node++, suffix_array[i] + lcp, slen);
                } else {
                    auto root = find_node(lcp);
                    if (root.second == 0) {
                        tree.at(root.first).emplace_back(next_new_node++, suffix_array[i] + lcp, slen);
                    } else {
                        auto &tmp = tree.at(root.first).back();
                        auto cut_node = next_new_node++;
                        // fixed
                        tree.insert({cut_node, {{tmp.node, tmp.end - root.second, tmp.end}}});
                        tmp = {cut_node, suffix_array[i], suffix_array[i] + lcp};
                        tree.at(cut_node).emplace_back(next_new_node++, suffix_array[i] + lcp, slen);
                    }
                }
                is_ready = false;
                ready_head = 0;


            }
        }

    }


    return
            tree;
}


std::vector<int> build_lcp_array(const std::string &text, const std::vector<int> &suffix_array) {
    std::vector<int> arr(suffix_array.size() - 1);
    for (int i = 0; i < arr.size(); ++i) {
        int cnt = 0;
        auto s0 = text.substr(suffix_array[i]);
        auto s1 = text.substr(suffix_array[i + 1]);
        while (s0[cnt] == s1[cnt])
            ++cnt;
        arr[i] = cnt;
    }

    return arr;

}
//
//#include <random>
//void debug() {
//
//    const std::string acgt = "ACGTACGTACGTACGT";
//
//    while (true) {
//        std::string text;
//        while (text.size() < 10) {
//            std::sample(acgt.begin(), acgt.end(), std::back_inserter(text), 1,
//                        std::mt19937{std::random_device{}()});
//        }
//
//        text += '$';
//
//        text = "GAACAACACG$";
//
//        std::cout << text << std::endl;
//
//        auto suffix_array = BuildSuffixArray(text);
//        auto lcp_array = build_lcp_array(text, suffix_array);
//
//
//        map<int, vector<Edge> > tree = SuffixTreeFromSuffixArray(suffix_array, lcp_array, text);
//
//
//        vector<pair<int, int> > stack(1, make_pair(0, 0));
//        vector<pair<int, int> > ans;
//        while (!stack.empty()) {
//            pair<int, int> p = stack.back();
//            stack.pop_back();
//            int node = p.first;
//            int edge_index = p.second;
//            if (!tree.count(node)) {
//                continue;
//            }
//            const vector<Edge> &edges = tree[node];
//            if (edge_index + 1 < edges.size()) {
//                stack.emplace_back(node, edge_index + 1);
//            }
//            ans.emplace_back(edges[edge_index].start, edges[edge_index].end);
//            stack.emplace_back(edges[edge_index].node, 0);
//        }
//        map<int, vector<Edge> > tree_ref = SuffixTreeFromSuffixArrayRef(suffix_array, lcp_array, text);
//        vector<pair<int, int> > stack_ref(1, make_pair(0, 0));
//        vector<pair<int, int> > ans_ref;
//        while (!stack_ref.empty()) {
//            pair<int, int> p = stack_ref.back();
//            stack_ref.pop_back();
//            int node = p.first;
//            int edge_index = p.second;
//            if (!tree_ref.count(node)) {
//                continue;
//            }
//            const vector<Edge> &edges = tree_ref[node];
//            if (edge_index + 1 < edges.size()) {
//                stack_ref.emplace_back(node, edge_index + 1);
//            }
//            ans_ref.emplace_back(edges[edge_index].start, edges[edge_index].end);
//            stack_ref.emplace_back(edges[edge_index].node, 0);
//        }
//
//        for (int i = 0; i < ans.size(); ++i) {
//            std::cout << text.substr(ans[i].first, ans[i].second) << "   "
//                      << text.substr(ans_ref[i].first, ans_ref[i].second) << std::endl;
//            if (ans[i].second - ans[i].first != ans_ref[i].second - ans_ref[i].first) {
//                std::cout << "found!  " << text << std::endl;
//                for (auto &s: suffix_array) {
//                    std::cout << s << " ";
//                }
//                std::cout << std::endl;
//                for (auto &s: lcp_array) {
//                    std::cout << s << " ";
//                }
//                std::cout << std::endl;
//
//                for (auto &s: suffix_array) {
//                    std::cout << text.substr(s)<<std::endl;
//                }
//
//                return;
//            }
//        }
//    }
//
//}


int main() {


//    if (1) debug();


    char buffer[200001];
    scanf("%s", buffer);
    string text = buffer;
    vector<int> suffix_array(text.length());
    for (int i = 0; i < text.length(); ++i) {
        scanf("%d", &suffix_array[i]);
    }
    vector<int> lcp_array(text.length() - 1);
    for (int i = 0; i + 1 < text.length(); ++i) {
        scanf("%d", &lcp_array[i]);
    }
    // Build the suffix tree and get a mapping from
    // suffix tree node ID to the list of outgoing Edges.
    map<int, vector<Edge> > tree = SuffixTreeFromSuffixArray(suffix_array, lcp_array, text);
    printf("%s\n", buffer);

//
//
//
//     Output the edges of the suffix tree in the required order.
//     Note that we use here the contract that the root of the tree
//     will have node ID = 0 and that each vector of outgoing edges
//     will be sorted by the first character of the corresponding edge label.
//
//     The following code avoids recursion to avoid stack overflow issues.
//     It uses a stack to convert recursive function to a while loop.
//     The stack stores pairs (node, edge_index).
//     This code is an equivalent of
//
//        OutputEdges(tree, 0);
//
//     for the following _recursive_ function OutputEdges:
//
//     void OutputEdges(map<int, vector<Edge> > tree, int node_id) {
//       const vector<Edge>& edges = tree[node_id];
//       for (int edge_index = 0; edge_index < edges.size(); ++edge_index) {
//         printf("%d %d\n", edges[edge_index].start, edges[edge_index].end);
//         OutputEdges(tree, edges[edge_index].node);
//       }
//     }
//


    vector<pair<int, int> > stack(1, make_pair(0, 0));
    while (!stack.empty()) {
        pair<int, int> p = stack.back();
        stack.pop_back();
        int node = p.first;
        int edge_index = p.second;
        if (!tree.count(node)) {
            continue;
        }
        const vector<Edge> &edges = tree[node];
        if (edge_index + 1 < edges.size()) {
            stack.emplace_back(node, edge_index + 1);
        }
        printf("%d %d\n", edges[edge_index].start, edges[edge_index].end);
        stack.emplace_back(edges[edge_index].node, 0);
    }
    return 0;


}
