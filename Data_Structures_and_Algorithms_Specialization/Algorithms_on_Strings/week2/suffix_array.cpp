#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<int> BuildSuffixArray(const string &text) {
    vector<pair<string, int>> arr;
    for (size_t i = 0; i < text.size(); ++i)
        arr.emplace_back(text.substr(i), i);
    std::sort(arr.begin(), arr.end(), [&](const pair<string, size_t> &left, const pair<string, size_t> &right) {
        return left.first < right.first;
    });
    vector<int> result;
    result.reserve(arr.size());
    for (auto &p: arr)
        result.push_back(p.second);
    return result;
}

int main() {
    string text;
    cin >> text;
    vector<int> suffix_array = BuildSuffixArray(text);
    for (int i = 0; i < suffix_array.size(); ++i) {
        cout << suffix_array[i] << ' ';
    }
    cout << endl;
    return 0;
}
