#pragma once

template <typename T>
struct StackNode{
    T data;
    StackNode<T>* next;

    StackNode(T value)
    {
        data = value;
        next = nullptr;
    }
};

template <typename T>
class Stack{
    private:
        StackNode<T>* top;
        int count;

    public:
        Stack()
        {
            top = nullptr;
            count = 0;
        }

        void push(T value)
        {
            StackNode<T>* newStackNode = new StackNode<T>(value);
            newStackNode->next = top;
            top = newStackNode;
            count++;
        }

        void pop()
        {
            if(top == nullptr)
            {
                return;
            }

            StackNode<T>* temp = top;
            top = top->next;

            delete temp;
            count--;
        }

        T peek()
        {
            if(top == nullptr) return T();

            return top->data;
        }

        bool isEmpty()
        {
            return count == 0;
        }

        void clear()
        {
            while(top != nullptr)
            {
                pop();
            }
        }

        int size()
        {
            return count;
        }

        ~Stack()
        {
            clear();
        }
};