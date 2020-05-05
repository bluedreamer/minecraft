#include "Level.h"
#include "Reader.h"

Level create_level_file(const Reader &reader)
{
   Level level;

   const auto &root_tag = reader.GetRoot();

   try
   {
      const auto &data_tag = root_tag.FindChildTag("Data");

      level.SetAllowCommands(data_tag.FindChildTag("allowCommands").AsBool());
      level.SetBorderCenterX(data_tag.FindChildTag("BorderCenterX").AsDouble());
      level.SetBorderCenterZ(data_tag.FindChildTag("BorderCenterZ").AsDouble());
      level.SetBorderDamagePerBlock(data_tag.FindChildTag("BorderDamagePerBlock").AsDouble());
      level.SetBorderSize(data_tag.FindChildTag("BorderSize").AsDouble());
      level.SetBorderSafeZone(data_tag.FindChildTag("BorderSafeZone").AsDouble());
      level.SetBorderSizeLerpTarget(data_tag.FindChildTag("BorderSizeLerpTarget").AsDouble());
      level.SetBorderSizeLerpTime(data_tag.FindChildTag("BorderSizeLerpTime").AsInt64());
      level.SetBorderWarningBlocks(data_tag.FindChildTag("BorderWarningBlocks").AsDouble());
      level.SetBorderWarningTime(data_tag.FindChildTag("BorderWarningTime").AsDouble());
      level.SetClearWeatherTime(data_tag.FindChildTag("clearWeatherTime").AsInt32());
      //TODO Need to do customboss events but no data in level.dat example so leaving it


   }
   catch(const std::exception &e)
   {
      throw;
   }

   return level;
}

Level::Level()
{

}
