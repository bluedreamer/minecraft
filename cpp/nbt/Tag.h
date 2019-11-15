#pragma once

#include <optional>
#include <string>
#include <vector>
#include <variant>
#include <list>

// https://minecraft.gamepedia.com/NBT_format

class Tag
{
public:
   enum class tag_e
   {
      END = 0,
      BYTE = 1,
      SHORT = 2,
      INT = 3,
      LONG = 4,
      FLOAT = 5,
      DOUBLE = 6,
      BYTE_ARRAY = 7,
      STRING = 8,
      LIST = 9,
      COMPOUND = 10,
      INT_ARRAY = 11,
      LONG_ARRAY = 12,
   };
   friend std::istream &operator>>(std::istream &in, Tag &rhs);
   friend std::ostream &operator<<(std::ostream &os, const Tag &rhs);

   tag_e GetType() const;
   void SetType(tag_e type);
   static size_t GetPayloadSizeInBytes(tag_e type);
   const Tag &FindChildTag(const std::string &name) const;
   bool IsNamed(const std::string &name) const;

   bool AsBool() const;
   int32_t AsInt32() const;
   int64_t AsInt64() const;
   double AsDouble() const;
private:
   void read(std::istream &in);
   void print(std::ostream &os) const;
   void get_tag_type(std::istream &in);
   void get_tag_name(std::istream &in);
   void get_tag_body(std::istream &in);
   void get_list_tag_data(std::istream &in);

   tag_e type_{tag_e::END};
   std::optional<std::string> name_{};
   using list_variant=std::variant<
      std::monostate,
      std::list<int8_t>,
      std::list<int16_t>,
      std::list<int32_t>,
      std::list<int64_t>,
      std::list<float>,
      std::list<double>,
      std::list<std::string>,
      std::list<Tag>
   >;

   std::variant<
      std::monostate, // END
      int8_t,  // 1
      int16_t, // 2
      int32_t, // 3
      int64_t, // 4
      float,   // 5
      double,  // 6
      std::vector<int8_t>, // 7
      std::string, // 8
      list_variant, // 9
      std::vector<Tag>, // 10
      std::vector<int32_t>, // 11
      std::vector<int64_t> // 12
   > data_{};
};

const char *enum_to_string(Tag::tag_e e);
