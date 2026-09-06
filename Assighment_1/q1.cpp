#include <iostream>
#include <string>
using namespace std;

class ImageGrid
{
private:
    int **data;
    int rows;
    int cols;

public:
    ImageGrid(int r, int c)
    {
        rows = r;
        cols = c;
        data = new int *[rows];
        for (int i = 0; i < rows; i++)
        {
            data[i] = new int[cols];
            for (int j = 0; j < cols; j++)
                data[i][j] = 0;
        }
    }

    ~ImageGrid()
    {
        for (int i = 0; i < rows; i++)
            delete[] data[i];
        delete[] data;
    }

    int getRows() { return rows; }
    int getCols() { return cols; }

    bool inBounds(int r, int c)
    {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    }

    int get(int r, int c)
    {
        // TODO: bounds-checked read. Return -1 (or throw) if out of bounds.
        return -1;
    }

    void set(int r, int c, int value)
    {
        // TODO: bounds-checked write.
    }

    void rotate90Clockwise()
    {
        // TODO: rotate the grid 90 degrees clockwise.
    }

    void flipHorizontal()
    {
        // TODO: mirror the image left-right, in place (no extra grid needed).
    }

    void histogram(int *outCounts /* size 256, pre-zeroed by caller */)
    {
        // TODO: count occurrences of each intensity value (0-255) into outCounts.
    }

    void applyBoxBlur()
    {
        // TODO: replace each pixel with the integer average of its EXISTING
    }

    void print()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cout << data[i][j];
                if (j != cols - 1)
                    cout << " ";
            }
            cout << endl;
        }
    }
};

int main()
{
    int rows, cols;
    cin >> rows >> cols;

    ImageGrid img(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int val;
            cin >> val;
            img.set(i, j, val);
        }
    }

    string cmd;
    while (cin >> cmd)
    {
        if (cmd == "ROTATE")
        {
            img.rotate90Clockwise();
        }
        else if (cmd == "FLIP")
        {
            img.flipHorizontal();
        }
        else if (cmd == "HIST")
        {
            int counts[256] = {0};
            img.histogram(counts);
            for (int v = 0; v < 256; v++)
            {
                if (counts[v] > 0)
                    cout << v << ": " << counts[v] << endl;
            }
        }
        else if (cmd == "BLUR")
        {
            img.applyBoxBlur();
        }
        else if (cmd == "PRINT")
        {
            img.print();
        }
        // TODO (optional): handle malformed/unknown commands gracefully.
    }

    return 0;
}
