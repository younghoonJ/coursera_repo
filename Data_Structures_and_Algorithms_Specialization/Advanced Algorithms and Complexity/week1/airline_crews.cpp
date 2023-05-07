#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

class MaxMatching {
public:
    void Solve() {
        vector<vector<bool>> adj_matrix = ReadData();
        vector<int> matching = FindMatching(adj_matrix);
        WriteResponse(matching);
    }

private:
    vector<vector<bool>> ReadData() {
        int num_left, num_right;
        cin >> num_left >> num_right;
        vector<vector<bool>> adj_matrix(num_left, vector<bool>(num_right));
        for (int i = 0; i < num_left; ++i)
            for (int j = 0; j < num_right; ++j) {
                int bit;
                cin >> bit;
                adj_matrix[i][j] = (bit == 1);
            }
        return adj_matrix;
    }

    void WriteResponse(const vector<int> &matching) {
        for (int i = 0; i < matching.size(); ++i) {
            if (i > 0)
                cout << " ";
            if (matching[i] == -1)
                cout << "-1";
            else
                cout << (matching[i] + 1);
        }
        cout << "\n";
    }


    vector<int>
    FindMatching(const vector<vector<bool>> &adj_matrix) {
        // Replace this code with an algorithm that finds the maximum
        // matching correctly in all cases.
        int num_left = adj_matrix.size();
        int num_right = adj_matrix[0].size();
        int num_original_node = num_left + num_right;
        int num_total_nodes = num_left + num_right + 2;
        size_t source_node = num_total_nodes - 2;
        size_t dest_node = num_total_nodes - 1;

        auto flow = std::vector<std::vector<int>>(
                num_total_nodes,
                std::vector<int>(num_total_nodes, false));

        std::vector<size_t> bwd_path(num_total_nodes);

        auto bfs = [&]() -> bool {
            std::vector<bool> visited(num_total_nodes, false);
            std::queue<size_t> q;

            q.push(source_node);
            while (not q.empty()) {
                auto from_node = q.front();
                q.pop();
                if (from_node == source_node) {
                    for (size_t to_node = 0; to_node < num_left; ++to_node) {
                        if (not visited[to_node] and 1 - flow[from_node][to_node] > 0) {
                            q.push(to_node);
                            visited[to_node] = true;
                            bwd_path[to_node] = from_node;
                        }
                    }
                } else if (from_node < num_left) {
                    for (size_t to_node = num_left; to_node < num_original_node; ++to_node) {
                        if (not visited[to_node] and
                            int(adj_matrix[from_node][to_node - num_left]) - flow[from_node][to_node] > 0) {
                            q.push(to_node);
                            visited[to_node] = true;
                            bwd_path[to_node] = from_node;
                        }
                    }
                } else {
                    if (not visited[dest_node] and 1 - flow[from_node][dest_node] > 0) {
                        bwd_path[dest_node] = from_node;
                        return true;
                    }
                    for (size_t to_node = 0; to_node < num_left; ++to_node) { // right to left
                        if (not visited[to_node] and flow[from_node][to_node] < 0) {
                            q.push(to_node);
                            visited[to_node] = true;
                            bwd_path[to_node] = from_node;
                        }
                    }
                }
            }
            return false;
        };


        int cnt = 0;
        while (bfs()) {
            int min_flow = 2;
            auto curr_node = dest_node;
            while (curr_node != source_node) {
                auto prev_node = bwd_path[curr_node];
                if (curr_node == dest_node or prev_node == source_node) {
                    if (min_flow > 1) min_flow = 1;
                } else if (curr_node >= num_left) { // left to right(curr_node)
                    if (min_flow > int(adj_matrix[prev_node][curr_node - num_left]) - flow[prev_node][curr_node])
                        min_flow = int(adj_matrix[prev_node][curr_node - num_left]) - flow[prev_node][curr_node];
                } else { //  right to left(curr_node) => 0 capacity
                    if (min_flow > -flow[prev_node][curr_node])
                        min_flow = -flow[prev_node][curr_node];
                }
                curr_node = prev_node;
            }

            curr_node = dest_node;
            while (curr_node != source_node) {
                auto prev_node = bwd_path[curr_node];
                flow[prev_node][curr_node] += min_flow;
                flow[curr_node][prev_node] -= min_flow;
                curr_node = prev_node;
            }

//            for (auto &r: flow) {
//                for (auto &v: r) {
//                    std::cout << v << " ";
//                }
//                std::cout << std::endl;
//            }
//            std::cout << std::endl;
            cnt++;
        }

        std::vector<int> matching(num_left, -1);
        for (size_t i = 0; i < num_left; ++i) {
            for (size_t j = num_left; j < num_original_node; ++j) {
                if (flow[i][j] > 0) {
//                    std::cout << i << ", " << j << ": " << flow[i][j] << std::endl;
                    matching[i] = static_cast<int>(j - num_left);
                }
            }

        }
        return matching;
    }

};

int main() {
    std::ios_base::sync_with_stdio(false);
    MaxMatching max_matching;
    max_matching.Solve();
    return 0;
}
