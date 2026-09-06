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
        // TODO: properly delete the Message Data Structure
    }

    int size() { return count; }

    void addMessage(int id, string role, string text)
    {
        // TODO: create a new Message node and append it at the tail.
    }

    Message *findById(int id)
    {
        // TODO: linear search from head, return node pointer or nullptr.
        return nullptr;
    }

    void deleteMessage(int id)
    {
        // TODO: find the node by id, then relink prev/next around it.
    }

    void editMessage(int id, string newText)
    {
        // TODO: find the node by id and overwrite its text field.
    }

    void evictOldestIfOverLimit(int maxMessages)
    {
        // TODO: while count > maxMessages, remove the head node
        // (reuse/adapt logic from deleteMessage or write a dedicated
        // removeHead() helper).
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
        // TODO (optional): handle malformed/unknown commands gracefully.
    }

    return 0;
}
