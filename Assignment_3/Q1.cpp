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
    
        void fewestStopsRoute(string name1, string name2)
        {
            int src = getStationID(name1);
            int dest = getStationID(name2);

            if(src == -1 || dest == -1 || !station[src].status || !station[dest].status)
            {
                cout << "One or both stations do not exist or are inactive." << endl;
                return;
            }

            if(src == dest)
            {
                cout << "MINSTOPS " << name1 << "->" << name2 << ": 0 hops | " << name1 << endl;
                return;
            }

            bool visited[100];
            int parent[100];
            for(int i = 0; i < stCount; i++)
            {
                visited[i] = false;
                parent[i] = -1; // -1 means no previous station yet
            }

            int queue[100];
            int front = 0;
            int rear = 0;

            // Put starting station in the queue
            visited[src] = true;
            queue[rear] = src;
            rear++;

            while(front < rear)
            {
                int u = queue[front];
                front++;

                if(u == dest) break; // Reached destination

                TrackNode *current = station[u].head;
                while(current != nullptr)
                {
                    int v = current->neID;
                    if(station[v].status && !visited[v])
                    {
                        visited[v] = true;
                        parent[v] = u;    
                        queue[rear] = v;  
                        rear++;
                    }
                    current = current->next;
                }
            }

            if(!visited[dest])
            {
                cout << "NO PATH" << endl;
                return;
            }

            int path[100];
            int pathLen = 0;
            int temp = dest;
            while(temp != -1)
            {
                path[pathLen] = temp;
                pathLen++;
                temp = parent[temp]; // Step backward to previous station
            }

            cout << "MINSTOPS " << name1 << "->" << name2 << ": " << (pathLen - 1) << " hops | ";
            for(int i = pathLen - 1; i >= 0; i--)
            {
                cout << station[path[i]].stName << " ";
            }
            cout << endl;
        }

         int fastestRoute(string name1, string name2, string avoidName = "", bool printRoute = true)
        {
            int src = getStationID(name1);
            int dest = getStationID(name2);

            if(src == -1 || dest == -1 || !station[src].status || !station[dest].status)
            {
                if(printRoute) cout << "UNKNOWN OR DECOMMISSIONED STATION" << endl;
                return 99999999;
            }

            int avoidID = -1;
            if(avoidName != "")
            {
                avoidID = getStationID(avoidName);
                if(avoidID == -1 || !station[avoidID].status || avoidID == src || avoidID == dest)
                {
                    if(printRoute) cout << "Invalid station to avoid." << endl;
                    return 99999999;
                }
            }
            
            int dist[100];
            bool visited[100];
            int parent[100];

            for(int i = 0; i < stCount; i++)
            {
                dist[i] = 99999999;
                visited[i] = false;
                parent[i] = -1;
            }
            dist[src] = 0;

            for(int step = 0; step < stCount; step++)
            {
                int u = -1;
                int minDist = 99999999;
                for(int i = 0; i < stCount; i++)
                {
                    if(station[i].status && i != avoidID && !visited[i] && dist[i] < minDist)
                    {
                        minDist = dist[i];
                        u = i;
                    }
                }

                if(u == -1 || u == dest) break;
                visited[u] = true;

                TrackNode *current = station[u].head;
                while(current != nullptr)
                {
                    int v = current->neID;
                    int weight = current->trTime;

                    if(station[v].status && v != avoidID && !visited[v])
                    {
                        if(dist[u] + weight < dist[v])
                        {
                            dist[v] = dist[u] + weight;
                            parent[v] = u;
                        }
                    }
                    current = current->next;
                }
            }

            if(dist[dest] == 99999999)
            {
                if(printRoute) cout << "NO PATH" << endl;
                return 99999999;
            }

            if(printRoute)
            {
                int path[100], pathLen = 0;
                for(int temp = dest; temp != -1; temp = parent[temp])
                    path[pathLen++] = temp;

                if(avoidName == "")
                    cout << "FASTEST " << name1 << "->" << name2 << ": " << dist[dest] << " min | ";
                else
                    cout << "FASTEST " << name1 << "->" << name2 << " avoiding " << avoidName << ": " << dist[dest] << " min | ";

                for(int i = pathLen - 1; i >= 0; i--)
                    cout << station[path[i]].stName << " ";
                cout << endl;
            }

            return dist[dest]; // <-- Return the fastest time!
        }
        
        
        void reachableWithinTime(string name, int T)
        {
            int src = getStationID(name);
            if(src == -1 || !station[src].status || T < 0)
            {
                cout << "Invalid station or time limit." << endl;
                return;
            }

            int reachID[100];
            int reachTime[100];
            int rCount = 0;

            // Call your fastestRoute function for every station!
            for(int i = 0; i < stCount; i++)
            {
                if(station[i].status && i != src)
                {
                    int time = fastestRoute(name, station[i].stName, "", false); // false = don't print route
                    if(time <= T)
                    {
                        reachID[rCount] = i;
                        reachTime[rCount] = time;
                        rCount++;
                    }
                }
            }

            // Sort the reachable stations by time (ties by smaller ID)
            for(int i = 0; i < rCount - 1; i++)
            {
                for(int j = i + 1; j < rCount; j++)
                {
                    if(reachTime[j] < reachTime[i] || (reachTime[j] == reachTime[i] && reachID[j] < reachID[i]))
                    {
                        int tempT = reachTime[i]; reachTime[i] = reachTime[j]; reachTime[j] = tempT;
                        int tempID = reachID[i]; reachID[i] = reachID[j]; reachID[j] = tempID;
                    }
                }
            }

            cout << "WITHIN " << T << " min of " << name << ": ";
            if(rCount == 0) cout << "NONE";
            for(int i = 0; i < rCount; i++)
            {
                cout << station[reachID[i]].stName << "(" << reachTime[i] << ") ";
            }
            cout << endl;
        }
        

        void dfsVisit(int u, int ignoreID, bool visited[], bool print)
        {
            visited[u] = true;
            if(print) cout << station[u].stName << " ";

            TrackNode *current = station[u].head;
            while(current != nullptr)
            {
                int v = current->neID;
                if(station[v].status && v != ignoreID && !visited[v])
                {
                    dfsVisit(v, ignoreID, visited, print);
                }
                current = current->next;
            }
        }

        // Counts how many separate connected components exist (ignoring ignoreID)
        int countComponents(int ignoreID = -1)
        {
            bool visited[100];
            for(int i = 0; i < stCount; i++) visited[i] = false;

            int compCount = 0;
            for(int i = 0; i < stCount; i++)
            {
                if(station[i].status && i != ignoreID && !visited[i])
                {
                    compCount++;
                    dfsVisit(i, ignoreID, visited, false);
                }
            }
            return compCount;
        }

        void networkComponents()
        {
            int total = countComponents(-1);
            cout << "COMPONENTS: " << total << endl;

            bool visited[100];
            for(int i = 0; i < stCount; i++) visited[i] = false;

            int compNum = 1;
            for(int i = 0; i < stCount; i++)
            {
                if(station[i].status && !visited[i])
                {
                    cout << "Component " << compNum << ": ";
                    dfsVisit(i, -1, visited, true);
                    cout << endl;
                    compNum++;
                }
            }
        }

        
        bool isCritical(int id)
        {
            int baseComp = countComponents(-1);
            int withoutStationComp = countComponents(id);
            return (withoutStationComp > baseComp);
        }

        void criticalStations()
        {
            cout << "CRITICAL: ";
            bool foundAny = false;

            for(int i = 0; i < stCount; i++)
            {
                if(station[i].status && isCritical(i))
                {
                    cout << station[i].stName << " ";
                    foundAny = true;
                }
            }

            if(!foundAny) cout << "NONE";
            cout << endl;
        }
        
        void decommissionStation(string name)
        {
            int id = getStationID(name);
            if(id == -1 || !station[id].status)
            {
                cout << "UNKNOWN OR DECOMMISSIONED STATION: " << name << endl;
                return;
            }

            // Rule 1: Cannot be a critical station
            if(isCritical(id))
            {
                cout << "DENIED: " << name << " is critical" << endl;
                return;
            }

            // Rule 2: Must have at most 2 tracks
            if(station[id].count > 2)
            {
                cout << "DENIED: " << name << " has more than 2 tracks" << endl;
                return;
            }

            // Remove all tracks connected to this station
            while(station[id].head != nullptr)
            {
                int neighborID = station[id].head->neID;
                station[neighborID].removeSS(id);
                station[id].removeSS(neighborID);
            }

            station[id].status = false;
            undoCount = 0; // Requirement 3: Decommissioning clears undo history
            cout << "DECOMMISSIONED: " << name << endl;
        }
};

