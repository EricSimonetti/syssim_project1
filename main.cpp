#include <iostream>
#include <sstream>

using namespace std;

int* separateInput(string input, int size){
    istringstream stringStream(input); // split string at spaces
    string intAsString; // for storing each number
    int intInputs[size];

    // Traverse through all numbers
    for(int i = 0; i<size; i++){
        stringStream >> intAsString;
        intInputs[i] = stoi(intAsString);
    }
    return intInputs;
}

int main() {
    string input_line;
    getline(cin, input_line);
    int numElements = stoi(input_line); //get number of rooms

    for(int i = 0; i<numElements; i++){ //process rooms
        getline(cin, input_line);
        int* inputs = separateInput(input_line, 4);
        for(int j = 0; j<4; j++){

        }
    }
    getline(cin, input_line);
    numElements = stoi(input_line);//get number of creatures

    for(int i = 0; i<numElements; i++){ //process creatures
        getline(cin, input_line);
        int* inputs = separateInput(input_line, 2);
        for(int j = 0; j<2; j++){

        }
    }

    while(!(input_line == "exit")) { //player input
        getline(cin, input_line);
        cout << input_line << endl;
    }
    return 0;
}