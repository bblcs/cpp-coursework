
#pragma once

#include "io.hpp"
#include <cstddef>
#include <memory>

namespace io {
class BufferedIO : virtual public IO {
protected:
  std::unique_ptr<byte[]> buf_;
  std::size_t bufsize_;
  std::size_t bufpos_;
  std::size_t bufend_;
  bool dirty_ = false;

  BufferedIO(std::size_t bufsize = 4096)
      : buf_(std::make_unique<byte[]>(bufsize)), bufsize_(bufsize) {}
  /// read from source into buf. returns numbers of bytes read.
  virtual std::size_t fill_buf() = 0;
  /// write from buf to source. returns true if did something.
  virtual bool flush_buf() = 0;
  /// source seek
  virtual void src_seek(std::size_t offset) = 0;
  /// source tell
  virtual std::size_t src_tell() const = 0;

public:
  void flush() {
    if (dirty_) {
      flush_buf();
      dirty_ = false;
    }
    bufpos_ = 0;
    bufend_ = 0;
  }

  void close() override { flush(); }
  void seek(std::size_t offset) override {
    flush();
    src_seek(offset);
    bufpos_ = bufend_ = 0;
  }
};
} // namespace io
