#pragma once

#include <list>
#include <string>
#include <algorithm>
#include "Mutex.h"
#include "Locker.h"

using std::list;
using std::string;

//orderedlist in increase order, which is thread safe
template <class T>
class orderedList{
    list<T> alist;
    ReadWriteMutex rwmu;

public:
    bool find(T t){
        ReadLocker locker(&rwmu);
        auto iter = std::find(alist.begin(), alist.end(), t);
        return (iter != alist.end()) ? true : false;
    }
    void push(T t){
        WriteLocker locker(&rwmu);
        alist.insert(std::upper_bound(alist.begin(), alist.end(), t), t);
    }
    bool pop(T t){
        WriteLocker locker(&rwmu);
        auto iter = std::find(alist.begin(), alist.end(), t);
        if (iter != alist.end()) {
            alist.erase(iter);
            return true;
        } else {
            return false;
        }
    }
    string toString(){
        string res = "head";
        ReadLocker locker(&rwmu);
        for (auto iter: alist) {
            res += "->" + std::to_string(iter);
        }
        res += "\n";
        return res;
    }
};
