#pragma once

#include <unordered_set>
#include <unordered_map>
#include "../TNode/TNode.h"

namespace PKB {
	using LineNum = std::string;
	using PrevLine = std::string;
	using NextLine = std::string;
	using ParentLine = std::string;
	using ChildLine = std::string;
    using ExprStr = std::string;
	using Variable = std::string;
	using Constant = std::string;
	using Procedure = std::string;
    using CallerProc = std::string;
    using CalleeProc = std::string;
    using ModifiesLine = std::string;
    using UsesLine = std::string;

    struct pairHash {
    public:
        inline std::size_t operator()(
            const std::pair<std::string, std::string>& v) const {
            std::hash<std::string> hash_str;
            size_t hash_val = 0;
            hash_val = hash_str(v.first) + 1;
            hash_val = hash_val ^ (hash_val >> 6);
            hash_val = hash_val * 31;
            hash_val = hash_val ^ (hash_val << 2);
            hash_val = hash_val + hash_str(v.second) + 1;
            hash_val = hash_val ^ (hash_val >> 7);
            hash_val = hash_val * 29;
            hash_val = hash_val ^ (hash_val << 1);

            return hash_val;
        }
    };

    void addToSetInMap(std::unordered_map<std::string, std::unordered_set<std::string>>& map,
        const std::string key, const std::string val);
    void addToSetInMap(std::unordered_map<std::string,
        std::unordered_set<std::pair<std::string, std::string>, pairHash>>&map,
        const std::string key, const std::pair<std::string, std::string> val);
}