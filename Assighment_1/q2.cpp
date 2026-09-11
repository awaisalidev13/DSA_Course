#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class EmbeddingStore
{
private:
    float **items;
    int dim;
    int count;
    int cap;

    void resize(int newCap)
    {
        if(newCap <= cap)
            return;
        float** temp = new float*[newCap];

        for(int i=0 ; i<count ; i++)
        {
            temp[i] = new float[dim];
            for(int j=0 ; j<dim ; j++)
            {
                temp[i][j] = items[i][j];
            }
        }
        for(int i=0 ; i<count ; i++)
        {
            delete[] items[i];
        }
        delete[] items;
        items = temp;
        cap = newCap;
    }

public:
    EmbeddingStore(int dimension, int initialCapacity = 4)
    {
        dim = dimension;
        cap = initialCapacity;
        count = 0;
        items = new float *[cap];
    }

    ~EmbeddingStore()
    {
        for (int i = 0; i < count; i++)
            delete[] items[i];
        delete[] items;
    }

    int size() { return count; }
    int capacity() { return cap; }

    void addEmbedding(float *vec)
    {
        if(count == cap)
        {
            resize(cap * 2);
        }
        items[count] = new float[dim];
        for(int i=0 ; i<dim ; i++)
        {
            items[count][i] = vec[i];
        }
        count++;
    }

    void removeEmbedding(int index)
    {
        if(index >= count || index < 0)
        {
            return;
        }

    
        for(int i=index ; i<count-1 ; i++)
        {   
            for(int j=0 ; j<dim ; j++)
            {
                items[i][j] = items[i+1][j];
            }
        }
        delete[] items[count-1];
        count--;
    }

    float *getEmbedding(int index)
    {
        if(index >= count || index < 0)
        {
            return nullptr;
        }
        
        return items[index];
    }

    float distance(float *a, float *b)
    {
        float sum = 0;
        for(int i=0 ; i<dim ; i++)
        {
            sum += pow(b[i]-a[i],2);
        }
        
        return sqrt(sum);
    }

    int nearestNeighbor(float *query, float &outDistance)
    {
        if(count == 0)
        {
            return -1;
        }
        
        int bestIndex = 0;
        float min = distance(query,items[0]);
        for(int i=1 ; i<count ; i++)
        {
            if(distance(query,items[i]) < min)
            {
                min = distance(query, items[i]);
                bestIndex = i;
            }
        }

        outDistance = min;
        return bestIndex;
    }

    void printState()
    {
        for (int i = 0; i < count; i++)
        {
            cout << "[" << i << "] ";
            for (int j = 0; j < dim; j++)
            {
                cout << items[i][j];
                if (j != dim - 1)
                    cout << " ";
            }
            cout << endl;
        }
    }
};

int main()
{
    int dim;
    cin >> dim;

    EmbeddingStore store(dim);

    string cmd;
    while (cin >> cmd)
    {
        if (cmd == "ADD")
        {
            float *vec = new float[dim];
            for (int i = 0; i < dim; i++)
                cin >> vec[i];
            store.addEmbedding(vec);
            delete[] vec; // addEmbedding should have made its own copy
            cout << "size=" << store.size() << " capacity=" << store.capacity() << endl;
        }
        else if (cmd == "REMOVE")
        {
            int idx;
            cin >> idx;
            store.removeEmbedding(idx);
            cout << "size=" << store.size() << " capacity=" << store.capacity() << endl;
        }
        else if (cmd == "QUERY")
        {
            float *q = new float[dim];
            for (int i = 0; i < dim; i++)
                cin >> q[i];
            float dist;
            int idx = store.nearestNeighbor(q, dist);
            cout << "nearest=" << idx << " distance=" << dist << endl;
            delete[] q;
        }
        else if (cmd == "STATE")
        {
            store.printState();
        }
        else
        {
            cout << "Unknown Command : " << cmd << endl;
        }
    }

    return 0;
}
