#include "StringHelper.hpp"

using namespace std;

int main() {
    string str{"123"};
    auto res = StringHelper::ReadLines(2);
    for (const auto& e:res) {
        cout << e << endl;
    }

    return 0;
}