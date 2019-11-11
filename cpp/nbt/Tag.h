#pragma once

#include <optional>
#include <string>
#include <vector>
#include <variant>

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
private:
   void read(std::istream &in);

   tag_e type_;
   std::optional<std::string> name_;
   // { values;
   std::variant<int32_t, int64_t, std::string, double, int8_t, std::vector<int8_t>, std::vector<Tag>> data_;

   // }
};
