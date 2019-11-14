#include <iostream>
#include <vector>

#include "Reader.h"
#include "Level.h"

int main(int argc, char* argv[])
{
   try
   {
      std::vector<std::string> level_files={
         "../cpp/save_files/example_files/level_1.dat",
         "../cpp/save_files/example_files/level_2.dat",
         "../cpp/save_files/example_files/level_3.dat",
         "../cpp/save_files/example_files/level_4.dat",
         "../cpp/save_files/example_files/level_5.dat",
         "../cpp/save_files/example_files/level_6.dat"};
      for(const auto &filename : level_files)
      {
         std::cout << "READING: " << filename << std::endl;
         Reader reader(filename);
         Level l(create_level_file(reader));
      }
   }
   catch(const std::exception &e)
   {
      std::cerr << "Exception: " << e.what() << std::endl;
   }

   return 0;
}