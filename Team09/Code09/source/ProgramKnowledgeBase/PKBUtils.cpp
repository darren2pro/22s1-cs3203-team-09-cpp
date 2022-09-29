#include "PKBUtils.h"

namespace PKB {
    void PKB::addToSetInMap(std::unordered_map<std::string, std::unordered_set<std::string>>& map,
        const std::string key, const std::string val) {
        if (map.find(key) == map.end()) {
            std::unordered_set<std::string> vals;
            vals.insert(val);
            map[key] = vals;
        }
        else {
            map.at(key).insert(val);
        }
    }

    void PKB::addToSetInMap(std::unordered_map<std::string,
        std::unordered_set<std::pair<std::string, std::string>, pairHash>>&map,
        const std::string key, const std::pair<std::string, std::string> val) {
        if (map.find(key) == map.end()) {
            std::unordered_set<std::pair<std::string, std::string>, pairHash> vals;
            vals.insert(val);
            map[key] = vals;
        }
        else {
            map.at(key).insert(val);
        }
    }
}