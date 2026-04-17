#pragma once

#include <cstdint>
#include <string>

namespace io {
using byte = uint8_t;
using word = uint16_t;
using dword = uint32_t;
using qword = uint64_t;

class IO {
public:
  /// close the source manually
  virtual void close() = 0;
  /// change position
  virtual void seek(std::size_t offset) = 0;

  virtual ~IO() = default;
};

class Reader : public virtual IO {
public:
  /// whether the source is open
  virtual bool eof() = 0;
  /// read 8 bits
  virtual byte read_byte() = 0;
  /// read 16 bits
  virtual word read_word() = 0;
  /// read 32 bits
  virtual dword read_dword() = 0;
  /// read 64 bits
  virtual qword read_qword() = 0;
  /// read into a string
  virtual std::string read_string(std::size_t length) = 0;

  virtual ~Reader() = default;
};

class Writer : public virtual IO {
public:
  /// write 8 bits
  virtual void write_byte(byte byte) = 0;
  /// write 16 bits
  virtual void write_word(word word) = 0;
  /// write 32 bits
  virtual void write_dword(dword dword) = 0;
  /// write 64 bits
  virtual void write_qword(qword qword) = 0;
  /// write string
  virtual void write_string(const std::string &s) = 0;

  virtual ~Writer() = default;
};

class ReaderWriter : public virtual Reader, public virtual Writer {
  virtual ~ReaderWriter() = default;
};

} // namespace io
