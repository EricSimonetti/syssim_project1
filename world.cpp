//
// Created by erics on 9/2/2021.
//

#include <iostream>
#include <sstream>
using namespace std;
#include <string>
using std::string;
#include "creature.cpp"



class room{
private:
    int index, indexNorth = -1, indexSouth = -1, indexEast = -1, indexWest = -1;
    int cleanliness; // 0 for clean, 1 for half-dirty, 2 for dirty
    int numCreatures = 0;
    room *north, *south, *east, *west;
    PC *pc;
    creature *creatures[10];
public:
    room(int index, int cleanliness, int indexNorth, int indexSouth, int indexEast, int indexWest){
        this -> index = index;
        this -> cleanliness = cleanliness;
        this -> indexNorth = indexNorth;
        this -> indexSouth = indexSouth;
        this -> indexEast = indexEast;
        this -> indexWest = indexWest;
    }

    ~room(){
        for(int i = 0; i<numCreatures; i++){ //remove creature from array of creatures
            delete creatures[i];
        }
    }

    int getNorth(){ return indexNorth; }
    int getSouth(){ return indexSouth; }
    int getEast(){ return indexEast; }
    int getWest(){ return indexWest; }

    void setRooms(room *north, room *south, room *east, room *west){
        this -> north = north;
        this -> south = south;
        this -> east = east;
        this -> west = west;
    }

    string description(){
        string desc = "Room " + std::to_string(index);

        switch(cleanliness){
            case(0): desc += ", clean,"; break;
            case(1): desc += ", half-dirty,"; break;
            case(2): desc += ", dirty,"; break;
        }

        desc += " neighbors ";
        if(indexNorth != -1) desc += std::to_string(indexNorth) + " to the North ";
        if(indexSouth != -1) desc += std::to_string(indexSouth) + " to the South ";
        if(indexEast != -1) desc += std::to_string(indexEast) + " to the East ";
        if(indexWest != -1) desc += std::to_string(indexWest) + " to the West ";
        desc += ", contains: \n";

        for(int i = 0; i<numCreatures; i++){
            desc += creatures[i] -> description() + "\n";
        }
        desc += "PC";

        return desc;
    }

    string addCreature(creature *creature){
        if(numCreatures<10){
            creatures[numCreatures] = creature;
            numCreatures++;
            creatureAction(creature -> getType());
            return "success";
        }
        return "Room full";
    }

    string addPC(PC *pc){
        if(numCreatures<10) {
            this->pc = pc;
            return "success";
        }
        return "Room full";
    }

    string moveCreature(creature *creature, int direction){ //0 is north, 1 is south, 2 is east, 3 is west, -1 is random
        string success;
        bool isPC = creature->getType() == 0;
        if(direction == -1){ //random rooms until entry
            // Fill vector with room pointers
            int listSize = 0;
            if(indexNorth != -1) listSize++;
            if(indexSouth != -1) listSize++;
            if(indexEast != -1) listSize++;
            if(indexWest != -1) listSize++;
            if(listSize == 0){
                return "delete me";
            }

            std::vector<room*> roomList(listSize);
            int curr = 0;
            if(indexNorth != -1) roomList[curr++] = north;
            if(indexSouth != -1) roomList[curr++] = south;
            if(indexEast != -1) roomList[curr++] = east;
            if(indexWest != -1) roomList[curr] = west;

            std::random_device rd;
            std::mt19937 prng(rd());

            std::shuffle(roomList.begin(), roomList.end(), prng);
            int listIndex = 0;
            success = (isPC?
                          (roomList[listIndex] -> addPC(static_cast<PC *>(creature))) :
                          (roomList[listIndex] -> addCreature(creature))
                      );
            while(success == "Room full"){
                if (listIndex == listSize-1) { //searched all the rooms and couldn't move
                    return "delete me";
                }
                listIndex++;
                success = (isPC?
                             (roomList[listIndex] -> addPC(static_cast<PC *>(creature))) :
                             (roomList[listIndex] -> addCreature(creature))
                          );
            }
            if(roomList[listIndex] == north) success = "north";
            if(roomList[listIndex] == south) success = "south";
            if(roomList[listIndex] == east) success = "east";
            if(roomList[listIndex] == west) success = "west";
            if(isPC){
                pc = nullptr;
            }
            else{
                (removeCreature(creature, false));
            }
        }
        else{
            switch (direction) {
                case (0):
                    if (indexNorth != -1){
                        success = isPC ? (north->addPC(static_cast<PC *>(creature))) : (north->addCreature(creature));
                        if(success != "Room full") success = "north";
                    }
                    else
                        return "There is no room to the north";
                    break;
                case (1):
                    if (indexSouth != -1){
                        success = isPC ? (south->addPC(static_cast<PC *>(creature))) : (south->addCreature(creature));
                        if(success != "Room full") success = "south";
                    }
                    else
                        return "There is no room to the south";
                    break;
                case (2):
                    if (indexEast != -1){
                        success = isPC ? (east->addPC(static_cast<PC *>(creature))) : (east->addCreature(creature));
                        if(success != "Room full") success = "east";
                    }
                    else
                        return "There is no room to the east";
                    break;
                case (3):
                    if (indexWest != -1){
                        success = isPC ? (west->addPC(static_cast<PC *>(creature))) : (west->addCreature(creature));
                        if(success != "Room full") success = "west";
                    }
                    else
                        return "There is no room to the west";
                    break;
                default:
                    return "No such room";
            }
            if (success != "Room full" && (creature->getType() == 0)) {
                pc = nullptr;
                return "success";
            }
            if (success != "Room full") { //if creature is able to move
                removeCreature(creature, false);
                return "success";
            }
        }
        return success;
    }

