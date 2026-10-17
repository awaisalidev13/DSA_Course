#pragma once
#include "Tree.h"
#include "Stack.h"

class Navigation{
    private:
        TreeNode* root;
        TreeNode* current;
        Stack<int> backStack;
        Stack<int> forwardStack;

        
        TreeNode* stepInto(TreeNode* node, string piece)
        {
            if(piece == ".")
            {
                return node;
            }

            if(piece == "..")
            {
                if(node->parent != nullptr)
                {
                    return node->parent;
                }
                return node;   
            }

            for(int i = 0; i < node->children.size(); i++)
            {
                TreeNode* child = node->children[i];
                if(child->name == piece && child->type == 1)
                {
                    return child;
                }
            }

            return nullptr;
        }

        TreeNode* resolvePath(string path)
        {
            if(path == "")
            {
                return nullptr;
            }

            TreeNode* walker;

            if(path[0] == '/')
            {
                walker = root;
            }
            else
            {
                walker = current;
            }

            string piece = "";

            for(int i = 0; i < path.length(); i++)
            {
                if(path[i] == '/')
                {
                    if(piece != "")
                    {
                        walker = stepInto(walker, piece);
                        if(walker == nullptr)
                        {
                            return nullptr;
                        }
                        piece = "";
                    }
                }
                else
                {
                    piece = piece + path[i];
                }
            }

            // the last piece has no '/' after it, so handle it here
            if(piece != "")
            {
                walker = stepInto(walker, piece);
                if(walker == nullptr)
                {
                    return nullptr;
                }
            }

            return walker;
        }
        void printStack(Stack<int>& s, string label)
        {
            cout << label << endl;

            if(s.isEmpty())
            {
                cout << "empty" << endl;
                return;
            }

            Stack<int> temp;

            while(!s.isEmpty())
            {
                int id = s.peek();
                cout << getPath(findNodeById(root, id)) << endl;
                temp.push(id);
                s.pop();
            }

            while(!temp.isEmpty())
            {
                s.push(temp.peek());
                temp.pop();
            }
        }

    public:
        Navigation(TreeNode* r)
        {
            root = r;
            current = r;
        }

        void open(string path)
        {
            TreeNode* target = resolvePath(path);

            if(target == nullptr)
            {
                cout << "Error Invalid Path" << endl;
                return;
            }

            if(target == current)
            {
                return;
            }
            else
            {
                backStack.push(current->id);
                current = target;
                forwardStack.clear();
            }
            
        }

        void where()
        {
            cout << getPath(current) << endl;
        }

        void back(int k)
        {
            if(k <= 0)
            {
                cout << "Error Invalid Argument" << endl;
                return;
            }

            for(int i = 0; i < k; i++)
            {
                if(backStack.isEmpty())
                {
                    return;
                }

                forwardStack.push(current->id);
                int prevId = backStack.peek();
                backStack.pop();
                current = findNodeById(root, prevId);
            }
        }

        void forward(int k)
        {
            if(k <= 0)
            {
                cout << "Error Invalid Argument" << endl;
                return;
            }

            for(int i = 0; i < k; i++)
            {
                if(forwardStack.isEmpty())
                {
                    return;
                }

                backStack.push(current->id);
                int nextId = forwardStack.peek();
                forwardStack.pop();
                current = findNodeById(root, nextId);
            }
        }

        void showHistory()
        {
            printStack(backStack, "Back stack (top first):");
            printStack(forwardStack, "Forward stack (top first):");
        }
};