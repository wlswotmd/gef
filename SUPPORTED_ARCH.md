# Supported architectures
I also list the tools I used.

* x86
    * gcc: `gcc` via apt with `-m32` option.
    * qemu: `qemu-i386` via apt.
    * gdb: `gdb-multiarch` via apt.
* x64
    * gcc: `gcc` via apt.
    * qemu: `qemu-x86_64` via apt.
    * gdb: `gdb-multiarch` via apt.
* ARM
    * gcc: `gcc-arm-linux-gnueabihf` via apt.
    * qemu: `qemu-arm` via apt.
    * gdb: `gdb-multiarch` via apt.
* Aarch64
    * gcc: `gcc-aarch64-linux-gnu` via apt.
    * qemu: `qemu-aarch64` via apt.
    * gdb: `gdb-multiarch` via apt.
* PPC32
    * gcc: `gcc-powerpc-linux-gnu` via apt.
    * qemu: `qemu-ppc` via apt.
    * gdb: `gdb-multiarch` via apt.
* PPC64
    * gcc: `gcc-powerpc64-linux-gnu` and `gcc-powerpc64le-linux-gnu` via apt.
    * qemu: `qemu-ppc64` and `qemu-ppc64le` via apt.
    * gdb: `gdb-multiarch` via apt.
* MIPS32
    * gcc: `gcc-mips-linux-gnu` and `gcc-mipsel-linux-gnu` via apt.
    * qemu: `qemu-mips` and `qemu-mipsel` via apt.
    * gdb: `gdb-multiarch` via apt.
* MIPS64
    * gcc: `gcc-mips64-linux-gnuabi64` and `gcc-mips64el-linux-gnuabi64` via apt.
    * qemu: `qemu-mipsel` and `qemu-mips64el` via apt.
    * gdb: `gdb-multiarch` via apt.
* SPARC32
    * gcc: `sparcv8--uclibc--stable-2022.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain is not available through apt.
    * qemu: `qemu-sparc` via apt.
    * gdb: `gdb-multiarch` via apt.
* SPARC64
    * gcc: `sparc64--glibc--stable-2022.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain obtained with apt seems to be broken since the built ELF always SIGSEGV.
    * qemu: `qemu-sparc64` via apt.
    * gdb: `gdb-multiarch` via apt.
* RISCV32
    * gcc: `riscv32-ilp32d--glibc--bleeding-edge-2022.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain is not available through apt.
    * qemu: `qemu-riscv32` via apt.
    * gdb: `gdb-multiarch` via apt.
* RISCV64
    * gcc: `gcc-riscv64-linux-gnu` via apt.
    * qemu: `qemu-riscv64` via apt.
    * gdb: `gdb-multiarch` via apt.
* s390x
    * gcc: `gcc-s390x-linux-gnu` via apt.
    * qemu: `qemu-s390x` via apt.
    * gdb: `gdb-multiarch` via apt.
* sh4
    * gcc: `sh-sh4--uclibc--stable-2022.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain obtained with apt seems to be broken since static build is failed.
    * qemu: `qemu-sh4` via apt.
    * gdb: `gdb-multiarch` via apt.
* m68k
    * gcc: `gcc-m68k-linux-gnu` via apt.
    * qemu: `qemu-m68k` via apt.
    * gdb: `gdb-multiarch` via apt.
* alpha
    * gcc: `gcc-alpha-linux-gnu` via apt.
    * qemu: `qemu-alpha` via apt.
    * gdb: `gdb-multiarch` via apt.
* PA-RISC (HPPA)
    * gcc: `gcc-hppa-linux-gnu` via apt.
    * qemu: `qemu-hppa` via apt.
    * gdb: `gdb-multiarch` via apt.
* OpenRISC 1000 (OR1K)
    * gcc: `openrisc--glibc--stable-2022.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain is not available through apt.
    * qemu: `qemu-or1k` via apt.
    * gdb: `gdb` built from source with `./configure --enable-targets=all --with-python=/usr/bin/python3`.
        * Because `gdb-multiarch` does not support this architecture.
* NiosII
    * gcc: `nios2--glibc--stable-2022.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain is not available through apt.
    * qemu: `qemu-nios2` via apt.
    * gdb: `gdb` built from source with `./configure --enable-targets=all --with-python=/usr/bin/python3`.
        * Because `gdb-multiarch` does not support this architecture.
* MicroBlaze
    * gcc: `microblazeel--glibc--stable-2022.08-1` and `microblazebe--glibc--stable-2022.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain is not available through apt.
    * qemu: `qemu-microblazeel` and `qemu-microblaze` via apt.
    * gdb: `gdb` built from source with `./configure --enable-targets=all --with-python=/usr/bin/python3`.
        * Because `gdb-multiarch` does not support this architecture.
* Xtensa
    * gcc: `xtensa-lx60--uclibc--stable-2022.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain obtained with apt seems to be broken since the C header is unavailable.
    * qemu: `qemu-xtensa` via apt.
    * gdb: `xtensa-lx60--uclibc--stable-2022.08-1` from https://toolchains.bootlin.com/
        * Because neither `gdb-multiarch` nor `gdb` built from source support this architecture.
        * The toolchain also includes `xtensa-linux-gdb`, so I used it.
        * The readline seems to be broken so workaround is here: `LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libncurses.so.6 xtensa-linux-gdb`.
