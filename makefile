SRC=cpp/save_files/Level.cpp cpp/level_dat_reader.cpp cpp/save_files/DataPacks.cpp cpp/save_files/DimensionData.cpp \
   cpp/save_files/GameRules.cpp cpp/save_files/GeneratorOptions.cpp cpp/save_files/Player.cpp \
	cpp/save_files/Version.cpp cpp/save_files/CustomBossEvents.cpp \
	cpp/nbt/Tag.cpp cpp/nbt/Reader.cpp
OBJS=$(SRC:.cpp=.o)
LIBS=-lboost_iostreams -lz
INCLUDES=-Icpp/save_files -Icpp/nbt
CXXFLAGS:=-g -std=c++17 ${INCLUDES}

bin/level_dat_reader: ${OBJS}
	g++ -o $@ ${OBJS} ${LIBS}

clean:
	rm -f ${OBJS} bin/level_dat_reader
