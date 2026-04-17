#include "buff_string_io.hpp"
#include <gtest/gtest.h>

TEST(BufferedStringIOTests, Read) {
  std::string s("abcd");
  io::BufferedStringReaderWriter rw(s);

  ASSERT_EQ('a', rw.read_byte());
  ASSERT_EQ('b', rw.read_byte());
  ASSERT_EQ('c', rw.read_byte());
  ASSERT_EQ('d', rw.read_byte());
  ASSERT_TRUE(rw.eof());

  rw.seek(0);
  ASSERT_EQ("abcd", rw.read_string(4));
  ASSERT_TRUE(rw.eof());

  rw.close();
}

TEST(BufferedStringIOTests, Write) {
  std::string s("abcd");
  io::BufferedStringReaderWriter rw(s);

  rw.write_byte('1');
  rw.write_byte('2');
  rw.write_byte('3');
  rw.write_byte('4');

  rw.seek(0);
  ASSERT_EQ('1', rw.read_byte());
  ASSERT_EQ('2', rw.read_byte());
  ASSERT_EQ('3', rw.read_byte());
  ASSERT_EQ('4', rw.read_byte());
  ASSERT_TRUE(rw.eof());

  rw.seek(0);
  rw.write_string("alaboba");
  rw.seek(0);
  ASSERT_EQ("alaboba", rw.read_string(7));
  ASSERT_TRUE(rw.eof());

  rw.close();
}
