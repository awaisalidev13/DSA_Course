#pragma once
#include "queue.h"
#include "tree.h"
#include "job.h"

class PrintCenter{
    private:
        TreeNode* root;
        Queue<Job> waiting;
        int nextjobId;
        int printerClock;

    public:
        PrintCenter(TreeNode* r)
        {
            root = r;
            nextjobId = 101;
            printerClock = 0;
        }

        void printFile(int fileId, int copies)
        {
            TreeNode* fileNode = findNodeById(root, fileId);

            if(fileNode == nullptr )
            {
                cout << "Error File Not Found" << endl;
                return;
            }

            if(fileNode->type == 1)
            {
                cout << "Error Cannot Print Folder" << endl;
                return;
            }

            if(copies <= 0)
            {
                cout << "Error Invalid Number of Copies" << endl;
                return;
            }

            int totalPages = fileNode->pages * copies;
            int remainingPages = totalPages;
            Job newJob(nextjobId, fileId, totalPages, remainingPages);
            waiting.enqueue(newJob);
            nextjobId++;
            cout << "Job " << newJob.jobId << " added to the queue for file " << fileNode->name << " with " << totalPages << " pages." << endl;
        }

        //test
        void showQueue()
        {
            if(waiting.isEmpty())
            {
                cout << "Queue is empty." << endl;
                return;
            }

            int n = waiting.size();
            for(int i = 0; i < n; i++)
            {
                Job job = waiting.peekFront();
                cout << "Job " << job.jobId << " for file " << job.fileId << " with " << job.remainingPages << " pages." << endl;
                waiting.dequeue();
                waiting.enqueue(job);
            }
        }

        void run(int turns)
        {
            if(turns <= 0)
            {
                cout << "Error Invalid Number of Turns" << endl;
                return;
            }

            for(int i = 0; i < turns; i++)
            {
                if(waiting.isEmpty())
                {
                    cout << "Queue is empty. Stopping early." << endl;
                    return;
                }

                Job job = waiting.peekFront();
                waiting.dequeue();

                int pagesPrinted = 2;
                if(job.remainingPages < 2)
                {
                    pagesPrinted = job.remainingPages;
                }

                job.remainingPages = job.remainingPages - pagesPrinted;
                printerClock = printerClock + pagesPrinted;

                cout << "Turn " << (i + 1) << ": Job " << job.jobId
                    << " printed " << pagesPrinted
                    << ", left " << job.remainingPages
                    << ", clock " << printerClock << endl;

                if(job.remainingPages > 0)
                {
                    waiting.enqueue(job);
                }
                else
                {
                    // placeholder: replace with a BST insert in Q4
                    cout << "Job " << job.jobId << " completed at time " << printerClock << endl;
                }
            }
        }

        void cancel(int jobId)
        {
            if(waiting.isEmpty())
            {
                cout << "Job Not Found." << endl;
                return;
            }

            int n = waiting.size();
            bool found = false;
            for(int i=0 ; i<n ; i++)
            {
                Job job = waiting.peekFront();
                waiting.dequeue();
                if(job.jobId == jobId)
                {
                    found = true;
                    cout << "Job " << job.jobId << " Cancelled." << endl; 
                    continue;
                }

                waiting.enqueue(job);
            }

            if(!found)
            {
                cout << "Job Not Found." << endl;
                return;
            }


        }
};