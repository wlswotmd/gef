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
* ARM (Cortex-A)
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
    * toolchain: `sparc64--glibc--bleeding-edge-2023.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain obtained with apt seems to be broken since the built ELF always SIGSEGV.
    * qemu: `qemu-sparc64` via apt.
    * gdb: `gdb-multiarch` via apt.
* RISCV32
    * toolchain: `riscv32-ilp32d--glibc--bleeding-edge-2023.08-1` from https://toolchains.bootlin.com/
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
    * toolchain: `sh-sh4--uclibc--bleeding-edge-2023.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain obtained with apt seems to be broken since static build is failed.
        * glibc version is broken. use uclibc version.
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
    * toolchain: `openrisc--glibc--bleeding-edge-2023.08-1` from https://toolchains.bootlin.com/
    * qemu: `qemu-or1k` via apt.
    * gdb: build from latest.
        * `./configure --enable-targets=all --with-python=/usr/bin/python3`
* NiosII
    * toolchain: `nios2--glibc--bleeding-edge-2023.08-1` from https://toolchains.bootlin.com/
    * qemu: `qemu-nios2` via apt.
    * gdb: build from latest.
        * `./configure --enable-targets=all --with-python=/usr/bin/python3`
* MicroBlaze
    * toolchain: `microblazebe--glibc--stable-2023.08-1` from https://toolchains.bootlin.com/
        * bleeding edge version is broken.
    * qemu: `qemu-microblaze` via apt.
    * gdb: build from latest.
        * `./configure --enable-targets=all --with-python=/usr/bin/python3`
* Xtensa
    * toolchain: `xtensa-lx60--uclibc--bleeding-edge-2023.08-1` from https://toolchains.bootlin.com/
        * Because the toolchain obtained with apt seems to be broken since the C header is unavailable.
    * qemu: `qemu-xtensa` via apt.
    * gdb: `xtensa-lx60--uclibc--bleeding-edge-2023.08-1` from https://toolchains.bootlin.com/
        * Because `gdb` built from latest source will not work with `set architecture xtensa`.
        * The toolchain also includes `xtensa-linux-gdb`, so I used it.
        * The readline seems to be broken so workaround is here: `LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libncurses.so.6 xtensa-linux-gdb`.
