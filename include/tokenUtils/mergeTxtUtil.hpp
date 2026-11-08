#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "../algorithm/minheap.hpp"
#include <iostream>

struct inputNode {
    std::string val;
    inputNode *next;
    inputNode *prev;
};

std::vector<std::string> buildMergedText(std::string prompt, std::unordered_map<std::string, int>& mergeRule)
{
    // iterate through all char
    std::vector<std::string> keyHeap;

    int i = 0;
    int pointer = -1;
    std::string lowest = ""; 
    // first collect all key and order for all combination in whole string
    while (i < prompt.length() - 1)
    {
        std::string key = std::string(1, prompt[i]) + prompt[i + 1];
        int order = mergeRule[key];
        if (lowest == "") {
            lowest = key;
            pointer = i;
        } else {
            if (mergeRule[lowest] > mergeRule[key]) {
                lowest = key;
                pointer = i;
            }
        }
        // find way to check if the rule exist
        if (order != 0)
        {
            keyHeap.push_back(key);
        }
        i++;
    }

    // sort collected merge rule to minheap
    buildMinHeap(keyHeap, mergeRule);
    // x
    // iterate string til it found matching pattern with mergerule[0]
    // do the merge
    // check again the next matching string, if found do merge if not remove the first row


    // say N is index of recently merged text, get the order of n+1 and n-1 index from mergeRule
    // put that to min heap
    // go to x until keyHeap is empty

    // implement merge and slide
    // doMerge()

    return keyHeap;
}


void doMerge(){
    
}