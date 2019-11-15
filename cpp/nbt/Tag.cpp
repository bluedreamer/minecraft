#include <stdexcept>
#include <istream>
#include <cstdint>

#include <sstream>

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
   rhs.print(os);
   return os;
}

std::string indent = "";

const char *enum_to_string(Tag::tag_e e)
{
#define CASE(x) case x: return #x;
   switch(e)
   {
      CASE(Tag::tag_e::END)
      CASE(Tag::tag_e::BYTE)
      CASE(Tag::tag_e::SHORT)
      CASE(Tag::tag_e::INT)
      CASE(Tag::tag_e::LONG)
      CASE(Tag::tag_e::FLOAT)
      CASE(Tag::tag_e::DOUBLE)
      CASE(Tag::tag_e::BYTE_ARRAY)
      CASE(Tag::tag_e::STRING)
      CASE(Tag::tag_e::LIST)
      CASE(Tag::tag_e::COMPOUND)
      CASE(Tag::tag_e::INT_ARRAY)
      CASE(Tag::tag_e::LONG_ARRAY)
   }
#undef CASE
   return "{ERR: Out of range enum}";
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
   get_tag_type(in);
   get_tag_name(in);
   get_tag_body(in);
}

void Tag::get_tag_type(std::istream &in)
{
   uint8_t byte;
   read_data(in, byte);
   type_ = static_cast<tag_e >(byte);
   // TODO add throw here
}

void Tag::print(std::ostream &os) const
{
   //os << "type: " << (int)rhs.type_ << ' ';
   os << "\ntype: " << enum_to_string(type_) << ' ';
   os << "name: " << name_.value_or("<no value>") << ' ';
   //   std::variant<int32_t, int64_t, std::string, double, int8_t, std::vector<int8_t>, std::vector<Tag>> data_;
   os << "data: ";
   if(static_cast<size_t>(type_) != data_.index())
   {
      std::ostringstream err;
      err << "Bad things happened @#$%#$:";
      err << static_cast<size_t>(type_) << "!=" << data_.index();
      throw std::logic_error(err.str());
   }
   // Now index should match type if it all worked.

   switch(type_)
   {
      case Tag::tag_e::END:
         throw std::logic_error("Variant is not set");
         break;
      case Tag::tag_e::BYTE:
         os << (int)std::get<1>(data_);
         break;
      case Tag::tag_e::SHORT:
         os << std::get<2>(data_);
         break;
      case Tag::tag_e::INT:
         os << std::get<3>(data_);
         break;
      case Tag::tag_e::LONG:
         os << std::get<4>(data_);
         break;
      case Tag::tag_e::FLOAT:
         os << std::get<5>(data_);
         break;
      case Tag::tag_e::DOUBLE:
         os << std::get<6>(data_);
         break;
      case Tag::tag_e::BYTE_ARRAY:
      {
         const auto &d = std::get<7>(data_);
         os << "size: " << d.size() << ' ';
         for(auto &dv : d)
         {
            os << "Ox" << std::hex << (int)dv << std::dec << ",";
         }
      }
      break;

      case Tag::tag_e::STRING:
         os << std::get<8>(data_);
         break;
      case Tag::tag_e::LIST:
         // TODO add list printing
         break;

      case Tag::tag_e::COMPOUND:
      {
         const auto &d = std::get<10>(data_);
         os << "size: " << d.size() << ' ';
         for(auto &dv : d)
         {
            os << dv << ",";
         }
      }
         break;
      case Tag::tag_e::INT_ARRAY:
         // TODO add int array printing
         break;
      case Tag::tag_e::LONG_ARRAY:
         // TODO add long array printing
         break;

   }
   os << ' ' << std::endl;
   //os << "data: " << std::get<rhs.data_.index()>(rhs.data_) << ' ';
}

