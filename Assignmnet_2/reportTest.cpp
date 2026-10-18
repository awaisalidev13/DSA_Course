#include <iostream>
#include "Stack.h"
#include "Queue.h"
#include "Tree.h"
#include "Job.h"
#include "BST.h"
#include "Navigation.h"
#include "PrintCenter.h"
#include "FolderOps.h"
using namespace std;

void title(string s)
{
    cout << "\n########## " << s << " ##########" << endl;
}

int main()
{
    TreeNode* root = buildSampleTree();

    // ================= Q1 =================
    title("Q1 assignment example");
    {
        Navigation nav(root);
        nav.open("/CS/DSA");    nav.where();
        nav.open("Trees");      nav.where();
        nav.back(1);            nav.where();
        nav.open("Missing");    nav.where();
        nav.forward(1);         nav.where();
        nav.back(1);            nav.where();
        nav.open("../AI");      nav.where();
        nav.forward(1);         nav.where();
        nav.showHistory();
    }

    title("Q1 extra test 1: empty history");
    {
        Navigation nav(root);
        nav.back(1);
        nav.forward(1);
        nav.where();
    }

    title("Q1 extra test 2: invalid k");
    {
        Navigation nav(root);
        nav.back(0);
        nav.back(-3);
        nav.where();
    }

    title("Q1 extra test 3: k larger than history");
    {
        Navigation nav(root);
        nav.open("/CS/DSA");
        nav.open("Trees");
        nav.back(10);
        nav.where();
        nav.forward(10);
        nav.where();
    }

    title("Q1 extra test 4: failed path and same-folder open");
    {
        Navigation nav(root);
        nav.open("/CS/DSA");
        nav.open("/CS/Missing/../DSA");
        nav.where();
        nav.open("Trees");
        nav.back(1);
        nav.open(".");
        nav.forward(1);
        nav.where();
        nav.showHistory();
    }

    // ================= Q2 =================
    title("Q2 assignment example");
    {
        PrintCenter pc(root);
        pc.printFile(7, 1);
        pc.printFile(8, 1);
        pc.printFile(10, 1);
        cout << "\n-- RUN 4 --" << endl;
        pc.run(4);
        cout << "\n-- queue after 4 turns --" << endl;
        pc.showQueue();
        cout << "\n-- RUN 100 --" << endl;
        pc.run(100);
    }

    title("Q2 extra test 1: cancel after some printing");
    {
        PrintCenter pc(root);
        pc.printFile(7, 1);
        pc.printFile(8, 1);
        pc.printFile(10, 1);
        pc.run(2);
        pc.cancel(102);
        pc.showQueue();
        pc.cancel(999);
        pc.cancel(102);
        pc.run(100);
    }

    title("Q2 extra test 2: cancel on empty queue");
    {
        PrintCenter pc(root);
        pc.cancel(101);
        pc.showQueue();
    }

    title("Q2 extra test 3: rejected requests use no job ID");
    {
        PrintCenter pc(root);
        pc.printFile(99, 1);
        pc.printFile(3, 1);
        pc.printFile(7, 0);
        pc.printFile(7, 1);
        pc.printFile(7, 2);
        pc.showQueue();
    }

    // ================= Q3 =================
    title("Q3 SHOW_LEVELS and SUMMARY");
    showLevels(root, 3);
    summary(root, 3);
    summary(root, 6);
    showLevels(root, 7);

    title("Q3 COMMON_FOLDER");
    commonFolder(root, 7, 10);
    commonFolder(root, 7, 11);
    commonFolder(root, 7, 5);
    commonFolder(root, 7, 7);
    commonFolder(root, 3, 7);

    title("Q3 PRINT_FOLDER DEPTH from /CS/DSA, then RUN");
    {
        Navigation nav(root);
        PrintCenter pc(root);
        nav.open("/CS/DSA");
        pc.printFolder(nav.getCurrent(), 1, "DEPTH");
        pc.run(100);
    }

    title("Q3 PRINT_FOLDER LEVEL from /CS/DSA, then RUN");
    {
        Navigation nav(root);
        PrintCenter pc(root);
        nav.open("/CS/DSA");
        pc.printFolder(nav.getCurrent(), 1, "LEVEL");
        pc.run(100);
    }

    title("Q3 extra: empty folder, bad copies, bad order");
    {
        Navigation nav(root);
        PrintCenter pc(root);
        nav.open("/Empty");
        pc.printFolder(nav.getCurrent(), 1, "DEPTH");
        nav.open("/CS/DSA");
        pc.printFolder(nav.getCurrent(), 0, "DEPTH");
        pc.printFolder(nav.getCurrent(), 1, "SIDEWAYS");
        pc.printFile(7, 1);
    }

    // ================= Q4 =================
    title("Q4 assignment example (records from real printing)");
    {
        PrintCenter pc(root);
        pc.printFile(7, 1);
        pc.printFile(8, 1);
        pc.printFile(10, 1);
        pc.run(100);
        pc.findJob(101);
        pc.range(101, 102);
        pc.kth(2);
    }

    title("Q4 diagram: tree from completion order 102, 101, 103");
    {
        BST t;
        t.insert(Record(102, 8, 3, 9));
        t.insert(Record(101, 7, 5, 12));
        t.insert(Record(103, 10, 6, 14));
        cout << "In order: "; t.inOrderPrint(); cout << endl;
        t.deleteRecord(102);
        cout << "After deleting root 102, in order: "; t.inOrderPrint(); cout << endl;
        t.findJob(103);
        t.findJob(101);
    }

    title("Q4 extra tests: range, kth, all delete cases");
    {
        BST t;
        t.insert(Record(105, 1, 5, 50));
        t.insert(Record(102, 1, 2, 20));
        t.insert(Record(108, 1, 8, 80));
        t.insert(Record(101, 1, 1, 10));
        t.insert(Record(103, 1, 3, 30));
        t.insert(Record(107, 1, 7, 70));
        t.insert(Record(110, 1, 10, 100));

        cout << "In order: "; t.inOrderPrint(); cout << endl;
        t.range(102, 108);
        t.range(200, 300);
        t.range(108, 102);
        t.kth(1);
        t.kth(7);
        t.kth(8);
        t.kth(0);

        t.deleteRecord(101);
        cout << "After leaf 101: "; t.inOrderPrint(); cout << endl;
        t.deleteRecord(102);
        cout << "After one-child 102: "; t.inOrderPrint(); cout << endl;
        t.deleteRecord(108);
        cout << "After two-children 108: "; t.inOrderPrint(); cout << endl;
        t.deleteRecord(105);
        cout << "After root 105: "; t.inOrderPrint(); cout << endl;
        t.deleteRecord(999);
        t.findJob(107);
        t.findJob(110);
    }

    return 0;
}