* Cris
    * lib: [cris-dist_1.64-1_i386.deb](https://www.axis.com/ftp/pub/axis/tools/cris/compiler-kit/cris-dist_1.64-1_i386.deb)
    * toolchain: [x86_64-gcc-7.3.0-nolibc_cris-linux.tar.xz](https://ftp.iij.ad.jp/pub/linux/kernel/tools/crosstool/files/bin/x86_64/7.3.0/x86_64-gcc-7.3.0-nolibc_cris-linux.tar.xz) from http://ftp.iij.ad.jp/pub/linux/kernel/tools/crosstool/
        * `mkdir cris-gcc-7.3.0-glibc && tar xf x86_64-gcc-7.3.0-nolibc_cris-linux.tar.xz -C cris-gcc-7.3.0-glibc --strip-components 1`
        * `cd cris-gcc-7.3.0-glibc && mkdir rootfs && dpkg-deb -x ../cris-dist_1.64-1_i386.deb rootfs`
        * `export CRIS=$(pwd)/rootfs/usr/local/cris/cris-axis-linux-gnu`
        * `cd /lib/x86_64-linux-gnu && ln -s libisl.so.{23,15} && ln -s libmpfr.so.{6,4}`
        * `cris-linux-gcc -B $CRIS/lib -I $CRIS/sys-include -static ./test.c`
    * qemu: `qemu-cris` via apt.
        * It needs `-cpu` option like `qemu-cris -cpu crisv17 -g 1234 ./a.out`.
        * Could not use `-cpu crisv32` because gdb does not support it.
    * gdb: build from latest.
        * `./configure --enable-targets=all --with-python=/usr/bin/python3`
* loongarch64
    * toolchain: [loongarch64-clfs-7.3-cross-tools-gcc-glibc.tar.xz](https://github.com/loongson/build-tools/releases/download/2022.09.06/loongarch64-clfs-7.3-cross-tools-gcc-glibc.tar.xz)
    * qemu: build from latest.
        * `./configure --target-list=loongarch64-linux-user`
    * gdb: build from latest.
        * `./configure --enable-targets=all --with-python=/usr/bin/python3`
* arc32 (HS38; ARCv2)
    * toolchain: `arcle-hs38--glibc--bleeding-edge-2023.08-1` from https://toolchains.bootlin.com/
    * qemu: https://github.com/foss-for-synopsys-dwc-arc-processors/qemu
        * `./configure --target-list=arc-linux-user`.
    * gdb: build from latest.
        * `./configure --enable-targets=all --with-python=/usr/bin/python3`
* arc32 (HS58; ARCv3)
    * toolchain: [arc_gnu_2023.03_prebuilt_arc32_uclibc_linux_install.tar.gz](https://github.com/foss-for-synopsys-dwc-arc-processors/toolchain/releases/download/arc-2023.03-release/arc_gnu_2023.03_prebuilt_arc32_uclibc_linux_install.tar.gz)
    * qemu: https://github.com/foss-for-synopsys-dwc-arc-processors/qemu
        * `./configure --target-list=arc-linux-user`
        * It needs `-cpu` option like `qemu-arc -cpu hs5x -g 1234 ./a.out`.
    * gdb: [arc-2023.03-rc1.tar.gz](https://github.com/foss-for-synopsys-dwc-arc-processors/binutils-gdb/archive/refs/tags/arc-2023.03-rc1.tar.gz)
        * `./configure --target=arc64-snps-linux-gnu --with-python=/usr/bin/python3`
        * This `--target` is right. It enables `arc64:64` and `arc64:32`.
* arc64 (HS68; ARCv3)
    * toolchain: [arc_gnu_2023.03_prebuilt_arc64_glibc_linux_install.tar.gz](https://github.com/foss-for-synopsys-dwc-arc-processors/toolchain/releases/download/arc-2023.03-release/arc_gnu_2023.03_prebuilt_arc64_glibc_linux_install.tar.gz)
    * qemu: https://github.com/foss-for-synopsys-dwc-arc-processors/qemu
        * `./configure --target-list=arc64-linux-user`
        * It needs `-cpu` option like `qemu-arc64 -cpu hs6x -g 1234 ./a.out`.
    * gdb: [arc-2023.03-rc1.tar.gz](https://github.com/foss-for-synopsys-dwc-arc-processors/binutils-gdb/archive/refs/tags/arc-2023.03-rc1.tar.gz)
        * `./configure --target=arc64-snps-linux-gnu --with-python=/usr/bin/python3`
* csky
    * toolchain: https://github.com/c-sky/toolchain-build
        * `git submodule update --init`
        * `./build-csky-gcc.py csky-gcc --src ./ --triple csky-unknown-linux-gnu --disable-gdb`
        * It fails if you build along with gdb, so add `--disable-gdb`.
    * qemu: https://github.com/T-head-Semi/qemu
        * `./configure --target-list=cskyv1-linux-user,cskyv1eb-linux-user,cskyv2-linux-user,cskyv2eb-linux-user`
    * gdb: build from latest.
        * `./configure --enable-targets=all --with-python=/usr/bin/python3`


# Qemu-user UNSUPPORTED architectures
These will be added if I find a combination that works.
If you find it, please let me know in the issue page.

* bfin:
    * [x] toolchain: `bfin--uclibc--bleeding-edge-2018.02-1` from https://toolchains.bootlin.com/
    * [ ] qemu: https://github.com/vapier/qemu
        * gdb stub is broken.
    * [x] gdb: build from latest.
        * `./configure --enable-targets=all --with-python=/usr/bin/python3`
* hexagon:
    * [x] toolchain: https://github.com/quic/toolchain_for_hexagon
    * [x] qemu: `qemu-hexagon` via apt.
    * [ ] gdb: not found.
* tilegx:
    * [x] toolchain: https://ftp.riken.jp/Linux/kernel.org/tools/crosstool/files/bin/x86_64/7.3.0/
    * [x] lib: http://www.voidrouter.net/archives/211
    * [ ] qemu: https://gist.github.com/bata24/3cad590158911de318c1baf898f49626
        * the breakpoint is broken.
    * [x] gdb: build from latest.
        * `./configure --enable-targets=all --with-python=/usr/bin/python3`
* hppa64
    * [x] toolchain: `gcc-hppa64-linux` via apt.
    * [ ] lib: not found.
    * [ ] qemu: not found.
    * [ ] gdb: not found.
* sparc32plus
    * [ ] toolchain: not found.
    * [x] qemu: `qemu-sparc32plus` via apt.
    * [x] gdb: `gdb-multiarch` via apt.
* loongarch32
    * [ ] toolchain: not found.
    * [ ] qemu: not found.
    * [x] gdb: build from latest.
        * `./configure --enable-targets=all --with-python=/usr/bin/python3`
* e2k
    * [ ] toolchain: not found.
    * [x] qemu: https://github.com/OpenE2K/qemu-e2k
        * I haven't confirmed whether it works yet.
    * [ ] gdb: not found.
* nds32
    * [x] toolchain: https://github.com/VincentZWC/prebuilt-nds32-v3f-toolchain
    * [ ] qemu: not found.
    * [ ] gdb: not found.
