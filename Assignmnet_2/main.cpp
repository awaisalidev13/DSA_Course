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

int main()
{
    TreeNode* root = buildSampleTree();
    Navigation nav(root);
    PrintCenter pc(root);

    cout << ">> OPEN /CS/DSA" << endl;         nav.open("/CS/DSA");
    cout << ">> PRINT_FOLDER 1 DEPTH" << endl; pc.printFolder(nav.getCurrent(), 1, "DEPTH");
    cout << ">> RUN 3" << endl;                pc.run(3);
    cout << ">> BACK 1" << endl;               nav.back(1);
    cout << ">> OPEN /Admin" << endl;          nav.open("/Admin");
    cout << ">> PRINT_FOLDER 1 LEVEL" << endl; pc.printFolder(nav.getCurrent(), 1, "LEVEL");
    cout << ">> CANCEL 102" << endl;           pc.cancel(102);
    cout << ">> RUN 10" << endl;               pc.run(10);
    cout << ">> WHERE" << endl;                nav.where();
    cout << ">> SHOW_QUEUE" << endl;           pc.showQueue();
    cout << ">> FIND_JOB 102" << endl;         pc.findJob(102);
    cout << ">> RANGE 101 104" << endl;        pc.range(101, 104);
    cout << ">> KTH 2" << endl;                pc.kth(2);

    return 0;
}