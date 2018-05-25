set -gx CC  /usr/local/bin/gcc-7
set -gx CXX /usr/local/bin/g++-7
set -gx LD  /usr/local/bin/gcc-7
set -gx CPP /usr/local/bin/cpp-7
set -gx TARGET i386-elf
set -gx PREFIX /opt/cross/gcc-i386
./configure --prefix=$PREFIX \
    --target=$TARGET \
    --program-prefix=i386-elf- \
    --disable-werror
./configure --prefix=$PREFIX \
--target=$TARGET \
--disable-nls \
--enable-languages=c,c++ \
--without-headers \
--enable-interwork \
--enable-multilib \
--with-gmp=/usr/local/Cellar/gmp/6.1.2_1 \
--with-mpc=/usr/local/Cellar/libmpc/1.0.3_1 \
--with-mpfr=/usr/local/Cellar/mpfr/3.1.6 \
--disable-werror \
--disable-libssp \
--disable-libmudflap

