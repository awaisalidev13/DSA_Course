#pragma once
#include <iostream>
#include "Stack.h"
using namespace std;

struct Record{
    int jobId;
    int fileId;
    int totalPages;
    int completionTime;

    Record()
    {
        jobId = 0;
        fileId = 0;
        totalPages = 0;
        completionTime = 0;
    }

    Record(int jobId, int fileId, int totalPages, int completionTime)
    {
        this->jobId = jobId;
        this->fileId = fileId;
        this->totalPages = totalPages;
        this->completionTime = completionTime;
    }
};

struct BSTNode{
    Record data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Record r)
    {
        data = r;
        left = nullptr;
        right = nullptr;
    }
};

class BST{
    private:
        BSTNode* root;
        int count;

        void printRecord(const Record& r)
        {
            cout << "Job " << r.jobId
                 << " | File " << r.fileId
                 << " | Pages " << r.totalPages
                 << " | Completed at " << r.completionTime << endl;
        }

        BSTNode* insertHelper(BSTNode* current, Record r)
        {
            if(current == nullptr)
            {
                return new BSTNode(r);
            }

            if(r.jobId < current->data.jobId)
            {
                current->left = insertHelper(current->left, r);
            }
            else
            {
                current->right = insertHelper(current->right, r);
            }

            return current;
        }

        void destroy(BSTNode* current)
        {
            if(current == nullptr)
            {
                return;
            }

            destroy(current->left);
            destroy(current->right);
            delete current;
        }

        void inOrderHelper(BSTNode* current)
        {
            if(current == nullptr)
            {
                return;
            }

            inOrderHelper(current->left);
            cout << current->data.jobId << " ";
            inOrderHelper(current->right);
        }

        bool exists(int jobId)
        {
            BSTNode* current = root;
            while(current != nullptr)
            {
                if(jobId == current->data.jobId)
                {
                    return true;
                }
                if(jobId < current->data.jobId)
                {
                    current = current->left;
                }
                else
                {
                    current = current->right;
                }
            }
            return false;
        }

        // visits only branches that can contain IDs in [low, high]
        void rangeHelper(BSTNode* current, int low, int high, int& matches, int& totalPages)
        {
            if(current == nullptr)
            {
                return;
            }

            if(low < current->data.jobId)
            {
                rangeHelper(current->left, low, high, matches, totalPages);
            }

            if(current->data.jobId >= low && current->data.jobId <= high)
            {
                printRecord(current->data);
                matches++;
                totalPages = totalPages + current->data.totalPages;
            }

            if(high > current->data.jobId)
            {
                rangeHelper(current->right, low, high, matches, totalPages);
            }
        }

        BSTNode* findMin(BSTNode* current)
        {
            while(current->left != nullptr)
            {
                current = current->left;
            }
            return current;
        }

        // only called when the job is known to exist
        BSTNode* deleteHelper(BSTNode* current, int jobId)
        {
            if(current == nullptr)
            {
                return nullptr;
            }

            if(jobId < current->data.jobId)
            {
                current->left = deleteHelper(current->left, jobId);
            }
            else if(jobId > current->data.jobId)
            {
                current->right = deleteHelper(current->right, jobId);
            }
            else
            {
                // no left child (covers leaf and one right child)
                if(current->left == nullptr)
                {
                    BSTNode* child = current->right;
                    delete current;
                    return child;
                }

                // only a left child
                if(current->right == nullptr)
                {
                    BSTNode* child = current->left;
                    delete current;
                    return child;
                }

                // two children: copy the in-order successor's whole record,
                // then delete the successor from the right subtree
                BSTNode* successor = findMin(current->right);
                current->data = successor->data;
                current->right = deleteHelper(current->right, successor->data.jobId);
            }

            return current;
        }

    public:
        BST()
        {
            root = nullptr;
            count = 0;
        }

        void insert(Record r)
        {
            root = insertHelper(root, r);
            count++;
        }

        void findJob(int jobId)
        {
            BSTNode* current = root;

            while(current != nullptr)
            {
                if(jobId == current->data.jobId)
                {
                    printRecord(current->data);
                    return;
                }

                if(jobId < current->data.jobId)
                {
                    current = current->left;
                }
                else
                {
                    current = current->right;
                }
            }

            cout << "Completed job not found." << endl;
        }

        void range(int low, int high)
        {
            if(low > high)
            {
                cout << "Error: low must not be greater than high." << endl;
                return;
            }

            int matches = 0;
            int totalPages = 0;

            rangeHelper(root, low, high, matches, totalPages);

            if(matches == 0)
            {
                cout << "No matching records." << endl;
            }
            cout << "Total pages: " << totalPages << endl;
        }

        void kth(int k)
        {
            if(k <= 0 || k > count)
            {
                cout << "Error: invalid k." << endl;
                return;
            }

            Stack<BSTNode*> st;
            BSTNode* current = root;
            int position = 0;

            while(current != nullptr || !st.isEmpty())
            {
                while(current != nullptr)
                {
                    st.push(current);
                    current = current->left;
                }

                current = st.peek();
                st.pop();

                position++;
                if(position == k)
                {
                    printRecord(current->data);
                    return;
                }

                current = current->right;
            }
        }

        void deleteRecord(int jobId)
        {
            if(!exists(jobId))
            {
                cout << "Completed job not found." << endl;
                return;
            }

            root = deleteHelper(root, jobId);
            count--;
            cout << "Record for job " << jobId << " deleted." << endl;
        }

        int size()
        {
            return count;
        }

        // for testing and the report diagram
        void inOrderPrint()
        {
            inOrderHelper(root);
        }

        ~BST()
        {
            destroy(root);
        }
};