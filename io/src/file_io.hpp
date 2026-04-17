#pragma once

#include "io.hpp"
#include <cstdio>

namespace io {
class FileIO : public virtual IO {
protected:
  FILE *source_;

public:
  FileIO(FILE *f) : source_(f) {}

  void close() override { fclose(source_); }
  void seek(std::size_t offset) override { fseek(source_, offset, SEEK_SET); }
};

class FileReader : public virtual FileIO, public Reader {
  template <typename T> T read_raw() {
    T value;
    fread(&value, 1, sizeof(T), source_);
    return value;
  }

public:
  FileReader(FILE *f) : FileIO(f) {}

  bool eof() override { return feof(source_); }

  byte read_byte() override { return read_raw<byte>(); }
  word read_word() override { return read_raw<word>(); }
  dword read_dword() override { return read_raw<dword>(); }
  qword read_qword() override { return read_raw<qword>(); }

  std::string read_string(std::size_t length) override {
    std::string s;
    s.resize(length);

    fread(s.data(), sizeof(char), length, source_);
    return s;
  }
};

class FileWriter : public virtual FileIO, public Writer {

  template <typename T> void write_raw(T data) {
    fwrite(&data, 1, sizeof(T), source_);
  }

public:
  FileWriter(FILE *f) : FileIO(f) {}

  void write_byte(byte byte) override { write_raw(byte); }
  void write_word(word word) override { write_raw(word); }
  void write_dword(dword dword) override { write_raw(dword); }
  void write_qword(qword qword) override { write_raw(qword); }

  void write_string(const std::string &s) override {
    fwrite(s.data(), sizeof(char), s.length(), source_);
  }
};

class FileReaderWriter : public FileReader, public FileWriter {
public:
  FileReaderWriter(FILE *f) : FileIO(f), FileReader(f), FileWriter(f) {}
};

} // namespace io
