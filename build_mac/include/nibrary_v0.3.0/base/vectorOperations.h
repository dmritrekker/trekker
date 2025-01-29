#pragma once

#include "base/nibr.h"
#include "base/verbose.h"
#include <algorithm>
#include <vector>
#include <cmath>

namespace NIBR 
{

    template<class T1,class T2>
    void removeIdx(std::vector<T1>& inp, const std::vector<T2>& rmIdx) {
        
        if (rmIdx.size()==0) {
            return;
        } else if (inp.size()==rmIdx.size()) {
            std::vector<T1>().swap(inp);
            return;
        } 

        std::vector<T2> idx = rmIdx;

        std::sort(idx.begin(),idx.end());

        auto   beg    = inp.begin();
        std::size_t offset = 0;

        for (auto it = idx.begin(); it < idx.end(); it++) {
            std::size_t next = (it + 1 == idx.cend() ? inp.size() : *(it + 1));
            std::move(beg+*it+1, beg+next, beg+*it-offset);
            offset++;
        }
        
        inp.resize(inp.size()-idx.size());

    }

    template<class T>
    bool isUnique(std::vector<T>& inp, int ind) {

        int n = 0;

        for (int i=0; i<int(inp.size()); i++) {
            if (inp[ind] == inp[i])
                n++;
        }

        if (n==1)
            return true;
        else
            return false;

    }

    template<typename T>
    std::vector<T> linspace(T start, T end, std::size_t N) {
        std::vector<T> result;

        if (N == 0) return result;

        if (N == 1) {
            result.push_back(start);
            return result;
        }

        T step = (end - start) / static_cast<T>(N - 1);

        for (std::size_t i = 0; i < N; ++i) {
            result.push_back(start + i * step);
        }

        return result;
    }


    template<typename T>
    std::vector<T> getEvenlySeparatedSamples(const std::vector<T>& vec, std::size_t M) {
        std::vector<T> samples;
        std::size_t N = vec.size();
        
        double step = double(N-1)/double(M-1);

        for (std::size_t i = 0; i < M; ++i) {
            std::size_t index = std::size_t(std::round(i * step));
            if (index < N) {
                samples.push_back(vec[index]);
            }
        }
        
        return samples;
    }

    template<typename T>
    std::size_t findClosestIndex(const std::vector<T>& vec, const T& query) {
        if (vec.empty()) {
            disp(MSG_ERROR,"Vector is empty. Cannot find closest element.");
            return std::numeric_limits<std::size_t>::max(); // return an invalid index
        }

        std::size_t closestIndex = 0;
        T minDifference = std::numeric_limits<T>::max();

        for (std::size_t i = 0; i < vec.size(); ++i) {
            T difference = std::abs(vec[i] - query);
            if (difference < minDifference) {
                minDifference = difference;
                closestIndex = i;
            }
        }

        return closestIndex;
    }

    template<typename T>
    std::size_t findFirstGreaterIndex(const std::vector<T>& vec, const T& query) {
        if (vec.empty()) {
            disp(MSG_ERROR, "Vector is empty. Cannot find element greater than input.");
            return std::numeric_limits<std::size_t>::max(); // return an invalid index
        }

        for (std::size_t i = 0; i < vec.size(); ++i) {
            if (vec[i] >= query) {
                return i; 
            }
        }

        return std::numeric_limits<std::size_t>::max();  // return an invalid index
    }


    template<typename T>
    std::size_t findFirstSmallerIndex(const std::vector<T>& vec, const T& query) {
        if (vec.empty()) {
            disp(MSG_ERROR, "Vector is empty. Cannot find element smaller than input.");
            return std::numeric_limits<std::size_t>::max(); // return an invalid index
        }

        for (std::size_t i = 0; i < vec.size(); ++i) {
            if (vec[i] <= query) {
                return i;
            }
        }

        return std::numeric_limits<std::size_t>::max();  // return an invalid index
    }


}