int main() {
    MetroNetwork metro;
    int choice;

    while (true) {
        cout << "\n===== CAPITAL METRO CONTROL CENTER =====" << endl;
        cout << "1. Add Station" << endl;
        cout << "2. Add Track" << endl;
        cout << "3. Close Track" << endl;
        cout << "4. Delay Track" << endl;
        cout << "5. Undo Last Operation" << endl;
        cout << "6. Station Degree" << endl;
        cout << "7. Fewest Stops Route" << endl;
        cout << "8. Fastest Route (optional: avoid a station)" << endl;
        cout << "9. Stations Reachable Within T Minutes" << endl;
        cout << "10. Network Components" << endl;
        cout << "11. Critical Stations" << endl;
        cout << "12. Decommission Station" << endl;
        cout << "13. Display Network" << endl;
        cout << "14. Exit" << endl;
        cout << "Enter choice: ";
        if (!(cin >> choice)) break;

        if (choice == 14) break;

        string s1, s2, avoid;
        int time;

        switch (choice) {
            case 1:
                cout << "Enter station name: ";
                cin >> s1;
                metro.addStation(s1);
                break;
            case 2:
                cout << "Enter station 1, station 2, and travel time: ";
                cin >> s1 >> s2 >> time;
                metro.addTrack(s1, s2, time);
                break;
            case 3:
                cout << "Enter station 1 and station 2: ";
                cin >> s1 >> s2;
                metro.closeTrack(s1, s2);
                break;
            case 4:
                cout << "Enter station 1, station 2, and delay minutes: ";
                cin >> s1 >> s2 >> time;
                metro.delayTrack(s1, s2, time);
                break;
            case 5:
                metro.undo();
                break;
            case 6:
                cout << "Enter station name: ";
                cin >> s1;
                metro.stationDegree(s1);
                break;
            case 7:
                cout << "Enter source and destination station: ";
                cin >> s1 >> s2;
                metro.fewestStopsRoute(s1, s2);
                break;
            case 8:
                cout << "Enter source and destination station: ";
                cin >> s1 >> s2;
                cout << "Enter station to avoid (or - for none): ";
                cin >> avoid;
                metro.fastestRoute(s1, s2, avoid, true);
                break;
            case 9:
                cout << "Enter station name and time limit T: ";
                cin >> s1 >> time;
                metro.reachableWithinTime(s1, time);
                break;
            case 10:
                metro.networkComponents();
                break;
            case 11:
                metro.criticalStations();
                break;
            case 12:
                cout << "Enter station name to decommission: ";
                cin >> s1;
                metro.decommissionStation(s1);
                break;
            case 13:
                metro.displayStations();
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    }
    return 0;
}