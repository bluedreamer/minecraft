#include <system_error>
#include <iostream>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include "Reader.h"

Reader::Reader(const std::string &filename)
   :stream_(filename.c_str(), std::ios_base::in | std::ios_base::binary)
{
   stream_.exceptions(stream_.failbit);

   boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
   in.push(boost::iostreams::gzip_decompressor());
   in.push(stream_);

   std::istream uncompressed_stream(&in);
   uncompressed_stream >> root_tag_;
}

const Tag &Reader::GetRoot() const
{
   return root_tag_;
}

