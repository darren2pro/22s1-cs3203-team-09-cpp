#include "PKBStorage.h"
#include "../QueryProcessingSubsystem/Validator/SemanticException.h"

namespace PKB {
    RelationADT<std::string, std::string>* PKBStorage::getRelationFromEnum(Relation::Types type) {
        switch (type) {
        case Relation::ModifiesS:
            return &modifiesSRelations;
        case Relation::ModifiesP:
            return &modifiesPRelations;
        case Relation::UsesS:
            return &usesSRelations;
        case Relation::UsesP:
            return &usesPRelations;
        case Relation::Follows:
            return &followsRelations;
        case Relation::FollowsT:
            return &followsTRelations;
        case Relation::Parent:
            return &parentRelations;
        case Relation::ParentT:
            return &parentTRelations;
        case Relation::Calls:
            return &callsRelations;
        case Relation::CallsT:
            return &callsTRelations;
        case Relation::Next:
            return &nextRelations;
        case Relation::NextT:
            return &nextTRelationsCache;
        case Relation::Affects:
            return &affectRelationCache;
        case Relation::AffectsT:
            return &affectTRelationCache;
        default:
            return nullptr;
        }
    }

    EntityADT* PKBStorage::getEntityFromEnum(Declaration::DesignEntity entity) {
        switch (entity) {
        case Declaration::Variable:
            return &varEntity;
        case Declaration::Procedure:
            return &procEntity;
        case Declaration::Constant:
            return &constEntity;
        case Declaration::While:
            return &whileEntity;
        case Declaration::If:
            return &ifEntity;
        case Declaration::Assignment:
            return &assignEntity;
        case Declaration::Read:
            return &readEntity;
        case Declaration::Print:
            return &printEntity;
        case Declaration::Statement:
            return &stmtEntity;
        case Declaration::Call:
            return &callEntity;
        default:
            return nullptr;
        }
    }

    PatternADT* PKBStorage::getPatternFromEnum(Pattern::Types type) {
        switch (type) {
        case Pattern::Assign:
            return &assignPattern;
        case Pattern::If:
            return &ifPattern;
        case Pattern::While:
            return &whilePattern;
        default:
            return nullptr;
        }
    }

    RelationCacheADT<std::string, std::string>* PKBStorage::getCacheFromEnum(Relation::Types type) {
        switch (type) {
        case Relation::NextT:
            return &nextTRelationsCache;
        case Relation::Affects:
            return &affectRelationCache;
        case Relation::AffectsT:
            return &affectTRelationCache;
        default:
            return nullptr;
        }
    }

    PKBStorage::PKBStorage() {}

    PKBStorage::~PKBStorage() {}

    void PKBStorage::storeEntity(Declaration::DesignEntity type, const std::string value) {
        auto entity = getEntityFromEnum(type);
        if (type == Declaration::Procedure) {
            entity->addWithEnsureNoRepeatedKey(value);
        } else {
            entity->add(value);
        }
    }

    void PKBStorage::storeEntity(Declaration::DesignEntity type, const std::string first, const std::string second) {
        auto entity = getEntityFromEnum(type);
        entity->add(first, second);
    }

    void PKBStorage::storeRelations(Relation::Types type, std::string first, std::string second) {
        auto relation = getRelationFromEnum(type);
        if (type == Relation::Calls) {
            if (procEntity.contains(second)) {
                relation->add(first, second);
            } else {
                throw SemanticError("Calling a non-existent procedure.");
            }
        } else {
            relation->add(first, second);
        }
    }

    void PKBStorage::storePatterns(Pattern::Types type, const Variable var, const LineNum line, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        pattern->add(var, line, expr);
    }

    bool PKBStorage::entityContains(Declaration::DesignEntity type, const std::string value) {
        auto entity = getEntityFromEnum(type);
        return entity->contains(value);
    }

    std::unordered_set<std::string> PKBStorage::getEntitySet(Declaration::DesignEntity type) {
        auto entity = getEntityFromEnum(type);
        return entity->getSet();
    }

    std::string PKBStorage::getValueFromKey(std::string attrVal, Declaration::DesignEntity type, AttrReference::Attribute attr) {
        if (attr == AttrReference::ProcName || attr == AttrReference::VarName) {
            auto entity = getEntityFromEnum(type);
            return entity->get(attrVal);
        }
        else {
            // If any other attributes (stmt#), just return the original value.
            return attrVal;
        }
    }

