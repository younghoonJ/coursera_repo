#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

class StockCharts {
public:
    void Solve() {
        vector<vector<int>> stock_data = ReadData();
        int result = MinCharts(stock_data);
        WriteResponse(result);
    }

private:
    vector<vector<int>> ReadData() {
        int num_stocks, num_points;
        cin >> num_stocks >> num_points;
        vector<vector<int>> stock_data(num_stocks, vector<int>(num_points));
        for (int i = 0; i < num_stocks; ++i)
            for (int j = 0; j < num_points; ++j) {
                cin >> stock_data[i][j];
            }
        return stock_data;
    }

    void WriteResponse(int result) { cout << result << "\n"; }

    bool compare(const vector<int> &stock1, const vector<int> &stock2) {
        for (int i = 0; i < stock1.size(); ++i)
            if (stock1[i] >= stock2[i])
                return false;
        return true;
    }

    int MinCharts(const vector<vector<int>> &stock_data) {
        // Replace this incorrect greedy algorithm with an
        // algorithm that correctly finds the minimum number
        // of charts on which we can put all the stock data
        // without intersections of graphs on one chart.

        int num_stocks = stock_data.size();
        int num_data = stock_data.at(0).size();
        int num_tot = num_stocks * 2 + 2;
        int src = 0;
        int dst = num_tot - 1;

        std::vector<std::vector<int>> adj(num_stocks, std::vector<int>(num_stocks));

        for (int i = 0; i < num_stocks; ++i) {
            for (int j = i + 1; j < num_stocks; ++j) {
                if (compare(stock_data[i], stock_data[j])) adj[j][i] = 1;
                if (compare(stock_data[j], stock_data[i])) adj[i][j] = 1;
            }
        }

        std::vector<std::vector<int>> net(num_tot, std::vector<int>(num_tot, 0));
        for (int i = 1; i < num_stocks + 1; ++i) {
            net[0][i] = 1;
            for (int j = 0; j < num_stocks; ++j)
                net[i][num_stocks + 1 + j] = adj[i - 1][j];
        }
        for (int i = num_stocks + 1; i < num_tot - 1; ++i)
            net[i].back() = 1;

        std::vector<size_t> bwd_path(num_tot, -1);
        auto bfs = [&]() -> bool {
            std::vector<bool> visited(num_tot, false);
            std::queue<size_t> q;
            visited[src] = true;
            q.push(src);
            while (not q.empty()) {
                auto from_node = q.front();
                q.pop();
                for (int to_node = 0; to_node < num_tot; ++to_node) {
                    if (not visited[to_node] and net[from_node][to_node] > 0) {
                        q.push(to_node);
                        visited[to_node] = true;
                        bwd_path[to_node] = from_node;
                        if (from_node == dst)
                            return true;
                    }
                }
            }
            return visited[dst];
        };
        int flow_amt = 0;
        while (bfs()) {
            auto min_flow = 2;
            auto curr_node = dst;
            while (curr_node != src) {
                auto from_node = bwd_path[curr_node];
                if (min_flow > net[from_node][curr_node]) {
                    min_flow = net[from_node][curr_node];
                }
                curr_node = from_node;
            }
            curr_node = dst;
            while (curr_node != src) {
                auto from_node = bwd_path[curr_node];
                net[from_node][curr_node] -= min_flow;
                net[curr_node][from_node] += min_flow;
                curr_node = from_node;
            }
//            for (int i = 0; i < flow.size(); ++i) {
//                for (int j = 0; j < flow.at(i).size(); ++j) {
//                    std::cout << flow.at(i).at(j) << " ";
//                }
//                std::cout << std::endl;
//            }
//            std::cout << std::endl;

            flow_amt += min_flow;
        }

        return num_stocks - flow_amt;
    }

};

int main() {
    std::ios_base::sync_with_stdio(false);
    StockCharts stock_charts;
    stock_charts.Solve();
    return 0;
}
