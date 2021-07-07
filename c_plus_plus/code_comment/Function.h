#pragma once

#include <map>
#include <nlohmann/json.hpp>
#include <boost/regex/v4/regex.hpp>
#include <fstream>
#include <iostream>
#include <exception>
#include <memory>
#include <algorithm>

using namespace std;
using json = nlohmann::json;
using boost::regex;
using boost::regex_replace;

#define IN
#define OUT
#define INOUT

#define BUFFER_LEN 1024

void parseConfig(OUT json& jsConf);

void commentCpp(IN json& jsConf);
