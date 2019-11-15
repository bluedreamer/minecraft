#pragma once

#include <string>
#include <fstream>

#include "Tag.h"

class Reader
{
public:
   Reader(const std::string &filename);
   const Tag &GetRoot() const;
private:
   std::ifstream stream_;
   Tag root_tag_{};
};