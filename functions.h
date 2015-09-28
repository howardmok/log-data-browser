#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <deque>
#include <set>
#include <sstream>
#include <iostream>
#include <strings.h>

extern std::stringstream oss;

struct logs{
    std::string category, message;
    unsigned long time_stamp;
    unsigned int entryID;
    std::string printed;
};

class Sort_v{
public:
    bool operator()(const logs& log1, const unsigned long& timestamp2) const{
        if (log1.time_stamp < timestamp2){
            return true;
        }
        return false;
    }
    bool operator()(const logs& log1, const logs& log2) const{
        if (log1.time_stamp < log2.time_stamp){
            return true;
        }
        else if (log1.time_stamp == log2.time_stamp){
            std::string x, y;
            transform(log1.category.begin(), log1.category.end(), back_inserter(x), ::tolower);
            transform(log2.category.begin(), log2.category.end(), back_inserter(y), ::tolower);
            if (x.compare(y) < 0){
                return true;
            }
            else if (x.compare(y) == 0){//tie, switch to entry checking
                if (log1.entryID < log2.entryID){
                    return true;
                }
            }
        }
        return false;
    }
};

std::unordered_map<std::string, std::vector<int>> make_category(std::vector<logs>& master_log);

std::unordered_map<std::string, std::vector<int>> make_keywords(std::vector<logs>& master_log);

std::vector<int> timestamp_search2(std::vector<logs> master_log, unsigned long timestamp1, unsigned long timestamp2);

std::vector<int> category_search(std::unordered_map< std::string, std::vector<int> > categories, std::string category);

std::vector<int> keyword_search(std::unordered_map< std::string, std::vector<int> > messages, std::vector<std::string> keywords);

std::vector<std::string> split(const std::string &s);

#endif
