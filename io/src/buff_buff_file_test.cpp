#include "buff_buff_file_io.hpp"
#include <gtest/gtest.h>

TEST(BufferedFileIOTests, Read) {
  FILE *file = fopen("readmee", "r");
  io::BufferedFileReaderWriter rw(file);

  ASSERT_EQ('a', rw.read_byte());
  ASSERT_EQ('b', rw.read_byte());
  ASSERT_EQ('c', rw.read_byte());
  ASSERT_EQ('d', rw.read_byte());

  rw.seek(0);
  ASSERT_EQ("abcd", rw.read_string(4));

  rw.close();
}

TEST(BufferedFileIOTests, Write) {
  FILE *file = fopen("readwriteme.txt", "w+");
  io::BufferedFileReaderWriter rw(file);

  rw.write_byte('1');
  rw.write_byte('2');
  rw.write_byte('3');
  rw.write_byte('4');

  rw.seek(0);
  ASSERT_EQ('1', rw.read_byte());
  ASSERT_EQ('2', rw.read_byte());
  ASSERT_EQ('3', rw.read_byte());
  ASSERT_EQ('4', rw.read_byte());

  rw.seek(0);
  rw.write_string("alaboba");
  rw.seek(0);
  ASSERT_EQ("alaboba", rw.read_string(7));

  rw.close();
}
