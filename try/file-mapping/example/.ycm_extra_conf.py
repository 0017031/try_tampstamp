import os

# These are the compilation flags that will be used in case there's no
# compilation database set (by default, one is not set).
# CHANGE THIS LIST OF FLAGS. YES, THIS IS THE DROID YOU HAVE BEEN LOOKING FOR.
flags = [
    '-Wall',
    '-Wextra',
    '-Werror',
    # '-Wc++98-compat',
    '-Wno-long-long',
    '-Wno-variadic-macros',
    '-fexceptions',
    '-DNDEBUG',
    # THIS IS IMPORTANT! Without a "-std=<something>" flag, clang won't know which
    # language to use when compiling headers. So it will guess. Badly. So C++
    # headers will be compiled as C headers. You don't want that so ALWAYS specify
    # a "-std=<something>".
    # For a C project, you would set this to something like 'c99' instead of
    # 'c++11'.
    '-std=c++1z',
    # ...and the same thing goes for the magic -x option which specifies the
    # language that the files to be compiled are written in. This is mostly
    # relevant for c++ headers.
    # For a C project, you would set this to 'c' instead of 'c++'.
    '-x',
    'c++',

    # '-isystem',
    # '../llvm/include',

    '-isystem',
    'C:/Users/baic/Downloads/apps/LLVM/LLVM-5.0.0-r298093-win32/lib/clang/5.0.0/include',
    # '../llvm/tools/clang/include',

    '-I',
    '.',

    '-I',
    'C:\\Users\\baic\\Downloads\\work\\github-libs\\gsl\\include',
    # os.environ['GSL_ROOT'],

    '-I',
    #  'C:\\Users\\baic\\Downloads\\apps\\boost\\32bit\\1_63_0\\boost\\interprocess\\',
    os.environ['BOOST_ROOT'],
]
