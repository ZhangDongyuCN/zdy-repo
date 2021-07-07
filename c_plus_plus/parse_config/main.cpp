#include <iostream>
#include <string>

#include"ParseConfig.hpp"

using namespace std;

using ParseConfig = sinicheveen::ParseConfig;

int main()
{
    try
    {
		ParseConfig::s_strConfFilePath("collateMarkdownArtile.conf");
        ParseConfig::parseConfFileIntoMap();
        ParseConfig::printConfInMap();
		cout << ParseConfig::getSpecConfValFromFile("[SECTION2]","G") << endl;

        return 0;
    }
    catch (string e)
    {
        cout << e << endl;
        return 0;
    }
}