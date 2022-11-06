#include "EntityADT.h"
#include "../../QueryProcessingSubsystem/Validator/SemanticException.h"

namespace PKB {
    void EntityADT::add(const std::string key) {
        set.insert(key);
    }

    void EntityADT::add(const std::string key, const std::string value) {
        set.insert(key);
        map[key] = value;
    }

    void EntityADT::addWithEnsureNoRepeatedKey(const std::string key) {
        if (set.find(key) != set.end()) {
            throw SemanticError("Repeated key found in EntityADT");
        }
        set.insert(key);
    }

    bool EntityADT::contains(const std::string key) {
        return set.find(key) != set.end();
    }

    std::unordered_set<std::string> EntityADT::getSet() {
        return set;
    }

    std::string EntityADT::get(const std::string key) {
        if (map.find(key) != map.end()) {
            return map.at(key);
        } else {
            return std::string();
        }
    }
}