#pragma once
#include "queue.h"
#include "tree.h"
#include "job.h"
#include "stack.h"
#include "BST.h"

class PrintCenter{
    private:
        TreeNode* root;
        Queue<Job> waiting;
        int nextjobId;
        int printerClock;
        BST records;
    
        void addJob(TreeNode* fileNode, int copies)
        {
            int totalPages = fileNode->pages * copies;
            Job newJob(nextjobId, fileNode->id, totalPages, totalPages);
            waiting.enqueue(newJob);
            cout << "Job " << newJob.jobId << " added for file " << fileNode->name
                << " with " << totalPages << " pages." << endl;
            nextjobId++;
        }
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
                    records.insert(Record(job.jobId, job.fileId, job.totalPages, printerClock));
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

        void printFolder(TreeNode* folder, int copies, string order)
        {
            if(folder == nullptr)
            {
                cout << "Error: invalid folder." << endl;
                return;
            }
            if(copies <= 0)
            {
                cout << "Error Invalid Number of Copies" << endl;
                return;
            }
            if(order != "DEPTH" && order != "LEVEL")
            {
                cout << "Error: order must be DEPTH or LEVEL." << endl;
                return;
            }

            if(order == "DEPTH")
            {
                Stack<TreeNode*> st;
                st.push(folder);

                while(!st.isEmpty())
                {
                    TreeNode* node = st.peek();
                    st.pop();

                    if(node->type == 0)
                    {
                        addJob(node, copies);
                    }
                    else
                    {
                        // push in reverse so the first child is popped first
                        for(int i = node->children.size() - 1; i >= 0; i--)
                        {
                            st.push(node->children[i]);
                        }
                    }
                }
            }
            else
            {
                Queue<TreeNode*> q;   // separate from the print queue
                q.enqueue(folder);

                while(!q.isEmpty())
                {
                    TreeNode* node = q.peekFront();
                    q.dequeue();

                    if(node->type == 0)
                    {
                        addJob(node, copies);
                    }
                    else
                    {
                        for(int i = 0; i < node->children.size(); i++)
                        {
                            q.enqueue(node->children[i]);
                        }
                    }
                }
            }
        }

        void findJob(int jobId)       { records.findJob(jobId); }
        void range(int low, int high) { records.range(low, high); }
        void kth(int k)               { records.kth(k); }
        void deleteRecord(int jobId)  { records.deleteRecord(jobId); }
};