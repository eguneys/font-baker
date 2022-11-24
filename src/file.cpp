#include "common.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <filesystem>

namespace FB {


  class Win32File: public File {
    private:
      HANDLE m_handle;
      LARGE_INTEGER m_size;

    public:
      Win32File(HANDLE handle);
      size_t length() override;
      size_t read(void* buffer, size_t length) override;
      size_t write(const void* buffer, size_t length) override;
  };
}


using namespace FB;

Win32File::Win32File(HANDLE handle) {

  m_handle = handle;
  m_size.QuadPart = 0;

  LARGE_INTEGER file_size;
  if (GetFileSizeEx(m_handle, &file_size))
    m_size = file_size;
}


size_t Win32File::length() {
  return m_size.QuadPart;
}


size_t  Win32File::read(void* buffer, size_t length) {
  static const DWORD read_step = 65536;

  size_t read = 0;

  while (read < length) {
    DWORD to_read = read_step;
    if (to_read > length - read)
      to_read = (DWORD)(length - read);

    DWORD moved = 0;
    if (ReadFile(m_handle, (char*)buffer + read, to_read, &moved, NULL))
      read += moved;

    if (moved < to_read)
      break;
  }
  return read;
}


size_t Win32File::write(const void* buffer, size_t length) {
  static const DWORD write_step = 65536;

  size_t written = 0;

  while (written < length) {
    DWORD to_write = write_step;
    if (to_write > length - written)
      to_write = (DWORD)(length - written);

    DWORD moved = 0;
    if (WriteFile(m_handle, (char*)buffer + written, to_write, &moved, NULL))
      written += moved;

    if (moved < to_write)
      break;
  }
  return written;
}


using namespace FB;

File* Platform::file_open(const char* path, FileMode mode) {


  int access = 0;
  int creation = 0;

  switch (mode) {
    case FileMode::OpenRead:
      access = GENERIC_READ;
      creation = OPEN_EXISTING;
      break;
    case FileMode::CreateWrite:
      access = FILE_APPEND_DATA;
      creation = CREATE_ALWAYS;
      break;
  }

  auto result = CreateFile(path, access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
  return new Win32File(result);

}
