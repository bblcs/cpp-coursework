project('io', 'cpp',
  default_options: ['warning_level=3', 'cpp_std=c++23'])

gtest_dep = dependency('gtest_main')

test_src = files('src/string_test.cpp', 'src/file_test.cpp', 'src/buff_string_test.cpp', 'src/buff_buff_file_test.cpp')
test_exe = executable('io_test',
                      test_src,
                      dependencies: [gtest_dep])

test('io_test', test_exe)
