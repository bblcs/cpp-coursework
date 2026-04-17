#pragma once

#include "io.hpp"
#include <cstring>
#include <stdexcept>
#include <string>

namespace io {
class StringIO : public virtual IO {
protected:
  std::string source_;
  std::size_t off_;

public:
  StringIO(std::string s) : source_(std::move(s)), off_(0) {}

  void close() override {}
  void seek(std::size_t offset) override {
    if (offset > source_.size()) {
      throw std::out_of_range("seek out of range");
    }
    off_ = offset;
  }
};

class StringReader : public virtual StringIO, public Reader {
  template <typename T> T read_raw() {
    if (off_ + sizeof(T) > source_.size()) {
      throw std::out_of_range("string read out of bounds.");
    }

    T value;
    std::memcpy(&value, source_.data() + off_, sizeof(T));
    off_ += sizeof(T);

    return value;
  }

public:
  StringReader(std::string s) : StringIO(s) {}

  bool eof() override { return off_ == source_.size(); }

  byte read_byte() override { return read_raw<byte>(); }
  word read_word() override { return read_raw<word>(); }
  dword read_dword() override { return read_raw<dword>(); }
  qword read_qword() override { return read_raw<qword>(); }

  std::string read_string(std::size_t length) override {
    std::string res = source_.substr(off_, length);
    off_ += length;
    return res;
  }
};

class StringWriter : public virtual StringIO, public Writer {
  template <typename T> void write_raw(T data) {
    if (off_ + sizeof(T) > source_.size()) {
      throw std::out_of_range("string write out of bounds.");
    }
    std::memcpy(&source_[off_], &data, sizeof(T));
    off_ += sizeof(T);
  }

public:
  StringWriter(std::string s) : StringIO(s) {}

  void write_byte(byte byte) override { write_raw(byte); }
  void write_word(word word) override { write_raw(word); }
  void write_dword(dword dword) override { write_raw(dword); }
  void write_qword(qword qword) override { write_raw(qword); }

  void write_string(const std::string &s) override {
    source_.replace(off_, s.size(), s);
    off_ += s.size();
  }
};

class StringReaderWriter : public StringReader, public StringWriter {
public:
  StringReaderWriter(std::string s)
      : StringIO(std::move(s)), StringReader(s), StringWriter(s) {}
};

} // namespace io
