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
        if(inBounds(r,c))
        {
            return data[r][c];
        }
        else
        {    
            return -1;
        }
    }

    void set(int r, int c, int value)
    {
        if(inBounds(r,c))
        {
            data[r][c] = value;
        }
        else
        {
            cout << "Invalid index entered!\n";
        }
    }

    void rotate90Clockwise()
    {
        int** temp = new int*[cols];

        for(int i=0 ; i<cols ; i++)
        {
            temp[i] = new int[rows];
            for(int j=0 ; j<rows ; j++)
            {
                temp[i][j] = 0;
            }
        }
        
        // Created a temporary Array to store the result of rotated Array.

        for(int i=0 ; i<cols ; i++)
        {
            for(int j=0 ; j<rows ; j++)
            {
                temp[i][j] = data[rows-j-1][i];
            }
        }

        for(int i=0 ; i<rows ; i++)
        {
            delete[] data[i];
        }
        delete[] data;
        int tempVar = rows;
        rows = cols;
        cols = tempVar;
        data = temp;
        temp = nullptr;
    }

    void flipHorizontal()
    {
        
        for(int i=0 ; i<rows ; i++)
        {
            for(int j=0 ; j<cols/2 ; j++)
            {
                int temp = data[i][j];
                data[i][j] = data[i][cols-j-1];
                data[i][cols-j-1]  = temp;
            }
        }
    }

    void histogram(int *outCounts /* size 256, pre-zeroed by caller */)
    {
        for(int i=0 ; i<rows ; i++)
        {
            for(int j=0 ; j<cols ; j++)
            {
                outCounts[get(i,j)] += 1;
            }
        }
    }

    void applyBoxBlur()
    {
        int** temp = new int*[rows];
        for (int i = 0; i < rows; i++)
        {
            temp[i] = new int[cols];
            for (int j = 0; j < cols; j++)
                temp[i][j] = 0;
        }

        // created a temporary Array to store the result of Box Blur.

        for(int i=0 ; i<rows ; i++)
        {
            for(int j=0 ; j<cols ; j++)
            {
                int count = 1;
                int sum = data[i][j];
                
                // if conditions to check all 8 sides and calculate sum and avg to give to the Box Blured array
                if(inBounds(i-1,j-1))
                {
                    sum += data[i-1][j-1];
                    count++;
                }

                if(inBounds(i-1,j))
                {
                    sum += data[i-1][j];
                    count++;
                }

                if(inBounds(i-1,j+1))
                {
                    sum += data[i-1][j+1];
                    count++;
                }

                if(inBounds(i,j-1))
                {
                    sum += data[i][j-1];
                    count++;
                }

                if(inBounds(i,j+1))
                {
                    sum += data[i][j+1];
                    count++;
                }

                if(inBounds(i+1,j-1))
                {
                    sum += data[i+1][j-1];
                    count++;
                }

                if(inBounds(i+1,j))
                {
                    sum += data[i+1][j];
                    count++;
                }

                if(inBounds(i+1,j+1))
                {
                    sum += data[i+1][j+1];
                    count++;
                }

                int avg = sum/count;
                temp[i][j] = avg;
            }
        }

        for(int i=0 ; i<rows ; i++)
        {
            delete[] data[i];
        }

        delete[] data;

        data = temp;
        temp = nullptr;
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
        else
        {
            cout << "Unknown command: " << cmd << endl;
        }
    }

    return 0;
}