void Tag::get_tag_name(std::istream &in)
{
   // end tags have no length
   if(type_ != Tag::tag_e::END)
   {
      uint16_t len{0};
      read_data(in, len);
      len = static_cast<decltype(len)>(__bswap_16(len));

      if(len)
      {
         std::string name;
         name.resize(len, '\0');
         in.read(&name[0], len);
         name_ = std::move(name);
      }
   }
}

void Tag::get_tag_body(std::istream &in)
{
   std::cout << indent << "Start " << enum_to_string(type_) << ": " << name_.value_or("<no name>") << std::endl;
   // printing indent stuff
   if(type_ == Tag::tag_e::COMPOUND)
   {
      indent+='\t';
   }
   switch(type_)
   {
      case Tag::tag_e::BYTE:
      {
         int8_t data;
         read_data(in, data);
         std::cout << indent << '\t' << (int)data << std::endl;
         data_ = data;
      }
      break;

      case Tag::tag_e::SHORT:
      {
         int16_t data;
         read_data(in, data);
         std::cout << indent << '\t' << data << std::endl;
         data_ = static_cast<decltype(data)>(__bswap_16(data));
      }
      break;

      case Tag::tag_e::INT:
      {
         int32_t data;
         read_data(in, data);
         std::cout << indent << '\t' << data << std::endl;
         data_ = static_cast<decltype(data)>(__bswap_32(data));
      }
      break;

      case Tag::tag_e::LONG:
      {
         int64_t data;
         read_data(in, data);
         std::cout << indent << '\t' << data << std::endl;
         data_ = static_cast<decltype(data)>(__bswap_64(data));
      }
      break;

      case Tag::tag_e::FLOAT:
      {
         float data;
         read_data(in, data);
         std::cout << indent << '\t' << data << std::endl;
         data_ = data;
      }
      break;

      case Tag::tag_e::DOUBLE:
      {
         double data;
         read_data(in, data);
         std::cout << indent << '\t' << data << std::endl;
         data_ = data;
      }
      break;

      case Tag::tag_e::BYTE_ARRAY:
      {
//            std::cout << "Create Byte Array\n";
         int32_t array_len;
         read_data(in, array_len);
         array_len = static_cast<decltype(array_len)>(__bswap_32(array_len));
         std::vector<int8_t> t;
         t.resize(array_len, '\0');
         in.read((char *)&t[0], array_len);
         // TODO Add byte array dump
         data_ = t;
      }
      break;

      case Tag::tag_e::STRING:
      {
         uint16_t len;
         read_data(in, len);
         len = static_cast<decltype(len)>(__bswap_16(len));
         std::string t;
         t.resize(len, '\0');
         in.read(&t[0], len);
         std::cout << indent << '\t' << t << std::endl;
         data_ = t;

      }
      break;

      case Tag::tag_e::LIST:
      {
         get_list_tag_data(in);
      }
      break;

      case Tag::tag_e::COMPOUND:
      {
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
         data_ = std::move(data);
      }
      break;

      case Tag::tag_e::END:
      {
         // Do nothing as this is called recursively from COMPOUND
      }
      break;

      default:
         std::cout << "Bad: " << (int)type_ << '\n';
         throw std::logic_error("CODE Not written yet");
      break;
   }
   std::cout << indent << "End " << enum_to_string(type_) << ": " << name_.value_or("<no name>") << std::endl;
   // indent only handling
   if(type_ == Tag::tag_e::END)
   {
      indent.erase(indent.length()-1, 1);
   }
}

