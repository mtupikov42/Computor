#pragma once

#include <algorithm>
#include <cassert>

namespace Utils {

namespace {

template<class T>
const T& Min(const T& arg) {
    return arg;
}

template<class T>
const T& Max(const T& arg) {
    return arg;
}

}

template<typename T>
bool IsInRange(const T& val, const T& from, const T& to) {
    return (from <= val) && (val <= to);
}

template<typename T>
bool IsInRange(const T& val, const std::pair<const T&, const T&>& minmax) {
    return (minmax.first <= val) && (val <= minmax.second);
}

template<typename T>
bool IsInsideRange(const T& val, const T& from, const T& to) {
    return (from < val) && (val < to);
}

template<typename T>
bool IsInsideRange(const T& val, const std::pair<const T&, const T&>& minmax) {
    return (minmax.first < val) && (val < minmax.second);
}

template<class T, class U = T>
bool Modify(T& obj, U&& new_value) {
    if (obj == new_value) {
        return false;
    }
    obj = std::forward<U>(new_value);
    return true;
}

template <typename T>
constexpr const T& Clamp(const T& v, const T& lo, const T& hi) {
    assert(lo <= hi);
    return std::min(std::max(v, lo), hi);
}

template<class Collection, class T>
void RemoveAll(Collection& c, const T& value) {
	if (Find(c, value) != c.end()) {
		c.erase(std::remove(std::begin(c), std::end(c), value));
	}
}

template<class Collection, class Pred>
void RemoveAllIf(Collection& c, Pred&& predicate) {
    c.erase(std::remove_if(std::begin(c), std::end(c), std::forward<Pred>(predicate)), std::end(c));
}

template<class Collection, class T>
auto Find(Collection& c, const T& value) {
    return std::find(std::begin(c), std::end(c), value);
}

template<class Collection, class Pred>
auto FindIf(Collection& c, Pred&& predicate) {
    return std::find_if(std::begin(c), std::end(c), std::forward<Pred>(predicate));
}

template<class Collection, class T>
bool Contains(const Collection& c, const T& value) {
    return std::find(std::begin(c), std::end(c), value) != std::end(c);
}

template<class Collection, class Pred>
bool ContainsIf(const Collection& c, Pred&& predicate) {
    return FindIf(c, predicate) != std::end(c);
}

template<class Collection, class Pred>
int CountIf(const Collection& c, Pred&& predicate) {
    return std::count_if(std::begin(c), std::end(c), std::forward<Pred>(predicate));
}

template<class T, class... Args>
const auto& Min(const T& arg, const Args&... args) {
    const auto& tail_min = Min(args...);
    return std::min(arg, tail_min);
}

template<class T, class... Args>
const auto& Max(const T& arg, const Args&... args) {
    const auto& tail_max = Max(args...);
    return std::max(arg, tail_max);
}

template<class Collection>
auto MaxElement(const Collection& c) {
    return std::max_element(std::begin(c), std::end(c));
}

template<class Collection>
auto MinElement(const Collection& c) {
    return std::min_element(std::begin(c), std::end(c));
}

template<class Collection, class Comp>
auto MaxElement(const Collection& c, Comp&& comparator) {
    return std::max_element(std::begin(c), std::end(c), std::forward<Comp>(comparator));
}

template<class Collection, class Comp>
auto MinElement(const Collection& c, Comp&& comparator) {
    return std::min_element(std::begin(c), std::end(c), std::forward<Comp>(comparator));
}

template<class Collection>
void Sort(Collection& c) {
    std::sort(std::begin(c), std::end(c));
}

template<class Collection, class Comp>
void Sort(Collection& c, Comp&& comparator) {
    std::sort(std::begin(c), std::end(c), std::forward<Comp>(comparator));
}

template<class Collection>
void Unique(Collection& c) {
    c.erase(std::unique(std::begin(c), std::end(c)), std::end(c));
}

template<class Collection, class Pred>
void Unique(Collection& c, Pred&& predicate) {
    c.erase(std::unique(std::begin(c), std::end(c), std::forward<Pred>(predicate)), std::end(c));
}

template<class Collection, class Pred>
void ForEach(Collection& c, Pred&& predicate) {
    std::for_each(std::begin(c), std::end(c), std::forward<Pred>(predicate));
}

template<class Collection, class T>
int IndexOf(const Collection& c, const T& value) {
    const auto it = std::find(c.begin(), c.end(), value);
    return (it != c.end()) ? std::distance(c.begin(), it) : -1;
}

template<class Collection, class Pred>
int IndexOfIf(const Collection& c, Pred&& predicate) {
    const auto it = std::find_if(std::begin(c), std::end(c), std::forward<Pred>(predicate));
    return (it != c.end()) ? std::distance(c.begin(), it) : -1;
}

}
