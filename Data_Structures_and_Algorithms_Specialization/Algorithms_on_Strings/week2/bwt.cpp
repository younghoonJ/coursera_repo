#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

string BWT(const string &text) {
    auto mat = std::vector<string>();
    for (size_t i = 0; i < text.size(); ++i)
        mat.emplace_back(text.substr(i) + text.substr(0, i));
    std::sort(mat.begin(), mat.end());
    string result;
    result.reserve(text.size());
    for (size_t i = 0; i < text.size(); ++i)
        result.push_back(mat[i].back());
    return result;
}

int main() {
    string text;
    cin >> text;
    cout << BWT(text) << endl;
    return 0;
}