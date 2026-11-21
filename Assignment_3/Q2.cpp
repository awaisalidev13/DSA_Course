#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;


class Markete{
    double** rates;
    char names[100][31];
    int count;
    int cap;
    double fee;
    void dfs(int u, bool* seen, bool forward, int skip)
    {
        seen[u] = true;
        for(int v = 0; v < count; v++)
        {
            if(seen[v] || v == skip)
                continue;

            bool edgeExists;
            if(forward)
                edgeExists = (rates[u][v] != 0);
            else
                edgeExists = (rates[v][u] != 0);

            if(edgeExists)
                dfs(v, seen, forward, skip);
        }
    }

    bool strong(int skip)
    {
        int start = -1;
        for(int i = 0; i < count; i++)
        {
            if(i != skip)
            {
                start = i;
                break;
            }
        }
        if(start == -1)
            return true;

        bool seenF[100];
        bool seenB[100];
        for(int i = 0; i < 100; i++)
        {
            seenF[i] = false;
            seenB[i] = false;
        }

        dfs(start, seenF, true, skip);
        dfs(start, seenB, false, skip);

        for(int i = 0; i < count; i++)
        {
            if(i == skip) continue;
            if(!seenF[i] || !seenB[i])
                return false;
        }
        return true;
    }
    
    double eff(int a, int b)
    {
        if(rates[a][b] == 0)
            return 0;
        return rates[a][b] * (1 - fee / 100);
    }

    bool bellman(int src, double* best, int* parent)
    {
        for(int i = 0; i < count; i++)
        {
            best[i] = 0;
            parent[i] = -1;
        }
        best[src] = 1;

        for(int pass = 0; pass < count - 1; pass++)
        {
            for(int a = 0; a < count; a++)
            {
                for(int b = 0; b < count; b++)
                {
                    double e = eff(a, b);
                    if(e > 0 && best[a] > 0 && best[a] * e > best[b] * (1 + 1e-9))
                    {
                        best[b] = best[a] * e;
                        parent[b] = a;
                    }
                }
            }
        }

     
        for(int a = 0; a < count; a++)
        {
            for(int b = 0; b < count; b++)
            {
                double e = eff(a, b);
                if(e > 0 && best[a] > 0 && best[a] * e > best[b] * (1 + 1e-9))
                    return true;
            }
        }
        return false;
    }

     bool findArbitrage(int* loop, int& len, double& gain)
    {
        double best[100];
        int parent[100];
        for(int i = 0; i < count; i++)
        {
            best[i] = 1;
            parent[i] = -1;
        }

        for(int pass = 0; pass < count - 1; pass++)
        {
            for(int a = 0; a < count; a++)
            {
                for(int b = 0; b < count; b++)
                {
                    double e = eff(a, b);
                    if(e > 0 && best[a] * e > best[b] * (1 + 1e-9))
                    {
                        best[b] = best[a] * e;
                        parent[b] = a;
                    }
                }
            }
        }

        // extra sweep: find an edge that still improves
        int x = -1;
        for(int a = 0; a < count && x == -1; a++)
        {
            for(int b = 0; b < count; b++)
            {
                double e = eff(a, b);
                if(e > 0 && best[a] * e > best[b] * (1 + 1e-9))
                {
                    parent[b] = a;
                    x = b;
                    break;
                }
            }
        }
        if(x == -1)
            return false;

        // walk back count times to be sure we are inside the loop
        for(int i = 0; i < count; i++)
        {
            x = parent[x];
            if(x == -1) return false;
        }

        // collect the loop (comes out backwards)
        int tmp[100];
        int n = 0;
        int cur = x;
        do
        {
            tmp[n++] = cur;
            cur = parent[cur];
            if(cur == -1 || n > count) return false;
        } while(cur != x);

        // reverse into forward order
        int cyc[100];
        for(int i = 0; i < n; i++)
            cyc[i] = tmp[n - 1 - i];

        // rotate so the lowest ID is first
        int m = 0;
        for(int i = 1; i < n; i++)
            if(cyc[i] < cyc[m]) m = i;

        for(int i = 0; i < n; i++)
            loop[i] = cyc[(m + i) % n];
        loop[n] = loop[0];
        len = n + 1;

        // recompute the gain from the loop itself
        gain = 1;
        for(int i = 0; i < n; i++)
            gain *= eff(loop[i], loop[i + 1]);

        return gain > 1 + 1e-9;
    }

    void printLoop(int* loop, int len, double gain)
    {
        for(int i = 0; i < len; i++)
        {
            cout << names[loop[i]];
            if(i < len - 1) cout << " -> ";
        }
        cout << " (gain " << gain << "x)" << endl;
    }
    
    
    public:
        Markete()
        {
            count = 0;
            cap = 4;
            fee = 0;

            rates = new double*[cap];
            for(int i=0 ; i<cap ; i++)
            {
                rates[i] = new double[cap];
                for(int j=0 ; j<cap ; j++)
                {
                    rates[i][j] = 0;
                }
            }


        }

