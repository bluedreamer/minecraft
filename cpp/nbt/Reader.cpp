#include <system_error>
#include <iostream>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include "Reader.h"
#include "Tag.h"

Reader::Reader(const std::string &filename)
   :stream_(filename.c_str(), std::ios_base::in | std::ios_base::binary)
{
   stream_.exceptions(stream_.failbit);

   boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
   in.push(boost::iostreams::gzip_decompressor());
   in.push(stream_);

   std::istream uncompressed_stream(&in);
   //boost::iostreams::copy(in, std::cout);

   Tag t;
   uncompressed_stream >> t;

   std::cout << "Dump\n" << t << std::endl;
   // TODO better exception here
//   if(!stream_)
//   {
//      std::string msg("Unable to open file: [");
//      msg+=filename;
//      msg+="]";
//      throw std::system_error(errno, msg);
//   }


}
