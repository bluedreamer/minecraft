#include <stdexcept>
#include <istream>
#include <cstdint>

//TODO Remove below
#include <iostream>

#include "Tag.h"

std::istream &operator>>(std::istream &in, Tag &rhs)
{
   rhs.read(in);

   return in;
}

std::ostream &operator<<(std::ostream &os, const Tag &rhs)
{
   os << "type: " << (int)rhs.type_ << ' ';
   os << "name: " << rhs.name_.value_or("<no value>") << ' ';
   //   std::variant<int32_t, int64_t, std::string, double, int8_t, std::vector<int8_t>, std::vector<Tag>> data_;
   os << "data: ";
   switch(rhs.data_.index())
   {
      case 0:
         os << std::get<0>(rhs.data_);
         break;
      case 1:
         os << std::get<1>(rhs.data_);
         break;
      case 2:
         os << std::get<2>(rhs.data_);
         break;
      case 3:
         os << std::get<3>(rhs.data_);
         break;
      case 4:
         os << (int)std::get<4>(rhs.data_);
         break;
      case 5:
      {
         const auto &d = std::get<5>(rhs.data_);
         for(auto &dv : d)
         {
            os << "Ox" << std::hex << (int)dv << std::dec << ",";
         }
      }
      case 6:
      {
         const auto &d = std::get<5>(rhs.data_);
         for(auto &dv : d)
         {
            os << dv << ",";
         }
      }
      break;
   }
   os << ' ' << std::endl;
   //os << "data: " << std::get<rhs.data_.index()>(rhs.data_) << ' ';
   return os;
}

Tag::tag_e Tag::GetType() const
{
   return type_;
}

void Tag::SetType(Tag::tag_e type)
{
   type_=type;
}

size_t Tag::GetPayloadSizeInBytes(Tag::tag_e type)
{
   switch(type)
   {
      case tag_e::BYTE:    return 1;
      case tag_e::SHORT:   return 2;
      case tag_e::INT:     return 4;
      case tag_e::LONG:    return 8;
      case tag_e::FLOAT:   return 4;
      case tag_e::DOUBLE:  return 8;

      default:
         throw std::logic_error("This tag type does not have a fixed size");
      break;
   }
   return 0;
}

template<typename T>
static void read_data(std::istream &in, T &data)
{
   in.read(reinterpret_cast<std::remove_reference<decltype(in)>::type::char_type *>(&data), sizeof(data));
}

void Tag::read(std::istream &in)
{
   uint8_t byte;
   read_data(in, byte);
   SetType(static_cast<Tag::tag_e >(byte));
   if(GetType() != Tag::tag_e::END)
   {
      uint16_t len;
      read_data(in, len);
      len = __bswap_16(len);

      if (len)
      {
         std::string name;
         name.resize(len, '\0');
         in.read(&name[0], len);

         name_ = std::move(name);
      }

      switch (type_)
      {
         case Tag::tag_e::BYTE:
         {
//            std::cout << "Create Byte\n";
            int8_t data;
            read_data(in, data);
            data_=data;
         }
            break;

         case Tag::tag_e::INT:
         {
//            std::cout << "Create Int\n";
            int32_t data;
            read_data(in, data);
            data_=(int32_t)__bswap_32(data);
         }
            break;

         case Tag::tag_e::LONG:
         {
//            std::cout << "Create Long\n";
            int64_t data;
            read_data(in, data);
            data_=(int64_t)__bswap_64(data);
         }
            break;

         case Tag::tag_e::DOUBLE:
         {
//            std::cout << "Create Double\n";
            double data;
            read_data(in, data);
            data_=data;
         }
            break;

         case Tag::tag_e::BYTE_ARRAY:
         {
//            std::cout << "Create Byte Array\n";
            int32_t len;
            read_data(in, len);
            len=(decltype(len))__bswap_32(len);
            std::vector<int8_t> t;
            t.resize(len, '\0');
            in.read((char*)&t[0], len);
            data_=t;
         }
            break;

         case Tag::tag_e::STRING:
         {
//            std::cout << "Create String\n";
            uint16_t len;
            read_data(in, len);
            len=__bswap_16(len);
            std::string t;
            t.resize(len, '\0');
            in.read(&t[0], len);
            data_=t;

         }
            break;

         case Tag::tag_e::LIST:
         {
//            std::cout << "Create List\n";
            int8_t list_data_type;
            read_data(in, list_data_type);

            int32_t list_items;
            read_data(in, list_items);
            list_items=(int32_t)__bswap_32(list_items);

            auto payload_size = Tag::GetPayloadSizeInBytes(static_cast<Tag::tag_e>(list_data_type));

            while(in && list_items > 0)
            {
               Tag t;
               t.SetType(static_cast<Tag::tag_e >(list_data_type));
               std::vector<char> data(payload_size, 0);
               in.read(&data[0], payload_size);
               //TODO save the list items somewhere
            }
         }
            break;

         case Tag::tag_e::COMPOUND:
         {
//            std::cout << "Create Compound\n";
            bool is_end = false;
            std::vector<Tag> data;
            while(!is_end && in)
            {
               Tag t;
               in >> t;
               if(t.GetType() == Tag::tag_e::END)
               {
                  is_end = true;
               }
               data.push_back(t);
            }
            data_=std::move(data);
         }
            break;

         case Tag::tag_e::END:
         {
//            std::cout << "Create End\n";
//            Tag t;
//            in >> t;
         }
            break;

         default:
            std::cout << "Bad: " << (int)type_ << '\n';
            throw std::logic_error("CODE Not written yet");
            break;
      }
   }

}
