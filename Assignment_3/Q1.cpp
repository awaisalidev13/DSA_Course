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
    int count;

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

    void insertSS(int ID , int time)
    {
        TrackNode* nn = new TrackNode(ID,time);

        if(head == nullptr)
        {
            head = nn;
        }
        else
        {
            if(head->neID > ID)
            {
                nn->next = head;
                head = nn;
            }
            else
            {
                TrackNode* current = head;

                while(current->next != nullptr && current->next->neID < ID)
                {
                    current = current->next;
                }
                if(current->next == nullptr)
                {
                    current->next = nn;
                }
                else
                {
                    nn->next = current->next;
                    current->next = nn;
                }
            }
        }

        count++;
    }
    
    int removeSS(int ID)
    {
        TrackNode* current = head;
        TrackNode* previous = nullptr;

        while(current != nullptr && current->neID != ID)
        {
            previous = current;
            current = current->next;
        }

        if(current == nullptr)
        {
            cout << "Station not found" << endl;
            return -1;
        }
        else
        {
            if(previous == nullptr)
            {
                head = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            int oldTime = current->trTime;
            delete current;
            count--;
            return oldTime;
        }
    }

    void delaySS(int ID , int time)
    {
        TrackNode* current = head;

        while(current != nullptr && current->neID != ID)
        {
            current = current->next;
        }

        if(current == nullptr)
        {
            cout << "Station not found" << endl;
        }
        else
        {
            current->trTime += time;
        }
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
                TrackNode *current = station[i].head;
                while(current != nullptr)
                {
                    TrackNode *temp = current;
                    current = current->next;
                    delete temp;
                }

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

        void addStation(string name)
        {
            if(stCount >= 100)
            {
                cout << "Station limit reached. Cannot add more stations." << endl;
                return;
            }
            if(isValidName(name))
            {
                if(getStationID(name) == -1)
                {
                    station[stCount].stName = name;
                    station[stCount].head = nullptr;
                    station[stCount].status = true;
                    station[stCount].stID = stCount;
                    station[stCount].count = 0;
                    stCount++;

                }
                else
                {
                    cout << "Station already exists." << endl;
                }
            }
            else
            {
                cout << "Invalid name. Station not added." << endl;
            }
        }

        void displayStations()

        {
            for(int i=0 ; i<stCount ; i++)
            {
                if (!station[i].status) continue;

                cout << station[i].stName << " : ";

                if(station[i].head == nullptr)
                {
                    cout << "-" << endl;
                }
                else
                {
                    TrackNode *current = station[i].head;
                    while(current != nullptr)
                    {
                        cout << station[current->neID].stName<< "(" << current->trTime << ")";
                        if(current->next != nullptr)
                        {
                            cout << " -> ";
                        }
                        current = current->next;
                    }
                    cout << endl;
                }
            }
        }

        bool trackExists(int stID1 , int stID2)
        {
            TrackNode *current = station[stID1].head;
            while(current != nullptr)
            {
                if(current->neID == stID2)
                {
                    return true;
                }
                current = current->next;
            }
            return false;
        }

        void insertSorted(int stID1 , int stID2 , int time)
        {
            station[stID1].insertSS(stID2,time);
        }
    
        void addTrack(string name1, string name2, int time)
        {
            int ID1 = getStationID(name1);
            int ID2 = getStationID(name2);
            if(ID1 == -1 || ID2 == -1 || !station[ID1].status || !station[ID2].status)
            {
                cout << "One or both stations do not exist or are inactive." << endl;
                return;
            }

            if(trackExists(ID1,ID2))
            {
                cout << "Track already exists." << endl;
                return;
            }
            if(time <= 0)
            {
                cout << "Invalid time. Track not added." << endl;
                return;
            }

            if(ID1 == ID2)
            {
                cout << "Cannot add track to the same station." << endl;
                return;
            }

            insertSorted(ID1, ID2, time);
            insertSorted(ID2, ID1, time);
        }

        void stationDegree(string name)
        {
            int ID = getStationID(name);
            if(ID == -1 || !station[ID].status)
            {
                cout << "Station does not exist or is inactive." << endl;
                return;
            }

            cout << "Degree of station " << name << " is: " << station[ID].count << endl;
        }

        void closeTrack(string name1, string name2)
        {
            int ID1 = getStationID(name1);
            int ID2 = getStationID(name2);
            if(ID1 == -1 || ID2 == -1 || !station[ID1].status || !station[ID2].status)
            {
                cout << "One or both stations do not exist or are inactive." << endl;
                return;
            }

            if(!trackExists(ID1,ID2))
            {
                cout << "Track does not exist." << endl;
                return;
            }

            station[ID1].removeSS(ID2);
            station[ID2].removeSS(ID1);
            cout << "TRACK CLOSED: " << name1 << "-" << name2 << endl;
        }
        
        void delayTrack(string name1, string name2, int time)
        {
            if(time <= 0)
            {
                cout << "Invalid time. Track not delayed." << endl;
                return;
            }
            int ID1 = getStationID(name1);
            int ID2 = getStationID(name2);
            if(ID1 == -1 || ID2 == -1 || !station[ID1].status || !station[ID2].status)
            {
                cout << "One or both stations do not exist or are inactive." << endl;
                return;
            }

            if(!trackExists(ID1,ID2))
            {
                cout << "Track does not exist." << endl;
                return;
            }

            station[ID1].delaySS(ID2,time);
            station[ID2].delaySS(ID1,time);
            cout << "TRACK DELAYED: " << name1 << "-" << name2 << " by " << time << endl;
        }
    
        void pushUndo(int type, int stID1, int stID2, int trTime)
        {
            if(undoCount < 50)
            {
                undoStack[undoCount].opType = type;
                undoStack[undoCount].stID1 = stID1;
                undoStack[undoCount].stID2 = stID2;
                undoStack[undoCount].trTime = trTime;
                undoCount++;
            }
            else
            {
                for(int i=1 ; i<50 ; i++)
                {
                    undoStack[i-1] = undoStack[i];
                }
                undoCount--;
                pushUndo(type,stID1,stID2,trTime);
            }
        }
        
        void addTrack(string name1, string name2, int time)
        {
            int ID1 = getStationID(name1);
            int ID2 = getStationID(name2);
            if(ID1 == -1 || ID2 == -1 || !station[ID1].status || !station[ID2].status)
            {
                cout << "One or both stations do not exist or are inactive." << endl;
                return;
            }
            if(trackExists(ID1, ID2))
            {
                cout << "Track already exists." << endl;
                return;
            }
            if(time <= 0)
            {
                cout << "Invalid time. Track not added." << endl;
                return;
            }
            if(ID1 == ID2)
            {
                cout << "Cannot add track to the same station." << endl;
                return;
            }

            insertSorted(ID1, ID2, time);
            insertSorted(ID2, ID1, time);
            pushUndo(1, ID1, ID2, time); // Record ADD
        }

        void closeTrack(string name1, string name2)
        {
            int ID1 = getStationID(name1);
            int ID2 = getStationID(name2);
            if(ID1 == -1 || ID2 == -1 || !station[ID1].status || !station[ID2].status)
            {
                cout << "One or both stations do not exist or are inactive." << endl;
                return;
            }
            if(!trackExists(ID1, ID2))
            {
                cout << "Track does not exist." << endl;
                return;
            }

            int oldTime = station[ID1].removeSS(ID2);
            station[ID2].removeSS(ID1);
            pushUndo(2, ID1, ID2, oldTime); // Record CLOSE with original time
            cout << "TRACK CLOSED: " << name1 << "-" << name2 << endl;
        }
        
        void delayTrack(string name1, string name2, int time)
        {
            int ID1 = getStationID(name1);
            int ID2 = getStationID(name2);
            if(ID1 == -1 || ID2 == -1 || !station[ID1].status || !station[ID2].status)
            {
                cout << "One or both stations do not exist or are inactive." << endl;
                return;
            }
            if(time <= 0)
            {
                cout << "Delay time must be positive." << endl;
                return;
            }
            if(!trackExists(ID1, ID2))
            {
                cout << "Track does not exist." << endl;
                return;
            }

            station[ID1].delaySS(ID2, time);
            station[ID2].delaySS(ID1, time);
            pushUndo(3, ID1, ID2, time); // Record DELAY with added time
            cout << "TRACK DELAYED: " << name1 << "-" << name2 << " by " << time << endl;
        }

        void undo()
        {
            if(undoCount == 0)
            {
                cout << "Nothing to undo. History is empty." << endl;
                return;
            }

            undoCount--;
            UndoOp op = undoStack[undoCount];

            if(op.opType == 1) // Reverse ADD -> remove track
            {
                station[op.stID1].removeSS(op.stID2);
                station[op.stID2].removeSS(op.stID1);
                cout << "UNDONE: ADD " << station[op.stID1].stName << "-" << station[op.stID2].stName << endl;
            }
            else if(op.opType == 2) // Reverse CLOSE -> re-insert with original time
            {
                insertSorted(op.stID1, op.stID2, op.trTime);
                insertSorted(op.stID2, op.stID1, op.trTime);
                cout << "UNDONE: CLOSE " << station[op.stID1].stName << "-" << station[op.stID2].stName << endl;
            }
            else if(op.opType == 3) // Reverse DELAY -> subtract extra time
            {
                station[op.stID1].delaySS(op.stID2, -op.trTime);
                station[op.stID2].delaySS(op.stID1, -op.trTime);
                cout << "UNDONE: DELAY " << station[op.stID1].stName << "-" << station[op.stID2].stName << endl;
            }
        }
    };

int main() {
    MetroNetwork metro;

    // 1. Add Stations from Page 3 Example
    metro.addStation("A");
    metro.addStation("B");
    metro.addStation("C");
    metro.addStation("D");
    metro.addStation("E");
    metro.addStation("F");

    // 2. Add Tracks from Page 3 Example
    // (Notice we add C-B before B-A gets tested to see if B's list sorts A(0), C(2), D(3) properly!)
    metro.addTrack("A", "B", 4);
    metro.addTrack("A", "C", 2);
    metro.addTrack("C", "B", 1);
    metro.addTrack("B", "D", 5);
    metro.addTrack("C", "D", 8);
    metro.addTrack("D", "E", 3);

    // 3. Display Network
    cout << "=== Network Display ===" << endl;
    metro.displayStations();

    // 4. Test Error Rejections
    cout << "\n=== Testing Edge Cases ===" << endl;
    metro.addStation("A");          // Duplicate station
    metro.addTrack("A", "B", 10);   // Duplicate track
    metro.addTrack("A", "A", 5);    // Self-loop
    metro.addTrack("A", "Z", 5);    // Unknown station
    metro.addTrack("E", "F", -3);   // Negative travel time

    return 0;
}