#pragma once
#include <set>

enum ACTION_TYPE { RECORD, EDIT, PLAYBACK };
enum DISPLAY_TYPE {RECORD_WIND, EDIT_WIND, PLAYBACK_WIND, SETTINGS_WIND, HOME_WIND};

struct USER {
private:
        std::string name;
        std::set<ACTION_TYPE> actions;
        std::string description;

public:
    USER(){}
    USER(std::string in_name, std::string in_desc) {
        name = in_name;
        description = in_desc;
    }
    const char* getName() {return name.c_str();}
    void addAction(ACTION_TYPE a) {actions.insert(a);}
    bool hasAction(ACTION_TYPE a) {return actions.find(a) != actions.end();}
    std::set<ACTION_TYPE> getActions() {return actions;}
    const char* getDescription() {return description.c_str();}
};
