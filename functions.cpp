#include <iostream>
#include "functions.h"
using namespace std;

unordered_map<string, vector<int>> make_category(vector<logs>& master_log){
    unordered_map<string, vector<int>> category_entries;
    for (unsigned int i = 0; i < master_log.size(); ++i){
        string lower_cat;
        transform(master_log[i].category.begin(), master_log[i].category.end(), back_inserter(lower_cat), ::tolower);
        category_entries[lower_cat].push_back(i);
    }
    return category_entries;
}

unordered_map<string, vector<int>> make_keywords(vector<logs>& master_log){
    unordered_map<string, vector<int>> keyword_entries;
    for (unsigned int i = 0; i < master_log.size(); ++i){
        string lower_cat, lower_msg;
        transform(master_log[i].message.begin(), master_log[i].message.end(), back_inserter(lower_msg), ::tolower);
        transform(master_log[i].category.begin(), master_log[i].category.end(), back_inserter(lower_cat), ::tolower);
        string long_word = lower_cat + ' ' + lower_msg;
        for (unsigned int m = 0; m < long_word.size(); ++m){
            if (!isalnum(long_word[m])){
                long_word[m] = ' ';
            }
        }
        vector<string> msg_parts = split(long_word);
        for (unsigned int l = 0; l < msg_parts.size(); ++l){
            if (keyword_entries[msg_parts[l]].empty() || keyword_entries[msg_parts[l]].back() != i)
                keyword_entries[msg_parts[l]].push_back(i);
        }
    }
    return keyword_entries;
}

vector<int> timestamp_search2(vector<logs> master_log, unsigned long timestamp1, unsigned long timestamp2){
    vector<int> timestamp_entries;
    vector<logs>::iterator ab = lower_bound(master_log.begin(), master_log.end(), timestamp1, Sort_v());
    int a = static_cast<int>(ab - master_log.begin());
    vector<logs>::iterator it = lower_bound(master_log.begin(), master_log.end(), timestamp2, Sort_v());
    int b = static_cast<int>(it - master_log.begin());
    for (int i = a; i < b; ++i){
        timestamp_entries.push_back(i);
    }
    oss << timestamp_entries.size() << "\n";
    return timestamp_entries;
}

vector<int> category_search(unordered_map<string, vector<int>> categories, string category){
    vector<int> category_entries;
    transform(category.begin(), category.end(), category.begin(), ::tolower);
    category_entries = categories[category];
    oss << category_entries.size() << "\n";
    return category_entries;
}

vector<int> keyword_search(unordered_map<std::string, vector<int>> messages, vector<string> keywords){
    vector<int> keyword_entries;
    for (unsigned int i = 0; i < keywords.size(); ++i){
        if(keyword_entries.empty())
            keyword_entries = messages[keywords[i]];
        else {
            vector<int> intersection;
            set_intersection(keyword_entries.begin(), keyword_entries.end(), messages[keywords[i]].begin(), messages[keywords[i]].end(), back_inserter(intersection));
            keyword_entries = move(intersection);
        }
    }
    oss << keyword_entries.size() << "\n";
    return keyword_entries;
}

vector<string> split(const string &s) {
    vector<string> elems;
    stringstream ss(s);
    string keyword;
    while(ss >> keyword)
        elems.push_back(keyword);
    return elems;
}
