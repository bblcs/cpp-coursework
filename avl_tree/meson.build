project('avl_tree', 'cpp',
  default_options: ['warning_level=3', 'cpp_std=c++23'])

src = files('src/avl.cpp')
lib = library('avl', src)

gtest_dep = dependency('gtest_main')

test_src = files('src/test.cpp')
test_exe = executable('avl_test',
                      test_src,
                      link_with: lib,
                      dependencies: [gtest_dep])

test('avl_test', test_exe)
