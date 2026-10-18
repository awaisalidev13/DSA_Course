#pragma once
#include <iostream>
#include "Tree.h"
#include "Queue.h"
using namespace std;

void showLevels(TreeNode* root, int folderId)
{
    TreeNode* start = findNodeById(root, folderId);

    if(start == nullptr)
    {
        cout << "Error: folder not found." << endl;
        return;
    }
    if(start->type == 0)
    {
        cout << "Error: that ID is a file, not a folder." << endl;
        return;
    }

    Queue<TreeNode*> q;   // separate from the print queue
    q.enqueue(start);
    int level = 0;

    while(!q.isEmpty())
    {
        int n = q.size();   // nodes in this level
        cout << "Level " << level << ": ";

        for(int i = 0; i < n; i++)
        {
            TreeNode* node = q.peekFront();
            q.dequeue();

            cout << node->name;
            if(i < n - 1)
            {
                cout << ", ";
            }

            for(int j = 0; j < node->children.size(); j++)
            {
                q.enqueue(node->children[j]);
            }
        }

        cout << endl;
        level++;
    }
}

// returns the height of this subtree, and adds up files and pages
int summaryHelper(TreeNode* node, int& files, int& pages)
{
    if(node->type == 0)
    {
        files++;
        pages = pages + node->pages;
        return 0;
    }

    int height = 0;
    for(int i = 0; i < node->children.size(); i++)
    {
        int childHeight = summaryHelper(node->children[i], files, pages);
        if(childHeight + 1 > height)
        {
            height = childHeight + 1;
        }
    }
    return height;
}

void summary(TreeNode* root, int folderId)
{
    TreeNode* start = findNodeById(root, folderId);

    if(start == nullptr)
    {
        cout << "Error: folder not found." << endl;
        return;
    }
    if(start->type == 0)
    {
        cout << "Error: that ID is a file, not a folder." << endl;
        return;
    }

    int files = 0;
    int pages = 0;
    int height = summaryHelper(start, files, pages);

    cout << "Files: " << files
         << " | Total pages: " << pages
         << " | Height: " << height << endl;
}

int depthOf(TreeNode* node)
{
    int d = 0;
    while(node->parent != nullptr)
    {
        d++;
        node = node->parent;
    }
    return d;
}

void commonFolder(TreeNode* root, int id1, int id2)
{
    TreeNode* f1 = findNodeById(root, id1);
    TreeNode* f2 = findNodeById(root, id2);

    if(f1 == nullptr || f2 == nullptr)
    {
        cout << "Error: file not found." << endl;
        return;
    }
    if(f1->type == 1 || f2->type == 1)
    {
        cout << "Error: both IDs must be files." << endl;
        return;
    }

    // start from each file's parent folder
    TreeNode* a = f1->parent;
    TreeNode* b = f2->parent;

    int da = depthOf(a);
    int db = depthOf(b);

    while(da > db)
    {
        a = a->parent;
        da--;
    }
    while(db > da)
    {
        b = b->parent;
        db--;
    }

    while(a != b)
    {
        a = a->parent;
        b = b->parent;
    }

    cout << getPath(a) << endl;
}