    void removeCreature(creature *creature, bool destroy){
        bool found = false;
        for (int i = 0; i < numCreatures; i++) { //remove creature from array of creatures
            if (found) {
                creatures[i - 1] = creatures[i];
            } else if (creatures[i] == creature) {
                found = true;
            }
        }
        numCreatures--;
        if(destroy){
            delete creature;
        }
    }

    //this version of the function is reserved for when creatures move to another room and auto clean/dirty that room
    void creatureAction(int type){
        if(type == 1 && cleanliness == 2){
            cleanliness--;
        }
        else if(type == 2 && cleanliness == 0){
            cleanliness++;
        }
    }


    string playerAction(bool action, int creatureIndex){ //0 is clean, 1 is dirty
        string response;
        if(!action && cleanliness == 0){
            return "Room is already clean";
        }
        if(action && cleanliness == 2){
            return "Room is already dirty";
        }

        creature *active;
        bool found = false;
        if(creatureIndex != -1){ //looks for the creature made to do the action
            for(int i = 0; i<numCreatures; i++){
                if(creatures[i] -> getIndex() == creatureIndex){
                    active = creatures[i];
                    found = true;
                }
                if(!found){
                    return "There is no such creature in this room";
                }
            }
        }

        action? cleanliness++ : cleanliness--; //actually changes the state

        for(int i = 0; i<numCreatures; i++){ //creatures reaction
            string reaction = creatures[i] -> react(action, false);
            if(creatures[i] == active){
                reaction += " a lot";
                pc -> gainRespect(true, reaction);
                if(checkRespect()!="ok"){
                    reaction += ". Your respect is now " + std::to_string(pc -> getRespect()) + ".\n";
                    return response + reaction + checkRespect();
                }
            }
            else{
                pc -> gainRespect(false, reaction);
                if(checkRespect()!="ok"){
                    reaction += ". Your respect is now " + std::to_string(pc -> getRespect()) + ".\n";
                    return response + reaction + checkRespect();
                }
            }

            string moveState;
            //if the creature wants to move
            if(creatures[i] -> getType() == 1 && cleanliness == 2 || creatures[i] -> getType() == 2 && cleanliness == 0){
                moveState = moveCreature(creatures[i], -1);
                //if the creature cant find a room to move to
                if(moveState == "delete me"){
                    reaction += " and leaves through the roof into the void";
                    reaction += ". Your respect is now " + std::to_string(pc -> getRespect()) + ".\n";
                    removeCreature(creatures[i], true);

                    //creatures reaction to the current creature leaving through the roof
                    string currReaction = "";
                    for(int j = 0; j<numCreatures; j++){
                        currReaction = creatures[j] -> react(action, true);
                        reaction += currReaction;
                        pc -> gainRespect(false, currReaction);
                        reaction += ". Your respect is now " + std::to_string(pc -> getRespect()) + ".\n";
                        if(checkRespect()!="ok"){
                            return response + reaction + checkRespect();
                        }
                    }
                }
                //if the creature has moved
                if(moveState == "north" || moveState == "south" || moveState == "east" || moveState == "west"){
                    reaction += " and leaves toward the " + moveState;
                    reaction += ". Your respect is now " + std::to_string(pc -> getRespect()) + ".\n";
                }
                i--; //this is here because all of the creatures get shifted over one,
                     // and the number of creatures in the room decreases, after a creature leaves
            }
            else {
                reaction += ". Your respect is now " + std::to_string(pc->getRespect()) + ".\n";
            }
            response += reaction;

        }
        return response.substr(0, response.length()-1); //chops off the final \n
    }

    string checkRespect(){
        if(pc -> getRespect() < 1){
            return "You loose big time!";
        }
        if(pc -> getRespect() > 80){
            return "You are the world champion! You Win!!!!!!!";
        }
        return "ok";
    }
};