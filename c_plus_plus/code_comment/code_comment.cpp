#include <iostream>
#include "Function.h"

using namespace std;

int main()
{
    json jsConf;
    parseConfig(jsConf);
    cout << endl << "configure json = " << jsConf.dump(4) << endl << endl;
    commentCpp(jsConf);
    cout << "done!" << endl << endl;
}