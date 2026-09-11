#include <iostream>
#include <string>
using namespace std;

struct Message
{
    int id;
    string role;
    string text;
    Message *prev;
    Message *next;

    Message(int _id, string _role, string _text)
    {
        id = _id;
        role = _role;
        text = _text;
        prev = nullptr;
        next = nullptr;
    }
};

class ChatHistory
{
private:
    Message *head;
    Message *tail;
    int count;

public:
    ChatHistory()
    {
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    ~ChatHistory()
    {
        for(int i = 0; i < count; i++)
        {
            Message *temp = head;
            head = head->next;
            delete temp;
            temp = nullptr;
        }
    }

    int size() { return count; }

    void addMessage(int id, string role, string text)
    {
        Message* newMsg = new Message(id,role,text);

        if(head == nullptr)
        {
            head = newMsg;
            tail = newMsg;
        }
        else
        {
            tail->next = newMsg;
            newMsg->prev = tail;
            tail = newMsg;
        }

        count++;
    }

    Message *findById(int id)
    {
        Message* temp = head;
        for(int i=0 ; i<count ; i++)
        {
            if(temp->id == id)
            {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void deleteMessage(int id)
    {
        bool found = false;
        Message* temp = head;
        for(int i=0 ; i<count ; i++)
        {
            if(temp->id == id)
            {
                found = true;
                break;
            }
            temp = temp->next;
        }

        if(found)
        {
            if(count == 1)
            {
                head = tail = nullptr;
                delete temp;
                temp = nullptr;
            }
            else if(temp == head)
            {
                head = head->next;
                head->prev = nullptr;
                delete temp;
                temp = nullptr;
            }

            else if(temp == tail)
            {
                tail = tail->prev;
                tail->next = nullptr;
                delete temp;
                temp = nullptr;
            }
            else
            {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                delete temp;
                temp = nullptr;
            }

            count--;
        }
        else
        {
            cout << "ID not found.\n";
        }

        
    }

    void editMessage(int id, string newText)
    {
        bool found = false;
        Message* temp = head;
        for(int i=0 ; i<count ; i++)
        {
            if(temp->id == id)
            {
                found = true;
                break;
            }
            temp = temp->next;
        }

        if(found)
        {
            temp->text = newText;
        }
        else
        {
            cout << "ID not Found.\n";
        }
    }

    void removehead()
    {
        if(head == nullptr)
        {
            return;
        }
        else if(count == 1)
        {
            delete head;
            head = tail = nullptr;
            count--;
        }
        else
        {
            Message* temp = head;
            head = head->next;
            head->prev = nullptr;
            delete temp;
            temp = nullptr;
            count--;
        }
    }
    void evictOldestIfOverLimit(int maxMessages)
    {
        while(count > maxMessages)
        {
            removehead();
        }
    }

    void printConversation()
    {
        Message *cur = head;
        while (cur != nullptr)
        {
            cout << "[" << cur->id << "] " << cur->role << ": " << cur->text << endl;
            cur = cur->next;
        }
    }
};

int main()
{
    ChatHistory chat;
    string cmd;

    while (cin >> cmd)
    {
        if (cmd == "ADD")
        {
            int id;
            string role, text;
            cin >> id >> role;
            cin.ignore();
            getline(cin, text);
            chat.addMessage(id, role, text);
        }
        else if (cmd == "DELETE")
        {
            int id;
            cin >> id;
            chat.deleteMessage(id);
        }
        else if (cmd == "EDIT")
        {
            int id;
            string newText;
            cin >> id;
            cin.ignore();
            getline(cin, newText);
            chat.editMessage(id, newText);
        }
        else if (cmd == "LIMIT")
        {
            int n;
            cin >> n;
            chat.evictOldestIfOverLimit(n);
            chat.printConversation();
        }
        else if (cmd == "PRINT")
        {
            chat.printConversation();
        }
        else
        {
            cout << "Invalid command.\n";
        }
    }

    return 0;
}