    bool PKBStorage::relationContainsSet(Relation::Types type, const std::string first, const std::string second) {
        auto relation = getRelationFromEnum(type);
        return relation->containsSet(first, second);
    }

    bool PKBStorage::relationContainsFirst(Relation::Types type, const std::string first) {
        auto relation = getRelationFromEnum(type);
        return relation->containsFirst(first);
    }

    bool PKBStorage::relationContainsSecond(Relation::Types type, const std::string second) {
        auto relation = getRelationFromEnum(type);
        return relation->containsSecond(second);
    }

    bool PKBStorage::relationIsEmpty(Relation::Types type) {
        auto relation = getRelationFromEnum(type);
        return relation->isEmpty();
    }

    std::unordered_set<std::string> PKBStorage::getRelationSecondFromFirst(Relation::Types type, const std::string first) {
        auto relation = getRelationFromEnum(type);
        return relation->getSecondFromFirst(first);
    }

    std::unordered_set<std::string> PKBStorage::getRelationFirstFromSecond(Relation::Types type, const std::string second) {
        auto relation = getRelationFromEnum(type);
        return relation->getFirstFromSecond(second);
    }

    std::unordered_set<std::string> PKBStorage::getRelationAllSecond(Relation::Types type) {
        auto relation = getRelationFromEnum(type);
        return relation->getAllSecond();
    }

    std::unordered_set<std::string> PKBStorage::getRelationAllFirst(Relation::Types type) {
        auto relation = getRelationFromEnum(type);
        return relation->getAllFirst();
    }

    std::unordered_set<std::pair<std::string, std::string>, pairHash> PKBStorage::getRelationSet(Relation::Types type) {
        auto relation = getRelationFromEnum(type);
        return relation->getSet();
    }
    
    // Pattern functions
    std::unordered_set<LineNum> PKBStorage::getPatternLineByVar(Pattern::Types type, const Variable var) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineByVar(var);
    }

    std::unordered_set<LineNum> PKBStorage::getPatternLineByVarMatchFull(Pattern::Types type, const Variable var, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineByVarMatchFull(var, expr);
    }

    std::unordered_set<LineNum> PKBStorage::getPatternLineByVarMatchPartial(Pattern::Types type, const Variable var, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineByVarMatchPartial(var, expr);
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBStorage::getPatternLineVarSet(Pattern::Types type) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineVarSet();
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBStorage::getPatternLineVarByMatchFull(Pattern::Types type, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineVarByMatchFull(expr);
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBStorage::getPatternLineVarByMatchPartial(Pattern::Types type, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineVarByMatchPartial(expr);
    }

    std::unordered_set<LineNum> PKBStorage::getPatternLineByUS(Pattern::Types type) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineByUS();
    }

    std::unordered_set<LineNum> PKBStorage::getPatternLineByUSMatchFull(Pattern::Types type, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineByUSMatchFull(expr);
    }

    std::unordered_set<LineNum> PKBStorage::getPatternLineByUSMatchPartial(Pattern::Types type, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineByUSMatchPartial(expr);
    }

    bool PKBStorage::isCacheFullyComputed(Relation::Types type) {
        auto cache = getCacheFromEnum(type);
        return cache->isFullyComputed;
    }

    void PKBStorage::setCacheFullyComputed(Relation::Types type) {
        auto cache = getCacheFromEnum(type);
        cache->isFullyComputed = true;
    }

    void PKBStorage::storeCacheSet(Relation::Types type, const std::string first, const std::string second) {
        auto cache = getCacheFromEnum(type);
        cache->addToSet(first, second);
    }

    void PKBStorage::storeCacheFirstToSecondMap(Relation::Types type, const std::string first, const std::string second) {
        auto cache = getCacheFromEnum(type);
        cache->addToFirstToSecondMap(first, second);
    }

    void PKBStorage::storeCacheSecondToFirstMap(Relation::Types type, const std::string second, const std::string first) {
        auto cache = getCacheFromEnum(type);
        cache->addToSecondToFirstMap(second, first);
    }

    void PKBStorage::clearCache() {
        nextTRelationsCache.reset();
        affectRelationCache.reset();
        affectTRelationCache.reset();
    }
}
