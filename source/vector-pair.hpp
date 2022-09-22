#include <string>
#include <utility>
#include <vector>

typedef std::vector<std::pair<std::string, std::string>> vector_pair;

std::vector<std::string> vectorPairKeys(vector_pair pair) {
    std::vector<std::string> keys;
    for (auto const& [key, val] : pair) {
        keys.push_back(key);
    }
    return keys;
}

std::vector<std::string> vectorPairValues(vector_pair pair) {
    std::vector<std::string> values;
    for (auto const& [key, val] : pair) {
        values.push_back(val);
    }
    return values;
}
