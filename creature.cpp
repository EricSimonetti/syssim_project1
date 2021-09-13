//
// Created by erics on 9/12/2021.
//

#include <iostream>
#include <sstream>
using namespace std;
#include <vector>
#include <random>
#include <algorithm>
#include <string>

using std::string;

class creature{
protected:
    int index;
    int type; //0 for the PC, 1 for an animal, 2 for an NPC
    creature() = default;
public :
    virtual string react(bool action, bool roof){}
    virtual string description(){}

    int getIndex(){
        return index;
    }

    int getType(){
        return type;
    }
};

class PC : public creature {
private:
    int respect = 40;
public:
    PC(int index, int type) {
        this -> index = index;
        this -> type = type;
    }
    void gainRespect(bool alot, string reaction){
        int deltaRespect = 0;
        if(alot){
            deltaRespect+=2;
        }
        deltaRespect++;

        if(reaction.find("growls") != std::string::npos ||
           reaction.find("grumbles") != std::string::npos){
            respect -= deltaRespect;
        }
        else{
            respect += deltaRespect;
        }
    }

    int getRespect(){
        return respect;
    }

    string description() override{
        return "PC";
    }

};

class NPC : public creature{
public:
    NPC(int index, int type) {
        this -> index = index;
        this -> type = type;
    }
    string react(bool action, bool roof) override{ //0 is clean and 1 is dirty (action)
        if(roof) return std::to_string(index) + " grumbles";
        return std::to_string(index) + (action?" smiles":" grumbles");
    }

    string description() override{
        return "human " + std::to_string(index);
    }
};

class animal : public creature{
public:
    animal(int index, int type) {
        this -> index = index;
        this -> type = type;
    }
    string react(bool action, bool roof) override{ //0 is clean and 1 is dirty (action
        if(roof) return std::to_string(index) + " growls";
        return std::to_string(index) + (action?" growls":" licks your face");
    }

    string description() override{
        return "animal " + std::to_string(index);
    }
};
