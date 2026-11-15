#include <iostream>
#include <cmath>
#include <string>
using namespace std;


struct TrackNode{
    int neID;
    int trTime;
    TrackNode *next;

    TrackNode(int id=0, int time=0){
        neID = id;
        trTime = time;
        next = nullptr;
    }
};

struct Station{
    string stName;
    int stID;
    bool status;
    TrackNode *head;
    

    Station(){
        stName = "";
        stID = -1;
        status = false;
        head = nullptr;
    }
    Station(string name, int id){
        stName = name;
        stID = id;
        status = true;
        head = nullptr;
    }
    
};

struct UndoOp{
    int opType; // 1 for add, 2 for remove
    int stID1;
    int stID2;
    int trTime;
};

class MetroNetwork{
    Station station[100];
    int stCount;
    UndoOp undoStack[50];
    int undoCount;

    public:
        MetroNetwork()
        {
            stCount = 0;
            undoCount = 0;
        }
        
        ~MetroNetwork()
        {
            for(int i=0 ; i<stCount ; i++)
            {
                delete station[i].head;

                station[i].head = nullptr;
            }
        }

        bool isValidName(string name)
        {
            if(name.length() >= 1 && name.length() <= 30)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        int getStationID(string name)
        {
            for(int i=0 ; i<stCount ; i++)
            {
                if(station[i].stName == name)
                {
                    return i;
                }
            }

            return -1;
        }
};