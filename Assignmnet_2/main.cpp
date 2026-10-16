#include <iostream>
#include "stack.h"
#include "queue.h"
using namespace std;

int main()
{
    Stack s;
    s.push(10);
    s.push(20);
    s.push(30);

    cout << "Top element: " << s.peek() << endl;

    s.pop();
    cout << "Top element after pop: " << s.peek() << endl;

    return 0;
}
