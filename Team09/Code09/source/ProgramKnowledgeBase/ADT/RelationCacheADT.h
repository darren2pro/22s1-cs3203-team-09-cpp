#pragma once

#include <unordered_map>
#include <unordered_set>
#include "RelationADT.h"
#include "../PKBUtils.h"

namespace PKB {
    template <typename T, typename U>
    class RelationCacheADT : public RelationADT<T, U> {
    public:
        using RelationADT<T, U>::set;
        using RelationADT<T, U>::firstToSecondMap;
        using RelationADT<T, U>::secondToFirstMap;
        using RelationADT<T, U>::isEmpty;
        using RelationADT<T, U>::containsFirst;
        using RelationADT<T, U>::containsSecond;
        using RelationADT<T, U>::containsSet;
        using RelationADT<T, U>::getFirstFromSecond;
        using RelationADT<T, U>::getSecondFromFirst;
        using RelationADT<T, U>::getAllFirst;
        using RelationADT<T, U>::getAllSecond;
        using RelationADT<T, U>::getSet;

        bool isFullyComputed = false;

        void addToSet(const T, const U);
        void addToFirstToSecondMap(const T, const U);
        void addToSecondToFirstMap(const U, const T);
        void reset();
    };

    template <typename T, typename U>
    void RelationCacheADT<T, U>::addToSet(const T t, const U u) {
        set.insert(std::make_pair(t, u));
    }

    template <typename T, typename U>
    void RelationCacheADT<T, U>::addToFirstToSecondMap(const T t, const U u) {
        addToSetInMap(firstToSecondMap, t, u);
    }

    template <typename T, typename U>
    void RelationCacheADT<T, U>::addToSecondToFirstMap(const U u, const T t) {
        addToSetInMap(secondToFirstMap, u, t);
    }

    template <typename T, typename U>
    void RelationCacheADT<T, U>::reset() {
        set.clear();
        firstToSecondMap.clear();
        secondToFirstMap.clear();
        isFullyComputed = false;
    }
}