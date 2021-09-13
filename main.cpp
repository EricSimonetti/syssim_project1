#include <iostream>
#include <sstream>
#include "world.cpp"

using namespace std;

int* separateInput(string input, int arr[]){
    istringstream stringStream(input); // split string at spaces
    string intAsString; // for storing each number

    // Traverse through all numbers
    for(int i = 0; i<5; i++){
        stringStream >> intAsString;
        arr[i] = stoi(intAsString);
    }
    return arr;
}

int main() {
    cout << "Please input world information:" << endl;
    string input_line;
    getline(cin, input_line);
    int numRooms = stoi(input_line); //get number of rooms
    if(numRooms < 1 || numRooms > 100){
        cout << "Please enter a valid number of rooms";
        return 0;
    }
    room *rooms[numRooms];
    PC *pc;
    int pcRoomIndex = 0;

    for(int i = 0; i<numRooms; i++){ //process room input
        getline(cin, input_line);
        int arr[5];
        int *inputs = separateInput(input_line, arr);
        rooms[i] = new room(i, inputs[0], inputs[1], inputs[2], inputs[3], inputs[4]);
    }

    //initialize rooms
    for(int i = 0; i<numRooms; i++){
        rooms[i] -> setRooms(rooms[rooms[i] -> getNorth()],
                             rooms[rooms[i] -> getSouth()],
                             rooms[rooms[i] -> getEast()],
                             rooms[rooms[i] -> getWest()]);
    }

    getline(cin, input_line);
    int numCreatures = stoi(input_line);//get number of creatures
    if(numCreatures < 1 || numCreatures > 100){
        cout << "Please enter a valid number of creatures";
        return 0;
    }

    for(int i = 0; i<numCreatures; i++){ //process creatures
        getline(cin, input_line);
        int arr[2];
        int* inputs = separateInput(input_line, arr);
        if(inputs[0] == 0){
            pc = new PC(i, 0);
            pcRoomIndex = inputs[1];
            rooms[inputs[1]] -> addPC(pc);
        }
        else if(inputs[0] == 1){
            rooms[inputs[1]] -> addCreature(new animal(i, 1));
        }
        else if(inputs[0] == 2){
            rooms[inputs[1]] -> addCreature(new NPC(i, 2));
        }
    }
    cout << "World initialized!\n" << endl;

    if(pc == nullptr){
        cout << "error ):" << endl;
    }
    else {
        cout << "For a list of commands, type \"help\"" << endl;
        cout << "Note: commands are case-sensitive!" << endl;
        while (!(input_line == "exit") && pc->getRespect() < 81 && pc->getRespect() > 0) { //player input
            getline(cin, input_line);
            if(input_line == "help"){
                cout << "\"look\": See the name, state, and description of the current room" << endl;
                cout << "\"clean\": Clean the current room" << endl;
                cout << "\"dirty\": Dirty the current room" << endl;
                cout << "\"north\": Try to travel to the north" << endl;
                cout << "\"south\": Try to travel to the south" << endl;
                cout << "\"east\": Try to travel to the east" << endl;
                cout << "\"west\": Try to travel to the west" << endl;
                cout << "\"{creature name}:clean\": Make the creature {creature name} clean the current room" << endl;
                cout << "\"{creature name}:dirty\": Make the creature {creature name} dirty the current room" << endl;
                cout << "\"exit\": Exit the game" << endl;
                cout << "Note: commands are case-sensitive!" << endl;
                //cout << "\"{creature name}:north\": Make a the creature {creature name} try to travel north" << endl;
            }
            else if(input_line == "look"){
                cout << rooms[pcRoomIndex] -> description() << endl;
            }
            else if(input_line == "clean"){
                cout << rooms[pcRoomIndex] -> playerAction(false, -1) << endl;
            }
            else if(input_line == "dirty"){
                cout << rooms[pcRoomIndex] -> playerAction(true, -1) << endl;
            }
            else if(input_line == "north"){
                string response = rooms[pcRoomIndex] -> moveCreature(pc, 0);
                if(response == "success"){
                    response = "You leave towards the North.";
                    pcRoomIndex = rooms[pcRoomIndex] -> getNorth();
                }
                cout << response << endl;
            }
            else if(input_line == "south"){
                string response = rooms[pcRoomIndex] -> moveCreature(pc, 1);
                if(response == "success"){
                    response = "You leave towards the South.";
                    pcRoomIndex = rooms[pcRoomIndex] -> getSouth();
                }
                cout << response << endl;
            }
            else if(input_line == "east"){
                string response = rooms[pcRoomIndex] -> moveCreature(pc, 2);
                if(response == "success"){
                    response = "You leave towards the East.";
                    pcRoomIndex = rooms[pcRoomIndex] -> getEast();
                }
                cout << response << endl;
            }
            else if(input_line == "west"){
                string response = rooms[pcRoomIndex] -> moveCreature(pc, 3);
                if(response == "success"){
                    response = "You leave towards the West.";
                    pcRoomIndex = rooms[pcRoomIndex] -> getWest();
                }
                cout << response << endl;
            }
            else if(input_line.substr(1) == ":clean"){
                cout << rooms[pcRoomIndex] -> playerAction(false, stoi(input_line.substr(0, 1))) << endl;
            }
            else if(input_line.substr(1) == ":dirty"){
                cout << rooms[pcRoomIndex] -> playerAction(true, stoi(input_line.substr(0, 1))) << endl;
            }
            else if(input_line == "exit"){
                cout << "Goodbye!" << endl;
            }
            else{
                cout << "Please input a proper command. To see the list of commands type \"help\"." << endl;
            }
        }
    }

    //clean up
    for(int i = 0; i<numRooms; i++){
        delete rooms[i]; //creatures are deleted inside of rooms;
        delete pc;
    }

    return 0;
}