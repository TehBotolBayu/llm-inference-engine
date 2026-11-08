#pragma once
#include <utility>
#include <vector>
#include <iostream>

// INSERT(heap, value):
void insertMinHeap(std::vector<std::string>& heap, std::string value, std::unordered_map<std::string, int>& mergeRule)
{
    // append value to heap
    int i = heap.size();
    heap.push_back(value);

    while (i > 0) { 
        int parent = (i - 1) / 2;
        if (mergeRule.at(heap[parent]) <= mergeRule.at(heap[i])) {
            break;
        }
        std::swap(heap[parent], heap[i]);
        i = parent;
    }
}


void siftDown(
    std::vector<std::string>& heap,
    int i,
    std::unordered_map<std::string, int>& mergeRule
) {
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int small = i;

        if (left < heap.size() &&
            mergeRule.at(heap[left]) < mergeRule.at(heap[small])) {
            small = left;
        }

        if (right < heap.size() &&
            mergeRule.at(heap[right]) < mergeRule.at(heap[small])) {
            small = right;
        }

        if (small == i) {
            break;
        }

        std::swap(heap[i], heap[small]);
        i = small;
    }
}

void buildMinHeap (std::vector<std::string>& heap, std::unordered_map<std::string, int>& mergeRule) {
    int i = (heap.size() / 2) - 1;
    while (i >= 0) {
        siftDown(heap, i, mergeRule);
        --i;
    }
}
    
 