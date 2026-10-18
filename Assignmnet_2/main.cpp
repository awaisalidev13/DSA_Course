#include <iostream>
#include "BST.h"
using namespace std;

int main()
{
    BST tree;

    // insertion order: 105, 102, 108, 101, 103, 107, 110
    tree.insert(Record(105, 1, 5, 50));
    tree.insert(Record(102, 1, 2, 20));
    tree.insert(Record(108, 1, 8, 80));
    tree.insert(Record(101, 1, 1, 10));
    tree.insert(Record(103, 1, 3, 30));
    tree.insert(Record(107, 1, 7, 70));
    tree.insert(Record(110, 1, 10, 100));

    cout << "In order: "; tree.inOrderPrint(); cout << endl;

    cout << "\n-- FIND_JOB --" << endl;
    tree.findJob(103);
    tree.findJob(999);

    cout << "\n-- RANGE 102 108 --" << endl;
    tree.range(102, 108);
    cout << "\n-- RANGE 200 300 (empty) --" << endl;
    tree.range(200, 300);
    cout << "\n-- RANGE 108 102 (invalid) --" << endl;
    tree.range(108, 102);

    cout << "\n-- KTH --" << endl;
    tree.kth(1);
    tree.kth(4);
    tree.kth(7);
    tree.kth(8);
    tree.kth(0);

    cout << "\n-- DELETE --" << endl;
    tree.deleteRecord(101);   // leaf
    cout << "In order: "; tree.inOrderPrint(); cout << endl;
    tree.deleteRecord(108);   // two children
    cout << "In order: "; tree.inOrderPrint(); cout << endl;
    tree.deleteRecord(105);   // root, two children
    cout << "In order: "; tree.inOrderPrint(); cout << endl;
    tree.deleteRecord(102);   // one child
    cout << "In order: "; tree.inOrderPrint(); cout << endl;
    tree.deleteRecord(999);   // not stored

    cout << "\n-- Records still correct after deletes --" << endl;
    tree.findJob(107);
    tree.findJob(110);

    return 0;
}