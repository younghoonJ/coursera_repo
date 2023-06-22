#include <ios>
#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int from;
    int to;
};

struct RGB {
    int b, g, r;

    explicit RGB(int i) : b(i * 3), g(b - 1), r(b - 2) {}
};

struct ConvertGSMNetworkProblemToSat {
    int numVertices;
    vector<Edge> edges;

    ConvertGSMNetworkProblemToSat(int n, int m) : numVertices(n), edges(m) {}

    void printEquisatisfiableSatFormula() {
        // each vertex has one and only one color
        int num_clauses = numVertices * 4 + 3 * static_cast<int>(edges.size());
        int num_vars = numVertices * 3;
        cout << num_clauses << ' ' << num_vars << endl;

        for (auto i = 1; i < numVertices + 1; ++i) {
            auto v = RGB(i);
            cout << v.r << ' ' << v.g << ' ' << v.b << " 0\n"; // r or g or b
            cout << -v.r << ' ' << -v.g << " 0\n";             // not r or not g;
            cout << -v.r << ' ' << -v.b << " 0\n";
            cout << -v.g << ' ' << -v.b << " 0\n";
        }

        // two verticies at the end of each edge should have different colors
        for (const auto &e: edges) {
            auto from = RGB(e.from);
            auto to = RGB(e.to);
            cout << -from.r << ' ' << -to.r << " 0\n";
            cout << -from.g << ' ' << -to.g << " 0\n";
            cout << -from.b << ' ' << -to.b << " 0\n";
        }
    }
};

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    ConvertGSMNetworkProblemToSat converter(n, m);
    for (int i = 0; i < m; ++i) {
        cin >> converter.edges[i].from >> converter.edges[i].to;
    }

    converter.printEquisatisfiableSatFormula();

    return 0;
}
