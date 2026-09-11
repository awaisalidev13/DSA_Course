#include <iostream>
#include <string>
using namespace std;

struct Job
{
    int jobId;
    int remainingTime;
    int burstTime;      // original burst time, useful for reporting
    int completionTime; // filled in when job finishes
    Job *next;

    Job(int id, int burst)
    {
        jobId = id;
        remainingTime = burst;
        burstTime = burst;
        completionTime = -1;
        next = nullptr;
    }
};

class GPUScheduler
{
private:
    Job *current; // points to the "current" job in the circle
    int jobCount;
    int clock; // simulated time elapsed so far

public:
    GPUScheduler()
    {
        current = nullptr;
        jobCount = 0;
        clock = 0;
    }

    ~GPUScheduler()
    {
        for(int i=0 ; i<jobCount ; i++)
        {
            Job* temp = current;
            current = current->next;
            delete temp;
            temp = nullptr;
        }
    }

    bool isEmpty()
    {
        return jobCount == 0;
    }

    void addJob(int jobId, int burstTime)
    {
        Job* newJob = new Job(jobId, burstTime);
        if(current == nullptr)
        {
            current = newJob;
            current->next = newJob;
        }
        else
        {
            newJob->next = current->next;
            current->next = newJob;
            current = newJob;
        }

        jobCount++;
    }

    void removeJob(int jobId)
    {
        //If the list is empty
        if(current == nullptr)
        {
            return;
        }
        //If the Job is at the end or tail
        if(current->jobId == jobId)
        {
            Job* temp = current;
            current = current->next;
            delete temp;
            temp = nullptr;
            jobCount--;
            return;
        }

        //If the job is at the head
        else if(current->next->jobId == jobId)
        {
            Job* temp = current->next;
            current->next = temp->next;
            delete temp;
            temp = nullptr;
            jobCount--;
            return;
        }

        //if the Job lies in the middle
        Job* temp = current->next;
        bool found = false;
        for(int i=0 ; i<jobCount-2 ; i++)
        {
            if(temp->next->jobId == jobId)
            {
                found = true;
                break;
            }
            temp = temp->next;
        }

        if(!found)
        {
            return;
        }

        Job* temp2 = temp->next;

        temp->next = temp->next->next;
        delete temp2;
        jobCount--;
        temp2= nullptr;
        
    }

    // Runs one time slice for the current job. Prints what happened.
    // Returns false if the scheduler is empty (nothing to run).
    bool runNextSlice(int quantum)
    {
        if(current == nullptr)
        {
           return false;
        }
        if((current->remainingTime - quantum) > 0)
        {
            current->remainingTime = current->remainingTime - quantum;
            clock += quantum;
            cout << "Running Job " << current->jobId << ", remaining time: " << current->remainingTime << endl;
            current = current->next;
            return true;
        }
        else
        {
            current->completionTime = clock + current->remainingTime;
            clock += current->remainingTime;
            cout << "Job " << current->jobId << " finished " << endl;
            removeJob(current->jobId);
            return true;
        }
        
    }

    void simulateAll(int quantum)
    {
        while (!isEmpty())
        {
            runNextSlice(quantum);
        }
        cout << "Total simulation time: " << clock << endl;
    }
};

int main()
{
    int n;
    cin >> n;

    GPUScheduler scheduler;
    for (int i = 0; i < n; i++)
    {
        int id, burst;
        cin >> id >> burst;
        scheduler.addJob(id, burst);
    }

    int quantum;
    cin >> quantum;

    scheduler.simulateAll(quantum);

    return 0;
}
