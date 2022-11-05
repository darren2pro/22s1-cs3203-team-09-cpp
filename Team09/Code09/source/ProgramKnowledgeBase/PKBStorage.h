#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "PKBUtils.h"
#include "ADT/PatternADT.h"
#include "ADT/RelationADT.h"
#include "ADT/RelationCacheADT.h"
#include "ADT/EntityADT.h"
#include "../SourceProcessor/TNode/TNode.h"
#include "../QueryProcessingSubsystem/Declaration.h"
#include "../QueryProcessingSubsystem/Relation.h"
#include "../QueryProcessingSubsystem/Pattern.h"
#include "../QueryProcessingSubsystem/AttrReference.h"


namespace PKB {
    class PKBStorage {
    private:
        RelationADT<std::string, std::string>* getRelationFromEnum(Relation::Types);
        EntityADT* getEntityFromEnum(Declaration::DesignEntity);
        PatternADT* getPatternFromEnum(Pattern::Types);
        RelationCacheADT<std::string, std::string>* getCacheFromEnum(Relation::Types);

        //entity set
        EntityADT varEntity;
        EntityADT procEntity;
        EntityADT constEntity;
        EntityADT whileEntity;
        EntityADT ifEntity;
        EntityADT assignEntity;
        EntityADT readEntity;
        EntityADT printEntity;
        EntityADT stmtEntity;
        EntityADT callEntity;
        
        //relations
        RelationADT<LineNum, Variable> modifiesSRelations;
        RelationADT<Procedure, Variable> modifiesPRelations;
        RelationADT<LineNum, Variable> usesSRelations;
        RelationADT<Procedure, Variable> usesPRelations;
        RelationADT<PrevLine, NextLine> followsRelations;
        RelationADT<PrevLine, NextLine> followsTRelations;
        RelationADT<ParentLine, ChildLine> parentRelations;
        RelationADT<ParentLine, ChildLine> parentTRelations;
        RelationADT<CallerProc, CalleeProc> callsRelations;
        RelationADT<CallerProc, CalleeProc> callsTRelations;
        RelationADT<PrevLine, NextLine> nextRelations;

        //relations cache
        RelationCacheADT<PrevLine, NextLine> nextTRelationsCache;
        RelationCacheADT<ModifiesLine, UsesLine> affectRelationCache;
        RelationCacheADT<ModifiesLine, UsesLine> affectTRelationCache;

        //pattern map
        PatternADT assignPattern;
        PatternADT ifPattern;
        PatternADT whilePattern;

    public:

        PKBStorage();
        ~PKBStorage();

        //store entities API
        void storeEntity(Declaration::DesignEntity entity, const std::string value);
        void storeEntity(Declaration::DesignEntity entity, const std::string first, const std::string second);

        //store relations API
        void storeRelations(Relation::Types type, const std::string first, const std::string second);

        //store patterns API
        void storePatterns(Pattern::Types type, const Variable, const LineNum, const ExprStr);

        //get entities API
        bool entityContains(Declaration::DesignEntity, const std::string);
        std::unordered_set<std::string> getEntitySet(Declaration::DesignEntity);
        std::string getValueFromKey(std::string synonymValue, Declaration::DesignEntity type, AttrReference::Attribute attr);

        //get relations API
        bool relationContainsSet(Relation::Types, const std::string, const std::string);
        bool relationContainsFirst(Relation::Types, const std::string);
        bool relationContainsSecond(Relation::Types, const std::string);
        bool relationIsEmpty(Relation::Types);
        std::unordered_set<std::string> getRelationSecondFromFirst(Relation::Types, const std::string);
        std::unordered_set<std::string> getRelationFirstFromSecond(Relation::Types, const std::string);
        std::unordered_set<std::string> getRelationAllSecond(Relation::Types);
        std::unordered_set<std::string> getRelationAllFirst(Relation::Types);
        std::unordered_set<std::pair<std::string, std::string>, pairHash> getRelationSet(Relation::Types);

        //get patterns API
        std::unordered_set<LineNum> getPatternLineByVar(Pattern::Types, const Variable);
        std::unordered_set<LineNum> getPatternLineByVarMatchFull(Pattern::Types, const Variable, const ExprStr);
        std::unordered_set<LineNum> getPatternLineByVarMatchPartial(Pattern::Types, const Variable, const ExprStr);
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> getPatternLineVarSet(Pattern::Types);
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> getPatternLineVarByMatchFull(Pattern::Types, const ExprStr);
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> getPatternLineVarByMatchPartial(Pattern::Types, const ExprStr);
        std::unordered_set<LineNum> getPatternLineByUS(Pattern::Types);
        std::unordered_set<LineNum> getPatternLineByUSMatchFull(Pattern::Types, const ExprStr);
        std::unordered_set<LineNum> getPatternLineByUSMatchPartial(Pattern::Types, const ExprStr);

        // cache API
        bool isCacheFullyComputed(Relation::Types);
        void setCacheFullyComputed(Relation::Types);
        void storeCacheSet(Relation::Types, const std::string, const std::string);
        void storeCacheFirstToSecondMap(Relation::Types, const std::string, const std::string);
        void storeCacheSecondToFirstMap(Relation::Types, const std::string, const std::string);
        void clearCache();
    };
}
