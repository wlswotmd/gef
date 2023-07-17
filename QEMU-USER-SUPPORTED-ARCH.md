# Qemu-user supported architectures
I also list the tools I used in my Ubuntu 22.04 environment.

* x86
    * toolchain: `gcc` via apt with `-m32` option.
    * qemu: `qemu-i386` via apt.
    * gdb: `gdb-multiarch` via apt.
* x64
    * toolchain: `gcc` via apt.
    * qemu: `qemu-x86_64` via apt.
    * gdb: `gdb-multiarch` via apt.
* ARM
    * toolchain: `gcc-arm-linux-gnueabihf` via apt.
    * qemu: `qemu-arm` via apt.
    * gdb: `gdb-multiarch` via apt.
* Aarch64
    * toolchain: `gcc-aarch64-linux-gnu` via apt.
    * qemu: `qemu-aarch64` via apt.
    * gdb: `gdb-multiarch` via apt.
* PPC32
    * toolchain: `gcc-powerpc-linux-gnu` via apt.
    * qemu: `qemu-ppc` via apt.
    * gdb: `gdb-multiarch` via apt.
* PPC64
    * toolchain: `gcc-powerpc64-linux-gnu` and `gcc-powerpc64le-linux-gnu` via apt.
    * qemu: `qemu-ppc64` and `qemu-ppc64le` via apt.
    * gdb: `gdb-multiarch` via apt.
* MIPS32
    * toolchain: `gcc-mips-linux-gnu` and `gcc-mipsel-linux-gnu` via apt.
    * qemu: `qemu-mips` and `qemu-mipsel` via apt.
    * gdb: `gdb-multiarch` via apt.
* MIPS64
    * toolchain: `gcc-mips64-linux-gnuabi64` and `gcc-mips64el-linux-gnuabi64` via apt.
    * qemu: `qemu-mips64` and `qemu-mips64el` via apt.
    * gdb: `gdb-multiarch` via apt.
* SPARC32
    * toolchain: `sparcv8--uclibc--stable-2022.08-1` from https://toolchains.bootlin.com/
    * qemu: `qemu-sparc` via apt.
    * gdb: `gdb-multiarch` via apt.
* SPARC64
    * toolchain: `sparc64--glibc--stable-2022.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain obtained with apt seems to be broken since the built ELF always SIGSEGV.
    * qemu: `qemu-sparc64` via apt.
    * gdb: `gdb-multiarch` via apt.
* RISCV32
    * toolchain: `riscv32-ilp32d--glibc--bleeding-edge-2022.08-1` from https://toolchains.bootlin.com/
    * qemu: `qemu-riscv32` via apt.
    * gdb: `gdb-multiarch` via apt.
* RISCV64
    * toolchain: `gcc-riscv64-linux-gnu` via apt.
    * qemu: `qemu-riscv64` via apt.
    * gdb: `gdb-multiarch` via apt.
* s390x
    * toolchain: `gcc-s390x-linux-gnu` via apt.
    * qemu: `qemu-s390x` via apt.
    * gdb: `gdb-multiarch` via apt.
* sh4
    * toolchain: `sh-sh4--uclibc--stable-2022.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain obtained with apt seems to be broken since static build is failed.
    * qemu: `qemu-sh4` via apt.
    * gdb: `gdb-multiarch` via apt.
* m68k
    * toolchain: `gcc-m68k-linux-gnu` via apt.
    * qemu: `qemu-m68k` via apt.
    * gdb: `gdb-multiarch` via apt.
* alpha
    * toolchain: `gcc-alpha-linux-gnu` via apt.
    * qemu: `qemu-alpha` via apt.
    * gdb: `gdb-multiarch` via apt.
* PA-RISC (HPPA)
    * toolchain: `gcc-hppa-linux-gnu` via apt.
    * qemu: `qemu-hppa` via apt.
    * gdb: `gdb-multiarch` via apt.
* OpenRISC 1000 (OR1K)
    * toolchain: `openrisc--glibc--stable-2022.08-1` from https://toolchains.bootlin.com/
    * qemu: `qemu-or1k` via apt.
    * gdb: build from latest source with `./configure --enable-targets=all --with-python=/usr/bin/python3`.
* NiosII
    * toolchain: `nios2--glibc--stable-2022.08-1` from https://toolchains.bootlin.com/
    * qemu: `qemu-nios2` via apt.
    * gdb: build from latest source with `./configure --enable-targets=all --with-python=/usr/bin/python3`.
* MicroBlaze
    * toolchain: `microblazebe--glibc--stable-2022.08-1` from https://toolchains.bootlin.com/
    * qemu: `qemu-microblaze` via apt.
    * gdb: build from latest source with `./configure --enable-targets=all --with-python=/usr/bin/python3`.
* Xtensa
    * toolchain: `xtensa-lx60--uclibc--stable-2022.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain obtained with apt seems to be broken since the C header is unavailable.
    * qemu: `qemu-xtensa` via apt.
    * gdb: `xtensa-lx60--uclibc--stable-2022.08-1` from https://toolchains.bootlin.com/
        * Because `gdb` built from latest source will not work with `set architecture xtensa`.
        * The toolchain also includes `xtensa-linux-gdb`, so I used it.
        * The readline seems to be broken so workaround is here: `LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libncurses.so.6 xtensa-linux-gdb`.
* Cris
    * lib: `cris-dist_1.64-1_i386.deb` from https://www.axis.com/ftp/pub/axis/tools/cris/compiler-kit/
        * `mkdir dist && dpkg-deb -x cris-dist_1.64-1_i386.deb dist`
    * toolchain: `x86_64-gcc-7.3.0-nolibc_cris-linux.tar.xz` from http://ftp.iij.ad.jp/pub/linux/kernel/tools/crosstool/
        * `cd /lib/x86_64-linux-gnu && ln -s libisl.so.{23,15} && ln -s libmpfr.so.{6,4}`
        * `cris-linux-gcc -B/PATH/TO/dist/usr/local/cris/cris-axis-linux-gnu/lib -I/PATH/TO/dist/usr/local/cris/cris-axis-linux-gnu/sys-include -static ./test.c`
    * qemu: `qemu-cris` via apt.
        * Need `-cpu` option like `qemu-cris -cpu crisv17 -g 1234 ./a.out`.
        * Could not use `-cpu crisv32` because gdb does not support it.
    * gdb: build from latest source with `./configure --enable-targets=all --with-python=/usr/bin/python3`.
* Loongarch64
    * toolchain: https://github.com/loongson/build-tools/releases/download/2022.09.06/loongarch64-clfs-7.3-cross-tools-gcc-glibc.tar.xz
    * qemu: build from latest source with `./configure --target-list=loongarch64-linux-user`.
    * gdb: build from latest source with `./configure --enable-targets=all --with-python=/usr/bin/python3`.
