#pragma once

template <typename T>
struct QueueNode{
    T data;
    QueueNode<T>* next;

    QueueNode(T value)
    {
        data = value;
        next = nullptr;
    }
};

template <typename T>
class Queue{
    private:
        QueueNode<T>* front;
        QueueNode<T>* rear;
        int count;

    public:
        Queue()
        {
            front = rear = nullptr;
            count = 0;
        }

        void enqueue(T value)
        {
            QueueNode<T>* newNode = new QueueNode<T>(value);
            count++;
            if(front == nullptr)
            {
                front = rear = newNode;
                return;
            }

            rear->next = newNode;
            rear = newNode;
        }

        void dequeue()
        {
            if(front == nullptr)
            {
                return;
            }

            QueueNode<T>* temp = front;
            front = front->next;

            delete temp;
            count--;

            if(front == nullptr)
            {
                rear = nullptr;
            }
        }

        T peekFront()
        {
            if(front == nullptr) return T();

            return front->data;
        }

        bool isEmpty()
        {
            return front == nullptr;
        }

        int size()
        {
            return count;
        }

        void clear()
        {
            while(front != nullptr)
            {
                dequeue();
            }
        }

        ~Queue()
        {
            clear();
        }
};