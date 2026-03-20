project('matrix', 'cpp',
  default_options: ['warning_level=3', 'cpp_std=c++23'])

src = files('src/matrix.hpp')
lib = library('matrix', src)

gtest_dep = dependency('gtest_main')

test_src = files('src/test.cpp')
test_exe = executable('tests',
                      test_src,
                      link_with: lib,
                      dependencies: [gtest_dep])

test('testt', test_exe)
