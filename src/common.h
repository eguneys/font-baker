#pragma once

#include <stdint.h>

namespace FB {

  using i8       = int8_t;
	using i16      = int16_t;
	using i32      = int32_t;
	using i64      = int64_t;
	using u8       = uint8_t;
	using u16      = uint16_t;
	using u32      = uint32_t;
	using u64      = uint64_t;
	using f32      = float;
	using f64      = double;


  struct String {
    char* buffer;
    int length;
  };

  using FilePath = char *;


  struct Color {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
  };


  class Image {

  };


  enum class FileMode {
    OpenRead,
    CreateWrite
  };

  class File;
  class Platform {
    public:
    static File * file_open(const char* path, FileMode mode);
  };


  class File {
    public:

    virtual size_t length() = 0;
    virtual size_t read(void* buffer, size_t length) = 0;
    virtual size_t write(const void* buffer, size_t length) = 0;

  };

}
