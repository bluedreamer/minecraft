#pragma once

#include <cstdint>
#include <string>
#include <chrono>

#include "CustomBossEvents.h"
#include "DataPacks.h"
#include "DimensionData.h"
#include "GameRules.h"
#include "GeneratorOptions.h"
#include "Player.h"
#include "Version.h"

// Source for format https://minecraft.gamepedia.com/Level_format#level.dat_format

//class CustomBossEvents;
//class DataPacks;
//class DimensionData;
//class GameRules;
//class GeneratorOptions;
//class Player;
//class Version;

class Level;
class Reader;
Level create_level_file(const Reader &reader);

class Level
{
public:
   Level();
   enum class difficulty_e
   {
      PEACEFUL = 0,
      EASY = 1,
      NORMAL = 2,
      HARD = 3,
   };
   enum class game_type_e {
      SURVIVAL = 0,
      CREATIVE = 1,
      ADVENTURE = 2,
      SPECTATOR = 3,
   };
   void SetAllowCommands(bool value) { allow_commands_ = value; }
   void SetBorderCenterX(double value) { border_center_x_=value; }
   void SetBorderCenterZ(double value) { border_center_z_=value; }
   void SetBorderDamagePerBlock(double value) { border_damage_per_block_=value; }
   void SetBorderSize(double value) { border_size_=value; }
   void SetBorderSafeZone(double value) { border_safe_zone_=value; }
   void SetBorderSizeLerpTarget(double value) { border_size_lerp_target_=value; }
   void SetBorderSizeLerpTime(int64_t value) { border_size_lerp_time_=value; }
   void SetBorderWarningBlocks(double value) { border_warning_blocks_=value;}
   void SetBorderWarningTime(double value) { border_warning_time_=value;}
   void SetClearWeatherTime(int32_t value) { clear_weather_time_=value; }

private:
   bool allow_commands_{false};            ///<  1 or 0 (true/false) - true if cheats are enabled.
   double border_center_x_{0};     ///< Center of the world border on the X coordinate. Defaults to 0.
   double border_center_z_{0};     ///< Center of the world border on the Z coordinate. Defaults to 0.
   double border_damage_per_block_{0.2}; ///< Defaults to 0.2.
   double border_size_{60000000};            ///< Width and length of the border of the border. Defaults to 60000000.
   double border_safe_zone_{5};        ///< Defaults to 5.
   double border_size_lerp_target_{60000000};   ///< Defaults to 60000000.
   int64_t border_size_lerp_time_{0};     ///< Defaults to 0.
   double border_warning_blocks_{5};     ///< Defaults to 5.
   double border_warning_time_{15};        ///< Defaults to 15.
   int32_t clear_weather_time_{0};        ///< The number of ticks until "clear weather" has ended.
   CustomBossEvents custom_boss_events_{}; ///< A collection of bossbars.
   DataPacks data_packs_{}; ///< Options for datapacks.
   int32_t data_version_{-1207500800}; ///<  An integer displaying the data version. https://minecraft.gamepedia.com/Data_version
   difficulty_e difficulty_{difficulty_e::NORMAL}; ///< The current difficulty setting. 0 is Peaceful, 1 is Easy, 2 is Normal, and 3 is Hard. Defaults to 2.
   bool difficulty_locked_{false};      ///< 1 or 0 (true/false) - True if the difficulty has been locked. Defaults to 0.
   uint64_t day_time_{0}; ///< The time of day. 0 is sunrise, 6000 is mid day, 12000 is sunset, 18000 is mid night, 24000 is the next day's 0. This value keeps counting past 24000 and does not reset to 0.
   DimensionData dimension_data_{};
   game_type_e game_type_{game_type_e::SURVIVAL}; ///< The default game mode for the singleplayer player when they initially spawn. 0 is Survival, 1 is Creative, 2 is Adventure, 3 is Spectator. Note: Singleplayer worlds do not use this field to save which game mode the player is currently in.
   GameRules game_rules_{}; ///< The gamerules used in the world. https://minecraft.gamepedia.com/Commands#gamerule
   std::string generator_name_{}; ///< The name of the generator; default, flat, largeBiomes, amplified, buffet, debug_all_block_states or default_1_1. Not case sensitive, but always written in the case here. The last one can only exist if the file was edited. It is a variation of the default generation. It can also be customized if it is a customized world from before 1.13. In this case the world will become default if opened using 1.13 or newer.
   GeneratorOptions generator_options_{}; ///< Used in buffet, superflat and old customized worlds.
   int32_t generator_version_{16777216}; ///< The version of the level generator. The effects of changing this are unknown, but values other than 0 have been observed.
   bool hardcore_{false}; ///<  1 or 0 (true/false) - true if the player will respawn in Spectator on death in singleplayer. Affects all three game modes.
   bool initialized_{false}; ///< 1 or 0 (true/false) - Normally true after a world has been initialized properly after creation. If the initial simulation was canceled somehow, this can be false and the world will be re-initialized on next load.
   std::chrono::microseconds last_played_{0}; ///<  The Unix time in milliseconds when the level was last loaded.
   std::string level_name_{}; ///< The name of the level.
   bool map_features_{true}; ///< 1 or 0 (true/false) - true if the map generator should place structures such as villages, strongholds, and mineshafts. Defaults to 1. Always 1 if the world type is Customized.
   Player player_{}; ///< The state of the Singleplayer player. This overrides the <player>.dat file with the same name as the Singleplayer player. This is only saved by Servers if it already exists, otherwise it is not saved for server worlds. https://minecraft.gamepedia.com/Player.dat_format#NBT_Structure
   int64_t random_seed_{0}; ///< The random level seed used to generate consistent terrain.
   bool raining_{false}; ///< 1 or 0 (true/false) - true if the level is currently experiencing rain, snow, and cloud cover.
   int32_t rain_time{0}; ///< The number of ticks before "raining" is toggled and this value gets set to another random value.
   uint64_t size_on_disk_{0}; ///< The estimated size in bytes of the level. Currently not modified or used by Minecraft, but was previously.
   int32_t spawn_x_{0}; ///< The X coordinate of the world spawn.
   int32_t spawn_y_{0}; ///< The Y coordinate of the world spawn.
   int32_t spawn_z_{0}; ///< The Z coordinate of the world spawn.
   bool thundering_{false}; ///< 1 or 0 (true/false) - true if the rain/snow/cloud cover is a lightning storm and dark enough for mobs to spawn under the sky.
   int32_t thunder_time_{0}; ///< The number of ticks before "thundering" is toggled and this value gets set to another random value.
   int64_t time_{0}; ///< The number of ticks since the start of the level.
   int32_t version_{0}; ///< The NBT version of the level, with 1.14.4 being 19133. https://minecraft.gamepedia.com/NBT_format
   Version world_version_{}; ///< Information about the Minecraft version the world was saved in.
   //TODO Add a UUID class if needed
   std::string wandering_trader_id_{}; ///< The UUID of the current wandering trader in the world.
   uint32_t wandering_trader_spawn_chance_{0}; ///< The current chance of the wandering trader spawning next attempt; this value is the percentage and will be divided by 10 when loaded by the game, for example a value of 50 means 5.0% chance.
   uint32_t wandering_trader_spawn_delay_{0}; ///< The amount of ticks until another wandering trader is attempted to spawn
};
