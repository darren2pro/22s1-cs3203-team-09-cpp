#pragma once

#include <unordered_map>
#include <unordered_set>
#include "PKBUtils.h"

namespace PKB {
    class EntityADT {
    private:
        std::unordered_set<std::string> set;
        std::unordered_map<std::string, std::string> map;

    public:
        void add(const std::string);
        void add(const std::string, const std::string);
        bool contains(const std::string);
        std::unordered_set<std::string> getSet();
        std::string get(const std::string);
    };
}