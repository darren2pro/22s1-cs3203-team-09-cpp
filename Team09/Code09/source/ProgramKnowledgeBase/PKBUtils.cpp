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
    void PKB::setStarFromBaseMap(std::unordered_set<std::pair<std::string, std::string>, pairHash>& set,
        std::unordered_map<std::string, std::unordered_set<std::string>>& star,
        const std::unordered_map<std::string, std::unordered_set<std::string>> base,
        std::string key) {
        if (base.find(key) == base.end()) {
            return;
        }

        std::vector<std::string> list;
        list.push_back(key);
        while (!list.empty()) {
            std::string currKey = list.back();
            list.pop_back();
            for (const auto& val : base.at(currKey)) {
                set.insert(std::make_pair(key, val));
                addToSetInMap(star, key, val);
                if (base.find(val) != base.end()) {
                    list.push_back(val);
                }
            }
        }
    }

    void PKB::setStarFromBaseMap(std::unordered_map<std::string, std::unordered_set<std::string>>& star,
        const std::unordered_map<std::string, std::unordered_set<std::string>> base,
        std::string key) {
        if (base.find(key) == base.end()) {
            return;
        }

        std::vector<std::string> list;
        list.push_back(key);
        while (!list.empty()) {
            std::string currKey = list.back();
            list.pop_back();
            for (const auto& val : base.at(currKey)) {
                addToSetInMap(star, key, val);
                if (base.find(val) != base.end()) {
                    list.push_back(val);
                }
            }
        }
    }
}