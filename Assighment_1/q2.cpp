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
        // TODO: allocate a new float* array of size newCap, copy existing
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
        // TODO: if count == cap, double capacity.
    }

    void removeEmbedding(int index)
    {
        // TODO: bounds-check. delete[] the vector at index, then shift all pointers.
    }

    float *getEmbedding(int index)
    {
        // TODO: bounds-checked access, return pointer (or nullptr if invalid).
        return nullptr;
    }

    float distance(float *a, float *b)
    {
        // TODO: Euclidean distance between two length-dim vectors.
        return 0.0f;
    }

    int nearestNeighbor(float *query, float &outDistance)
    {
        // TODO: linear scan over all stored embeddings, find index with
        // minimum distance() to query. Set outDistance to that distance.
        // Return -1 if store is empty.
        return -1;
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
        // TODO (optional): handle malformed/unknown commands gracefully.
    }

    return 0;
}
