petri_net.o: petri_net.cpp petri_net.hpp rapidjson/document.h \
  rapidjson/reader.h rapidjson/allocators.h rapidjson/rapidjson.h \
  rapidjson/stream.h rapidjson/encodings.h rapidjson/encodedstream.h \
  rapidjson/memorystream.h rapidjson/internal/clzll.h \
  rapidjson/internal/../rapidjson.h rapidjson/internal/meta.h \
  rapidjson/internal/stack.h rapidjson/internal/../allocators.h \
  rapidjson/internal/swap.h rapidjson/internal/strtod.h \
  rapidjson/internal/ieee754.h rapidjson/internal/biginteger.h \
  rapidjson/internal/diyfp.h rapidjson/internal/pow10.h \
  rapidjson/error/error.h rapidjson/error/../rapidjson.h \
  rapidjson/internal/strfunc.h rapidjson/internal/../stream.h \
  rapidjson/prettywriter.h rapidjson/writer.h rapidjson/internal/dtoa.h \
  rapidjson/internal/itoa.h rapidjson/stringbuffer.h
distributed_system.o: distributed_system.cpp rapidjson/document.h \
  rapidjson/reader.h rapidjson/allocators.h rapidjson/rapidjson.h \
  rapidjson/stream.h rapidjson/encodings.h rapidjson/encodedstream.h \
  rapidjson/memorystream.h rapidjson/internal/clzll.h \
  rapidjson/internal/../rapidjson.h rapidjson/internal/meta.h \
  rapidjson/internal/stack.h rapidjson/internal/../allocators.h \
  rapidjson/internal/swap.h rapidjson/internal/strtod.h \
  rapidjson/internal/ieee754.h rapidjson/internal/biginteger.h \
  rapidjson/internal/diyfp.h rapidjson/internal/pow10.h \
  rapidjson/error/error.h rapidjson/error/../rapidjson.h \
  rapidjson/internal/strfunc.h rapidjson/internal/../stream.h \
  rapidjson/prettywriter.h rapidjson/writer.h rapidjson/internal/dtoa.h \
  rapidjson/internal/itoa.h rapidjson/stringbuffer.h \
  distributed_system.hpp
main.o: main.cpp petri_net.hpp distributed_system.hpp