        ~Markete()
        {
            for(int i=0 ; i<cap ; i++)
            {
                delete[] rates[i];
                rates[i] = nullptr;
            }

            delete[] rates;
            rates = nullptr;
        }

        void resize()
        {
            double** temp = new double*[cap*2];
            for(int i=0 ; i<cap*2 ; i++)
            {
                temp[i] = new double[cap*2];
                for(int j=0 ; j<cap*2 ; j++)
                {
                    temp[i][j] = 0;
                }
            }

            for(int i=0 ; i<cap ; i++)
            {
                for(int j=0 ; j<cap ; j++)
                {
                    temp[i][j] = rates[i][j];
                }
            }

            for(int i=0 ; i<cap ; i++)
            {
                delete[] rates[i];
                rates[i] = nullptr;
            }

            delete[] rates;
            rates = temp;
            temp = nullptr;

            cap = cap*2;
        }

        int find(const char* name)
        {
            for(int i=0 ; i<count ; i++)
            {
                if(strcmp(name, names[i]) == 0)
                {
                    return i;
                }
            }
            return -1;
        }

        void listCurrency(const char* name)
        {
            if(count == 100)
            {
                cout << "FUll!" << endl;
                return;
            }

            char temp[200];
            strcpy(temp , name);

            if(strlen(temp) > 30 || strlen(temp) < 1)
            {
                cout << "Length Error." << endl;
                return;
            }

            if(find(temp) == -1)
            {
                if(count == cap)
                {
                    resize();
                }

                strcpy(names[count] , name);
                count++;
                cout << "Currencies : " << count << " | Capacity : " << cap << endl;

            }
            else
            {
                cout << "name already exists in the array. " << endl;
            }

            
        }

        void updateRate(const char* a, const char* b, double r)
        {
            int id1 = find(a);
            int id2 = find(b);

            if(id1 == -1 || id2 == -1 || id1 == id2 || r<0)
            {
                cout << "Rate cannot be updated!" << endl;
                return;
            }

            rates[id1][id2] = r;
            cout << "Rates updated | " << a << "->" << b << " : " << r << endl;
        }

        void setFee(double f)
        {
            if(f >= 0 && f <= 10)
            {
                fee = f;
                cout << "Fee set : " << f << "%" << endl;
            }
            else
            {
                cout << "Invalid fee rates!" << endl;
            }
        }

        void display()
        {
            if(count == 0)
            {
                cout << "No currencies listed." << endl;
                return;
            }

            const int W = 10;

            cout << setw(W) << "";
            for(int j = 0; j < count; j++)
                cout << setw(W) << names[j];
            cout << endl;

            for(int i = 0; i < count; i++)
            {
                cout << setw(W) << names[i];
                for(int j = 0; j < count; j++)
                {
                    if(rates[i][j] == 0)
                        cout << setw(W) << "-";
                    else
                        cout << setw(W) << rates[i][j];
                }
                cout << endl;
            }
        }

        void degree()
        {
            if(count == 0)
            {
                cout << "No currencies listed." << endl;
                return;
            }

            for(int i = 0; i < count; i++)
            {
                int inDeg = 0;
                int outDeg = 0;
                for(int j = 0; j < count; j++)
                {
                    if(rates[i][j] != 0)
                        outDeg++;
                    if(rates[j][i] != 0)
                        inDeg++;
                }
                cout << "Degree of " << names[i] << " = InDegree: " << inDeg << " OutDegree: " << outDeg << endl;
            }
        }
        void checkStrong()
        {
            if(strong(-1))
                cout << "STRONG: YES" << endl;
            else
                cout << "STRONG: NO" << endl;
        }
        
        void bestConversion(const char* a, const char* b)
        {
            int src = find(a);
            int dst = find(b);
            if(src == -1 || dst == -1 || src == dst)
            {
                cout << "Invalid currencies." << endl;
                return;
            }

            double best[100];
            int parent[100];

            if(bellman(src, best, parent))
            {
                cout << "BEST " << a << "->" << b << ": UNDEFINED" << endl;
                return;
            }
            if(best[dst] == 0)
            {
                cout << "BEST " << a << "->" << b << ": NO PATH" << endl;
                return;
            }

            int chain[100];
            int len = 0;
            for(int x = dst; x != -1; x = parent[x])
                chain[len++] = x;

            cout << "BEST " << a << "->" << b << ": " << best[dst] << " | ";
            for(int i = len - 1; i >= 0; i--)
                cout << names[chain[i]] << " ";
            cout << endl;
        }

