#include <iostream>

#include "Reader.h"
#include "Level.h"

int main(int argc, char* argv[])
{
   try
   {
      Reader reader("../cpp/save_files/example_files/level_1.dat");
      Level l(create_level_file(reader));

   }
   catch(const std::exception &e)
   {
      std::cerr << "Exception: " << e.what() << std::endl;
   }


   return 0;
}