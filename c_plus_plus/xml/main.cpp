#include "tinyxml2.h"
#include <iostream>

using namespace std;
using namespace tinyxml2;

int main()
{
	const char* szXML = "123";

	XMLDocument XMLDocument_;

	if (XMLError::XML_SUCCESS == XMLDocument_.Parse(szXML))
	{
		cout << 1 << endl;
	}
	else
	{
		cout << 0 << endl;
	}

	return 0;
}