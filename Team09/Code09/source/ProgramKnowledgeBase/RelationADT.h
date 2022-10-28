#pragma once

#include <unordered_map>
#include <unordered_set>
#include "PKBUtils.h"

namespace PKB {
    template <typename T, typename U>
    class RelationADT {
    public:
        std::unordered_set<std::pair<T, U>, pairHash> set;
        std::unordered_map<T, std::unordered_set<U>> firstToSecondMap;
        std::unordered_map<U, std::unordered_set<T>> secondToFirstMap;

        void add(const T, const U);
        bool isEmpty();
        bool containsFirst(const T);
        bool containsSecond(const U);
        bool containsSet(const T, const U);
        std::unordered_set<U> getSecondFromFirst(const T);
        std::unordered_set<T> getFirstFromSecond(const U);
        std::unordered_set<U> getAllSecond();
        std::unordered_set<T> getAllFirst();
        std::unordered_set<std::pair<T, U>, pairHash> getSet();
    };

    template <typename T, typename U>
     void RelationADT<T, U>::add(const T t, const U u) {
        std::pair<T, U> pair = std::make_pair(t, u);
        set.insert(pair);
        addToSetInMap(firstToSecondMap, t, u);
        addToSetInMap(secondToFirstMap, u, t);
    }
    
     template <typename T, typename U>
     bool RelationADT<T, U>::isEmpty() {
         return set.empty();
     }

     template <typename T, typename U>
     bool RelationADT<T, U>::containsFirst(const T t) {
         return firstToSecondMap.find(t) != firstToSecondMap.end();
     }

     template <typename T, typename U>
     bool RelationADT<T, U>::containsSecond(const U u) {
         return secondToFirstMap.find(u) != secondToFirstMap.end();
     }

     template <typename T, typename U>
     bool RelationADT<T, U>::containsSet(const T t, const U u) {
         return set.find(std::make_pair(t, u)) != set.end();
     }

     template <typename T, typename U>
     std::unordered_set<U> RelationADT<T, U>::getSecondFromFirst(const T t) {
         if (containsFirst(t)) {
             return firstToSecondMap.at(t);
         } else {
             return std::unordered_set<U>(); 
         }
     }

     template <typename T, typename U>
     std::unordered_set<T> RelationADT<T, U>::getFirstFromSecond(const U u) {
         if (containsSecond(u)) {
             return secondToFirstMap.at(u);
         } else {
             return std::unordered_set<T>();
         }
     }

     template <typename T, typename U>
     std::unordered_set<U> RelationADT<T, U>::getAllSecond() {
         std::unordered_set<U> result;
         for (const auto& elem : secondToFirstMap) {
             result.insert(elem.first);
         }
         return result;
     }

     template <typename T, typename U>
     std::unordered_set<T> RelationADT<T, U>::getAllFirst() {
         std::unordered_set<T> result;
         for (const auto& elem : firstToSecondMap) {
             result.insert(elem.first);
         }
         return result;
     }

     template <typename T, typename U>
     std::unordered_set<std::pair<T, U>, pairHash> RelationADT<T, U>::getSet() {
         return set;
     }
}
