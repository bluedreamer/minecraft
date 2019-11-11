#pragma once

#include <string>
#include <fstream>

class Reader
{
public:
   Reader(const std::string &filename);
private:
   std::ifstream stream_;
};