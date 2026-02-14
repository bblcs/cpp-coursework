project('plane-lib', 'cpp',
  default_options: ['warning_level=3', 'cpp_std=c++23'])

src = files('src/point.hpp', 'src/line.hpp', 'src/vec.hpp')
lib = library('plane-lib', src)

gtest_dep = dependency('gtest_main')

test_src = files('src/test.cpp')
test_exe = executable('tests',
                      test_src,
                      link_with: lib,
                      dependencies: [gtest_dep])

test('testt', test_exe)
