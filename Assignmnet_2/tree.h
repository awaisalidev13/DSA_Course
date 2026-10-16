#pragma  once
#include <iostream>
#include <string>
#include <vector>
using namespace std;



struct TreeNode{
    int id;
    string name;
    bool type; // 0 for file, 1 for folder
    int pages; 
    TreeNode* parent;
    vector<TreeNode*> children;

    TreeNode(int id, string name, bool type, int pages, TreeNode* parent)
    {
        this->id = id;
        this->name = name;
        this->type = type;
        this->pages = pages;
        this->parent = parent;
    }
};


void addChild(TreeNode* parent, TreeNode* child)
{
    parent->children.push_back(child);
    child->parent = parent;
}

TreeNode* buildSampleTree()
{
    //FOLDERS
    TreeNode* root = new TreeNode(0, "Campus", 1, 0, nullptr);
    TreeNode* CS = new TreeNode(1, "CS", 1, 0, nullptr);
    TreeNode* Admin = new TreeNode(2, "Admin", 1, 0, nullptr);
    TreeNode* DSA = new TreeNode(3, "DSA", 1, 0, nullptr);
    TreeNode* AI = new TreeNode(4, "AI", 1, 0, nullptr);
    TreeNode* Empty = new TreeNode(6, "Empty", 1, 0, nullptr);
    TreeNode* Trees = new TreeNode(9, "Trees", 1, 0, nullptr);

    //FILES
    TreeNode* Notices_pdf = new TreeNode(5, "Notices.pdf", 0, 2, nullptr);
    TreeNode* Stacks_pdf = new TreeNode(7, "Stacks.pdf", 0, 5, nullptr);
    TreeNode* Queues_pdf = new TreeNode(8, "Queues.pdf", 0, 3, nullptr);
    TreeNode* BST_pdf = new TreeNode(10, "BST.pdf", 0, 6, nullptr);
    TreeNode* Search_pdf = new TreeNode(11, "Search.pdf", 0, 4, nullptr);

    addChild(root, CS);
    addChild(root, Admin);
    addChild(root, Empty);
    addChild(CS, DSA);
    addChild(CS, AI);
    addChild(DSA, Stacks_pdf);
    addChild(DSA, Trees);
    addChild(DSA, Queues_pdf);
    addChild(Trees, BST_pdf);
    addChild(AI, Search_pdf);
    addChild(Admin, Notices_pdf);
    
    return root;


}

TreeNode* findNodeById(TreeNode* root, int id)
{
    if(root == nullptr)
    {
        return nullptr;
    }
    if(root->id == id)
    {
        return root;
    }

    for(const auto&node : root->children)
    {

        TreeNode* res = findNodeById(node, id);

        if(res != nullptr)
        {
            return res;
        }
    }

    return nullptr;
}

string getPath(TreeNode* node)
{
    if(node == nullptr)
    {
        return "";
    }
    
    if(node->parent == nullptr)
    {
        return "/";
    }

    TreeNode* temp = node;

    string res = "";

    while(temp->parent != nullptr)
    {
        res = temp->name + res;
        res = "/" + res;
        temp = temp->parent;
    }

    return res;
}