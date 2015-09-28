#include <iostream>
#include <fstream>
#include <getopt.h>
#include <locale>
#include "functions.h"
#include <stdlib.h>
#include <sstream>

using namespace std;

stringstream oss;

int main(int argc, char* argv[]) {
    
//    ostream& out = cout;
    ostream& out = oss;
    string option;
    
    struct option longOpts[] = {
        { "help", no_argument, NULL, 'h' },
    };
    opterr = false;
    
    int opt = 0, index = 0;
    while ((opt = getopt_long(argc, argv, "h", longOpts, &index)) != -1) {
        switch (opt) {
            case 'h':
                out << "I guess I can help :)";
                exit(0);
            default:
                return 0;
        }
    }
    
    vector<logs> master_log;
    vector<long> timestamp_entries;
    unordered_map<string, vector<int>> category_entries;
    unordered_map<string, vector<int>> keyword_entries;
    fstream fs;
    
    int month, day, hour, minute, second;
    unsigned int num_entries = 0;
    unsigned long timestamp1, timestamp2;
    char colon, bar;
    string timestamp3;
    string category, message;
    logs entry;
    
    fs.open(argv[1]);//open file specified by command line
    if (fs.is_open()){
        while(getline(fs, timestamp3, '|')){
            string s = timestamp3;
            for(char& c : timestamp3) {
                if(c == ':') c = ' ';
            }
            vector<string> vals = split(timestamp3);
            month = atoi(vals[0].c_str());
            day = atoi(vals[1].c_str());
            hour = atoi(vals[2].c_str());
            minute = atoi(vals[3].c_str());
            second = atoi(vals[4].c_str());
            getline(fs, category, '|');//read category
            getline(fs, message);//read message
            entry.time_stamp = ((long long)month*100000000) + (day*1000000) + (hour*10000) + (minute*100) + second;
            entry.category = category;
            entry.message = message;
            entry.entryID = num_entries++;
            entry.printed = s;
            master_log.push_back(entry);//add to master_log
        }
    }
    else
        return 1;
    fs.close();
    
    out << num_entries << " entries loaded\n";
    sort(master_log.begin(), master_log.end(), Sort_v());
    
    vector<int> timestamp_index;
    vector<int> category_index;
    vector<int> keyword_index;
    vector<int> most_recent;
    
    deque<logs> excerpt_list3;
    bool cat_create = false;
    bool key_create = false;
    bool sorted = false;
    
    char command;
    out << '%';
        while (cin >> command){//read in commands
            if (command == 't'){//timestamp search
                cin >> month >> colon >> day >> colon >> hour >> colon >> minute >> colon >> second >> bar;
                timestamp1 = ((long long)month*100000000) + (day*1000000) + (hour*10000) + (minute*100) + second;
                cin >> month >> colon >> day >> colon >> hour >> colon >> minute >> colon >> second;
                timestamp2 = ((long long)month*100000000) + (day*1000000) + (hour*10000) + (minute*100) + second;
                timestamp_index = timestamp_search2(master_log, timestamp1, timestamp2);
                most_recent = timestamp_index;
            }
            else if (command == 'c'){//category search
                if (!cat_create){
                    category_entries = make_category(master_log);
                }
                getline(cin, category);
                if (category[0] == ' '){
                    category.erase(0, 1);
                }
                category_index = category_search(category_entries, category);
                most_recent = category_index;
                cat_create = true;
            }
            else if (command == 'k'){//keyword search
                if (!key_create){
                    keyword_entries = make_keywords(master_log);
                }
                string keyword;
                getline(cin, keyword);
                keyword.erase(0, 1);
                transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
                for (unsigned int m = 0; m < keyword.size(); ++m){
                    if (!isalnum(keyword[m])){
                        keyword[m] = ' ';
                    }
                }
                vector<string> keywords = split(keyword);
                keyword_index = keyword_search(keyword_entries, keywords);
                most_recent = keyword_index;
                key_create = true;
            }
            else if (command == 'i'){//insert to excerpt
                cin >> num_entries;
                for (unsigned int i = 0; i < master_log.size(); ++i){
                    if (master_log[i].entryID == num_entries){
                        excerpt_list3.push_back(master_log[i]);
                        break;
                    }
                }
                sorted = false;
            }
            else if (command == 'r'){//insert search results
                deque<logs> x;
                for (unsigned int i = 0; i < most_recent.size(); ++i){
                    x.push_back(master_log[most_recent[i]]);
                    excerpt_list3.push_back(x[i]);
                }
                sorted = false;
            }
            else if (command == 'd'){//delete logs entry
                cin >> num_entries;
                if (excerpt_list3.size() > num_entries){
                    excerpt_list3.erase(excerpt_list3.begin() + num_entries);
                }
            }
            else if (command == 'b'){//move to beginning
                cin >> num_entries;
                if (excerpt_list3.size() > num_entries){
                    logs x = excerpt_list3[num_entries];
                    excerpt_list3.erase(excerpt_list3.begin() + num_entries);
                    excerpt_list3.push_front(x);
                }
                sorted = false;
            }
            else if (command == 'e'){//move to end
                cin >> num_entries;
                if (excerpt_list3.size() > num_entries){
                    logs x = excerpt_list3[num_entries];
                    excerpt_list3.erase(excerpt_list3.begin() + num_entries);
                    excerpt_list3.push_back(x);
                }
                sorted = false;
            }
            else if (command == 's'){//sort excerpt list
                if (!sorted){
                    sort(excerpt_list3.begin(), excerpt_list3.end(), Sort_v());
                    sorted = true;
                }
            }
            else if (command == 'l'){//clear excerpt list
                excerpt_list3.clear();
                sorted = true;
            }
            else if (command == 'g'){//print most recent search results
                for (unsigned int i = 0; i < most_recent.size(); ++i){
                    auto& entry = master_log[most_recent[i]];
                    out << entry.entryID << '|' << entry.printed << '|' << entry.category << '|' << entry.message << '\n';
                }
            }
            else if (command == 'p'){//print excerpt list
                for (unsigned int i = 0; i < excerpt_list3.size(); ++i){
                    out << i << '|' << excerpt_list3[i].entryID << '|' << excerpt_list3[i].printed << '|' << excerpt_list3[i].category << '|' << excerpt_list3[i].message << '\n';
                }
            }
            else if (command == 'q'){//quit
                cout << oss.str() << endl;
                return 0;
            }
            out << '%';
        }
    return 0;
}