void Tag::get_list_tag_data(std::istream &in)
{
   int8_t list_data_type;
   read_data(in, list_data_type);
   // set 9
   data_ = list_variant{};
   auto &list_data = std::get<9>(data_);
   int32_t list_items;
   read_data(in, list_items);
   list_items = (int32_t)__bswap_32(list_items);
   indent+='\t'; // increment indent for list body

   std::cout << indent << "List of " << list_items << ' ' << enum_to_string(static_cast<Tag::tag_e>(list_data_type)) << '\n';
   switch(list_data_type)
   {
      case 0:
      {
         list_data = std::monostate{};
      }
         break;
      case 1:
      {
         std::list<int8_t> data;
         while(in && list_items > 0)
         {
            std::cout << indent << "\tReading " << list_items << ':';
            decltype(data)::value_type d;
            read_data(in, d);
            --list_items;
            data.push_back(d);
         }
         list_data = data;
      }
         break;
      case 2:
      {
         std::list<int16_t> data;
         while(in && list_items > 0)
         {
            std::cout << indent << "\tReading " << list_items << ':';
            decltype(data)::value_type d;
            read_data(in, d);
            --list_items;
            data.push_back(d);
         }
         list_data = data;
      }
         break;
      case 3:
      {
         std::list<int32_t> data;
         while(in && list_items > 0)
         {
            decltype(data)::value_type d;
            read_data(in, d);
            data.push_back(d);
            std::cout << indent << "\tReading " << list_items << ':' << d << '\n';
            --list_items;
         }
         list_data = data;
      }
         break;
      case 4:
      {
         std::list<int64_t> data;
         while(in && list_items > 0)
         {
            decltype(data)::value_type d;
            read_data(in, d);
            data.push_back(d);
            std::cout << indent << "\tReading " << list_items << ':' << d << '\n';
            --list_items;
         }
         list_data = data;
      }
         break;
      case 5:
      {
         std::list<float> data;
         while(in && list_items > 0)
         {
            decltype(data)::value_type d;
            read_data(in, d);
            data.push_back(d);
            std::cout << indent << "\tReading " << list_items << ':' << d << '\n';
            --list_items;
         }
         list_data = data;
      }
         break;
      case 6:
      {
         std::list<double> data;
         while(in && list_items > 0)
         {
            decltype(data)::value_type d;
            read_data(in, d);
            data.push_back(d);
            std::cout << indent << "\tReading " << list_items << ':' << d << '\n';
            --list_items;
         }
         list_data = data;
      }
         break;
      case 8:
      {
         std::list<std::string> data;
         while(in && list_items > 0)
         {
            uint16_t len;
            read_data(in, len);
            len = static_cast<decltype(len)>(__bswap_16(len));
            decltype(data)::value_type d;
            d.resize(len, '\0');
            in.read(&d[0], len);
            data.push_back(d);
            std::cout << indent << "\tReading " << list_items << ':' << d << '\n';
            --list_items;
         }
         list_data = data;
      }
         break;
      case 10:
      {
         std::list<Tag> data;
         while(in && list_items > 0)
         {
            decltype(data)::value_type d;
            d.type_ = static_cast<Tag::tag_e>(list_data_type); // Which should be COMPOUND
            d.get_tag_body(in);
            data.push_back(d);
            --list_items;
         }
         list_data = data;
      }
         break;
      default:
         // TODO something unexpected
         std::cout << (int)list_data_type << " No code to read this type of list\n";
         std::terminate();
         break;
   }
   indent.erase(indent.length()-1, 1);
}

const Tag &Tag::FindChildTag(const std::string &name) const
{
   if(data_.index() == static_cast<size_t>(Tag::tag_e::COMPOUND))
   {
      for(const auto &child_tag : std::get<10>(data_))
      {
         if(child_tag.IsNamed(name))
         {
            return child_tag;
         }
      }
   }
   throw std::logic_error("Child tag not found: " + name);
}

bool Tag::IsNamed(const std::string &name) const
{
   if(name_.has_value() && name_ == name)
   {
      return true;
   }
   return false;
}

bool Tag::AsBool() const
{
   return std::get<int8_t>(data_);
}

int64_t Tag::AsInt64() const
{
   return std::get<int64_t>(data_);
}

double Tag::AsDouble() const
{
   return std::get<double>(data_);
}

int32_t Tag::AsInt32() const
{
   return std::get<int32_t>(data_);
}