        void convert(double amount, const char* a, const char* b)
        {
            int src = find(a);
            int dst = find(b);
            if(src == -1 || dst == -1 || src == dst || amount <= 0)
            {
                cout << "Invalid input." << endl;
                return;
            }

            double best[100];
            int parent[100];

            if(bellman(src, best, parent))
            {
                cout << "UNDEFINED" << endl;
                return;
            }
            if(best[dst] == 0)
            {
                cout << "NO PATH" << endl;
                return;
            }

            cout << amount << " " << a << " -> " << amount * best[dst] << " " << b << endl;
        }

        void detectArbitrage()
        {
            int loop[101];
            int len;
            double gain;
            if(findArbitrage(loop, len, gain))
            {
                cout << "ARBITRAGE: ";
                printLoop(loop, len, gain);
            }
            else
                cout << "NO ARBITRAGE" << endl;
        }

        void monitor()
        {
            int loop[101];
            int len;
            double gain;
            if(findArbitrage(loop, len, gain))
            {
                cout << "ALERT: ";
                printLoop(loop, len, gain);
            }
        }

        void delist(const char* name)
        {
            int id = find(name);
            if(id == -1)
            {
                cout << "Currency not found." << endl;
                return;
            }

            
            int loop[101];
            int len;
            double gain;
            if(findArbitrage(loop, len, gain))
            {
                for(int i = 0; i < len - 1; i++)
                {
                    if(loop[i] == id)
                    {
                        cout << "DELIST DENIED: " << name
                            << " (part of an arbitrage loop)" << endl;
                        return;
                    }
                }
            }

            
            if(!strong(id))
            {
                cout << "DELIST DENIED: " << name
                    << " (remaining currencies would not be strongly connected)" << endl;
                return;
            }

            
            for(int i = id; i < count - 1; i++)
                strcpy(names[i], names[i + 1]);

            
            for(int i = 0; i < count - 1; i++)
            {
                int ri = (i >= id) ? i + 1 : i;
                for(int j = 0; j < count - 1; j++)
                {
                    int rj = (j >= id) ? j + 1 : j;
                    rates[i][j] = rates[ri][rj];
                }
            }

            
            for(int k = 0; k < count; k++)
            {
                rates[count - 1][k] = 0;
                rates[k][count - 1] = 0;
            }

            count--;
            cout << "DELISTED: " << name << endl;
            monitor();
        }

};

int main()
{
    cout.setf(ios::fixed);
    cout.precision(4);

    Markete m;
    int choice = 0;
    char n1[200], n2[200];
    double x;

    do
    {
        cout << "\n===== CURRENCY EXCHANGE FLOOR =====" << endl;
        cout << "1. List New Currency" << endl;
        cout << "2. Update Rate" << endl;
        cout << "3. Set Trading Fee" << endl;
        cout << "4. Display Market" << endl;
        cout << "5. Show Degrees" << endl;
        cout << "6. Check Strong Connectivity" << endl;
        cout << "7. Best Conversion" << endl;
        cout << "8. Convert Amount" << endl;
        cout << "9. Detect Arbitrage" << endl;
        cout << "10. Delist Currency" << endl;
        cout << "11. Exit" << endl;
        cout << "Enter choice: ";

        cin >> choice;
        if(cin.fail()) break;   // stops an infinite loop if a letter is typed

        switch(choice)
        {
            case 1:
                cout << "Name: ";
                cin.width(200);
                cin >> n1;
                m.listCurrency(n1);
                break;

            case 2:
                cout << "From, To, Rate: ";
                cin.width(200);
                cin >> n1;
                cin.width(200);
                cin >> n2;
                cin >> x;
                m.updateRate(n1, n2, x);
                break;

            case 3:
                cout << "Fee (0-10): ";
                cin >> x;
                m.setFee(x);
                break;

            case 4:
                m.display();
                break;

            case 5:
                m.degree();
                break;

            case 6:
                m.checkStrong();
                break;

            case 7:
                cout << "From, To: ";
                cin.width(200);
                cin >> n1;
                cin.width(200);
                cin >> n2;
                m.bestConversion(n1, n2);
                break;

            case 8:
                cout << "Amount, From, To: ";
                cin >> x;
                cin.width(200);
                cin >> n1;
                cin.width(200);
                cin >> n2;
                m.convert(x, n1, n2);
                break;

            case 9:
                m.detectArbitrage();
                break;

            case 10:
                cout << "Name: ";
                cin.width(200);
                cin >> n1;
                m.delist(n1);
                break;

            case 11:
                cout << "Goodbye." << endl;
                break;

            default:
                cout << "Invalid choice." << endl;
        }
    } while(choice != 11);

    return 0;
}