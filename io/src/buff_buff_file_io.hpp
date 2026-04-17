#pragma once

#include "buff_io.hpp"
#include <cstdio>
#include <cstring>

namespace io {

class BufferedFileIO : public virtual BufferedIO {
protected:
  FILE *source_;

public:
  BufferedFileIO(FILE *f, std::size_t bufsize = 4096)
      : BufferedIO(bufsize), source_(f) {}

  void src_seek(std::size_t offset) override {
    fseek(source_, static_cast<long>(offset), SEEK_SET);
  }

  std::size_t src_tell() const override {
    return static_cast<std::size_t>(ftell(source_));
  }

protected:
  std::size_t fill_buf() override {
    std::size_t bytes = fread(buf_.get(), 1, bufsize_, source_);
    bufpos_ = 0;
    bufend_ = bytes;
    return bytes;
  }

  bool flush_buf() override {
    if (!dirty_)
      return true;
    std::size_t written = fwrite(buf_.get(), 1, bufpos_, source_);
    return written == bufpos_;
  }
};

class BufferedFileReader : public virtual BufferedFileIO, public Reader {
  template <typename T> T read_raw() {
    T value;
    byte *dest = reinterpret_cast<byte *>(&value);
    std::size_t rem = sizeof(T);

    while (rem > 0) {
      if (bufpos_ == bufend_) {
        if (fill_buf() == 0)
          throw std::runtime_error("file read out of bounds.");
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
  BufferedFileReader(FILE *f, std::size_t bufsize)
      : BufferedFileIO(f, bufsize) {}

  bool eof() override { return bufpos_ == bufend_ && feof(source_); }

  byte read_byte() override { return read_raw<byte>(); }
  word read_word() override { return read_raw<word>(); }
  dword read_dword() override { return read_raw<dword>(); }
  qword read_qword() override { return read_raw<qword>(); }

  std::string read_string(std::size_t length) override {
    std::string result;
    result.reserve(length);
    while (length > 0) {
      if (bufpos_ == bufend_) {
        if (fill_buf() == 0)
          break;
      }
      std::size_t chunk = std::min(length, bufend_ - bufpos_);
      result.append(reinterpret_cast<char *>(buf_.get() + bufpos_), chunk);
      bufpos_ += chunk;
      length -= chunk;
    }
    return result;
  }
};

class BufferedFileWriter : public virtual BufferedFileIO, public Writer {
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
  BufferedFileWriter(FILE *f, std::size_t bufsize = 4096)
      : BufferedFileIO(f, bufsize) {}

  void write_byte(byte b) override { write_raw(b); }
  void write_word(word w) override { write_raw(w); }
  void write_dword(dword d) override { write_raw(d); }
  void write_qword(qword q) override { write_raw(q); }

  void write_string(const std::string &s) override {
    const char *data = s.data();
    std::size_t rem = s.size();

    while (rem > 0) {
      if (bufpos_ == bufsize_) {
        flush_buf();
        bufpos_ = 0;
      }
      std::size_t space = bufsize_ - bufpos_;
      std::size_t chunk = std::min(rem, space);
      std::memcpy(buf_.get() + bufpos_, data, chunk);
      data += chunk;
      bufpos_ += chunk;
      rem -= chunk;
      dirty_ = true;
    }
  }
};

class BufferedFileReaderWriter : public BufferedFileReader,
                                 public BufferedFileWriter {
public:
  BufferedFileReaderWriter(FILE *f, std::size_t bufsize = 4096)
      : BufferedFileIO(f, bufsize), BufferedFileReader(f, bufsize),
        BufferedFileWriter(f, bufsize) {}
};

} // namespace io
