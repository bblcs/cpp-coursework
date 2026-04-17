#pragma once

#include "buff_io.hpp"
#include <cstring>
#include <stdexcept>
#include <string>

namespace io {

class BufferedStringIO : public virtual BufferedIO {
protected:
  std::string source_;
  std::size_t source_pos_ = 0;

public:
  BufferedStringIO(std::string s, std::size_t bufsize = 4096)
      : BufferedIO(bufsize), source_(std::move(s)) {}

  void src_seek(std::size_t offset) override {
    if (offset > source_.size())
      throw std::out_of_range("seek out of range.");
    source_pos_ = offset;
  }

  std::size_t src_tell() const override { return source_pos_; }

protected:
  std::size_t fill_buf() override {
    std::size_t available = source_.size() - source_pos_;
    std::size_t to_read = std::min(available, bufsize_);
    std::memcpy(buf_.get(), source_.data() + source_pos_, to_read);
    source_pos_ += to_read;
    bufpos_ = 0;
    bufend_ = to_read;
    return to_read;
  }

  bool flush_buf() override {
    if (!dirty_)
      return true;
    if (source_pos_ + bufpos_ > source_.size())
      source_.resize(source_pos_ + bufpos_);
    std::memcpy(&source_[source_pos_], buf_.get(), bufpos_);
    source_pos_ += bufpos_;
    return true;
  }
};

class BufferedStringReader : public virtual BufferedStringIO, public Reader {
  template <typename T> T read_raw() {
    T value;
    std::size_t rem = sizeof(T);
    byte *dest = reinterpret_cast<byte *>(&value);

    while (rem > 0) {
      if (bufpos_ == bufend_) {
        if (fill_buf() == 0)
          throw std::out_of_range("string read out of bounds");
      }
      std::size_t chunk = std::min(rem, bufend_ - bufpos_);
      std::memcpy(dest, buf_.get() + bufpos_, chunk);
      dest += chunk;
      bufpos_ += chunk;
      rem -= chunk;
    }
    return value;
  }

public:
  BufferedStringReader(std::string s, std::size_t bufsize = 4096)
      : BufferedStringIO(std::move(s), bufsize) {}

  bool eof() override {
    return bufpos_ == bufend_ && source_pos_ == source_.size();
  }

  byte read_byte() override { return read_raw<byte>(); }
  word read_word() override { return read_raw<word>(); }
  dword read_dword() override { return read_raw<dword>(); }
  qword read_qword() override { return read_raw<qword>(); }

  std::string read_string(std::size_t length) override {
    std::string res;
    res.reserve(length);
    while (length > 0) {
      if (bufpos_ == bufend_) {
        if (fill_buf() == 0)
          break;
      }
      std::size_t chunk = std::min(length, bufend_ - bufpos_);
      res.append(reinterpret_cast<char *>(buf_.get() + bufpos_), chunk);
      bufpos_ += chunk;
      length -= chunk;
    }
    return res;
  }
};

class BufferedStringWriter : public virtual BufferedStringIO, public Writer {
  template <typename T> void write_raw(T data) {
    const byte *src = reinterpret_cast<const byte *>(&data);
    std::size_t rem = sizeof(T);

    while (rem > 0) {
      if (bufpos_ == bufsize_) {
        flush_buf();
        bufpos_ = 0;
      }
      std::size_t space = bufsize_ - bufpos_;
      std::size_t chunk = std::min(rem, space);
      std::memcpy(buf_.get() + bufpos_, src, chunk);
      src += chunk;
      bufpos_ += chunk;
      rem -= chunk;
      dirty_ = true;
    }
  }

public:
  BufferedStringWriter(std::string s, std::size_t bufsize = 4096)
      : BufferedStringIO(std::move(s), bufsize) {}

  void write_byte(byte b) override { write_raw(b); }
  void write_word(word w) override { write_raw(w); }
  void write_dword(dword d) override { write_raw(d); }
  void write_qword(qword q) override { write_raw(q); }

  void write_string(const std::string &s) override {
    const char *data = s.data();
    std::size_t remaining = s.size();

    while (remaining > 0) {
      if (bufpos_ == bufsize_) {
        flush_buf();
        bufpos_ = 0;
      }
      std::size_t space = bufsize_ - bufpos_;
      std::size_t chunk = std::min(remaining, space);
      std::memcpy(buf_.get() + bufpos_, data, chunk);
      data += chunk;
      bufpos_ += chunk;
      remaining -= chunk;
      dirty_ = true;
    }
  }
};

class BufferedStringReaderWriter : public BufferedStringReader,
                                   public BufferedStringWriter {
public:
  BufferedStringReaderWriter(std::string s, std::size_t bufsize = 4096)
      : BufferedStringIO(std::move(s), bufsize),
        BufferedStringReader("", bufsize), BufferedStringWriter("", bufsize) {}
};

} // namespace io
