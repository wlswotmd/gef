## Table of Contents
* [What is this](#what-is-this)
* [Setup](#setup)
    * [Install](#install)
    * [Upgrade (replace itself)](#upgrade-replace-itself)
    * [Uninstall](#uninstall)
    * [Dependency](#dependency)
* [Added / Improved features](#added--improved-features)
    * [Qemu-system cooperation](#qemu-system-cooperation)
        * [General](#general)
        * [Linux specific](#linux-specific)
        * [Arch specific](#arch-specific)
        * [Other](#other)
    * [Qemu-user cooperation](#qemu-user-cooperation)
        * [General](#general-1)
    * [Heap dump features](#heap-dump-features)
    * [Other improved features](#other-improved-features)
    * [Other new features](#other-new-features)
    * [Other](#other-1)
* [Memo (Japanese)](#memo-japanese)

## What is this
This is a fork of [GEF](https://github.com/hugsy/gef).
However, it is specialized for x86 / x64 / ARM / AArch64, and various features are added.
I hope you find it useful for CTF player, reverser, exploit developer, and so on.

## Setup

### Install

```bash
# Run with root user (sudo is NOT recommended)
wget -q https://raw.githubusercontent.com/bata24/gef/dev/install.sh -O- | sh
```

### Upgrade (replace itself)
```bash
python3 /root/.gdbinit-gef.py --upgrade
```

### Uninstall

```bash
rm -f /root/.gdbinit-gef.py /root/.gef.rc
```

### Dependency
See [install.sh](https://github.com/bata24/gef/blob/dev/install.sh) or
[install-minimal.sh](https://github.com/bata24/gef/blob/dev/install-minimal.sh).

## Added / Improved features

All of these features are experimental. Tested on Ubuntu 22.04.

### Qemu-system cooperation
* It works with any version qemu-system, but qemu-6.x or higher is recommended.
    * Start qemu with the `-s` option and listen on `localhost:1234`.
    * Attach with `gdb-multiarch -ex 'target remote localhost:1234'`.
    * Or `gdb-multiarch -ex 'set architecture TARGET_ARCH' -ex 'target remote localhost:1234'`.

#### General
* `qreg`: displays the register values from qemu-monitor (allows to get like `$cs` even under qemu 2.x).
    * It is shortcut for `monitor info registers`.
    * It also prints the details of the each bit of the system register when x64/x86.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/qreg.png)
* `sysreg`: pretty prints system registers.
    * It is the result of `info registers` with filtering general registers.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/sysreg.png)
* `pagewalk`: displays the page table from scanning physical memory.
    * x64 (Supported: PML5T/PML4T)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pagewalk-x64.png)
    * x86 (Supported: PAE/Non-PAE)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pagewalk-x86.png)
    * ARM64 (Supported: EL1&0-stage1/EL1&0-stage2/EL2&0-stage1/EL2-stage1/EL3-stage1)
        * ARM v8.7 base.
        * 32bit mode is NOT supported.
        * PAC/MTE are NOT supported.
        * For stage2 translation, you have to do `pagewalk arm64 1` then `pagewalk arm64 2`.
        * Secure memory scanning is supported, but you have to break in the secure world.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pagewalk-arm64.png)
    * ARM (Cortex-A only, LPAE/Non-LPAE, PL0/PL1)
        * PL2 is NOT supported.
        * Secure memory scanning is supported, you don't have to break in the secure world (use register with `_S` suffix).
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pagewalk-arm.png)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pagewalk-arm-secure.png)
* `v2p`, `p2v`: displays transformation virtual address <-> physical address.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/v2p-p2v.png)
* `xp`: is a shortcut for physical memory dump.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/xp.png)

#### Linux specific
* `ksymaddr-remote`: displays kallsyms information from scanning kernel memory (heuristic).
    * Supported: the symbol of kernel itself.
    * Unsupported: the symbol of kernel modules.
    * Supported on x64/x86/ARM64/ARM.
    * Supported on both kASLR is enabled or not.
    * Unsupported: to resolve no-function address when kernel built as `CONFIG_KALLSYMS_ALL=n`.
    * This command is faster than `vmlinux-to-elf`, but it fails to parse depending on the in-memory layout.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/ksymaddr-remote.png)
* `ksymaddr-remote-apply`: applies kallsyms information obtained by `ksymaddr-remote` to gdb.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/ksymaddr-remote-apply.png)
* `vmlinux-to-elf-apply`: applies kallsyms information obtained by `vmlinux-to-elf` to gdb.
    * Very slow, but probably more accurate than my implementation.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/vmlinux-to-elf-apply1.png)
    * Once you get symboled vmlinux file, you can reuse and apply it automatically even after rebooting qemu-system.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/vmlinux-to-elf-apply2.png)
* `slub-dump`: dumps slub free-list (heuristic).
    * Original code: https://github.com/PaoloMonti42/salt
    * Supported on x64/x86/ARM64/ARM + SLUB.
    * Unsupported: SLAB, SLOB.
    * Supported on both kASLR is enabled or not.
    * Supported on both `CONFIG_SLAB_FREELIST_HARDENED` is `y` or `n`.
    * Supported on both the vmlinux symbol exists or not.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/slub-dump.png)
* `kbase`: displays the kernel base address.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/kbase.png)
* `kversion`: displays the debugged kernel version.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/kversion.png)
* `kcmdline`: displays the debugged kernel startup cmdline.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/kcmdline.png)
* `ktask`: displays each task address.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/ktask.png)
* `kmod`: displays each module address.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/kmod.png)
* `kcdev`: displays each character device information.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/kcdev.png)
* `kfops`: displays each fops member.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/kfops.png)
* `syscall-table-view`: displays system call table (x64/x86/ARM64/ARM only).
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/syscall-table-view.png)
    * It also dumps ia32/x32 syscall table under x64.
    * It also dumps compat syscall table under ARM64.
* `thunk-hunter`: collects and displays the thunk function addresses that are called automatically (x64/x86 only).
    * If this address comes from RW area, this is useful for getting RIP.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/thunk-hunter.png)
* `usermodehelper-hunter`: collects and displays the information that is executed by `call_usermodehelper_setup`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/usermodehelper-hunter.png)
* `magic`: displays useful addresses in kernel. Of cource, it also supports in userland.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/magic2.png)
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/magic1.png)

#### Arch specific
* `uefi-ovmf-info`: dumps addresses of some important structures in each boot phase of UEFI when OVMF is used (heuristic).
    * Supported on x64 only.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/uefi-ovmf-info.png)
* `msr`: displays MSR (Model Specific Registers) values by embedding/executing dynamic assembly.
    * Supported on x64/x86 WITHOUT `-enable-kvm`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/msr.png)
* `xsm`: dumps secure memory when gdb is in normal world.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/xsm.png)
* `wsm`: writes the value to secure memory when gdb is in normal world.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/wsm.png)
* `bsm`: sets the breakpoint to secure memory when gdb is in normal world.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/bsm.png)
* `optee-break-ta`: sets the breakpoint to the offset of OPTEE-Trusted-App when gdb is in normal world.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/optee-break-ta.png)

### Qemu-user cooperation
* It works with any version qemu-user, but qemu-6.x or higher is recommended.
    * Start qemu with the `-g 1234` option and listen on `localhost:1234`.
    * Attach with `gdb-multiarch -ex 'file /PATH/TO/BINARY' -ex 'target remote localhost:1234'`.
    * Or `gdb-multiarch -ex 'set architecture TARGET_ARCH' -ex 'target remote localhost:1234'`.
* Intel pin is supported.
    * Listen with `pin -appdebug -appdebug_server_port 1234 -t obj-intel64/inscount0.so -- /bin/ls`.
* Intel SDE is supported.
    * Listen with `sde64 -debug -debug-port 1234 -- /bin/ls`.

#### General
* Supported architecture
    * x86/x64
    * ARM(EABI)/Thumb2(EABI)/Aarch64
    * PPC32/PPC64
    * MIPS32(o32)/MIPS32(n32)/MIPS64(n64)
    * SPARC32(v8)/SPARC64(v9)
    * RISCV32/RISCV64
    * s390x
    * sh4
    * m68k
    * alpha
    * HPPA(PA-RISC)
* `vmmap`: is improved.
    * It displays the meomry map information even when connecting to gdb stub like qemu-user (heuristic), intel pin and intel SDE.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/vmmap-qemu-user.png)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/vmmap-pin.png)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/vmmap-sde.png)
    * It is redirected to `pagewalk` when connecting to gdb stub of qemu-system.
* `si`/`ni`: are the wrapper for native `si`/`ni`.
    * On some architectures such as s390x, a `PC not saved` error may be output when executing "stepi/nexti".
    * But the execution itself is fine, so this command ignores this error and executes `context` normally.
    * If you want native `si`/`ni`, use the full form `stepi`/`nexti`.
* `c`: is the wrapper for native `c`.
    * When connecting to qemu-user's gdb stub, gdb does not trap SIGINT during "continue".
    * If you want to trap, you need to issue SIGINT on the qemu-user side, but switching screens is troublesome.
    * This command realizes a pseudo SIGINT trap by trapping SIGINT on the python side and throwing SIGINT back to qemu-user.
    * It works local qemu-user only.
    * If you want native `c`, use the full form `continue`.

### Heap dump features
* `partition-alloc-dump-stable`: dumps partition-alloc free-list (heuristic).
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/partition-alloc-dump.png)
    * This command is reserved for the implementation of latest stable version of chromium.
        * Currently tested: v110.x / 1078401 / ac3cfdd3e961f4db164ab8de44c38c9ba34a8c1e
        * https://commondatastorage.googleapis.com/chromium-browser-snapshots/index.html?prefix=Linux_x64/1078401/
    * Supported on x64 only (maybe it works on x86/ARM/ARM64, but not tested).
    * It will try heuristic search if binary has no symbol.
* `tcmalloc-dump`: dumps tcmalloc free-list (heuristic).
    * For tcmalloc, there are 3 major versions.
        1. tcmalloc that is a part of gperftools published in 2005: supported.
        2. tcmalloc that is included in chromium: supported. (For the implementation in 2020 Jun).
        3. tcmalloc that is maintained in Google Inc. published in 2020: unsupported.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/tcmalloc-dump.png)
    * Not maintained for a while.
* `musl-dump`: dumps musl-libc unused chunks (heuristic).
    * Supported on x64/x86, based on musl-libc v1.2.2.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/musl-dump.png)
* `optee-bget-dump`: dumps bget allocator of OPTEE-Trusted-App.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/optee-bget-dump.png)

### Other improved features
* Glibc heap commands are improved.
    * Thread arena is supported for all heap commands.
        * Use `-a` option.
    * They print info if the chunk is in free-list.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/heap-if-in-freelist.png)
    * `find-fake-fast`: searches for a memory with a size-like value that can be linked to the fastbin free-list.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/find-fake-fast.png)
    * `visual-heap`: is colorized heap viewer.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/visual-heap.png)
    * `extract-heap-addr`: analyzes tcache-protected-fd introduced from glibc-2.32.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/extract-heap-addr.png)
* `registers`: is improved.
    * It also shows raw values of `$eflags` and `$cpsr`.
    * It displays current ring for x64/x86 when prints `$eflags` (Ring state is from `$cs`).
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/registers-x64.png)
    * It displays current exception level for ARM64 when prints `$cpsr` (Secure state is from `$SCR_EL3`).
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/registers-arm64.png)
    * It displays current mode for ARM when prints `$cpsr` (Secure state is from `$SCR`).
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/registers-arm.png)
* `context`: is improved.
    * It supports automatic display of system call arguments when calling a system call.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/context-syscall-args.png)
    * It supports automatic display of address and value when accessing memory.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/context-memory-access.png)
    * It supports smart symbol printing for cpp function.
        * ex: `std::map<int, std::map<int, int>>` will be replaced by `std::map<...>`.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/smart-cpp-function-name.png)
        * command: `gef config context.smart_cpp_function_name true` or `smart-cpp-function-name` (later is used to toggle).
* `telescope`: is improved.
    * It displays ordinal numbers as well as offsets.
    * It displays if there are canary and ret-addr on the target area.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/telescope.png)
    * It supports blacklist address features (to avoid dying when touching the address mapped to the serial device).
    * It also shows the symbol if available.
* `procinfo`: is improved.
    * It displays some additional informations.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/procinfo.png)
* `elf-info`: is improved.
    * It displays Program Header and Section Header.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/elf-info.png)
    * It supports parsing from memory.
* `checksec`: is improved.
    * It shows whether Static or Dynamic.
    * It shows whether Stripped or not.
    * It detects canary against static stripped binary.
    * It shows whether Intel CET instructions (endbr64/endbr32) is found or not.
    * It shows whether RPATH/RUNPATH is set or not.
    * It shows if Clang CFI/SafeStack is used or not.
    * It shows whether System-ASLR is enabled or not.
    * It shows whether GDB ASLR setting is enabled or not.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/checksec.png)
* `got`: is improved.
    * It displays not only GOT address but also PLT address.
    * It scans `.plt.sec` section if Intel CET is enabled.
    * It can also display the GOT of the library.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/got.png)
* `canary`: is improved.
    * It displays all canary positions in memory.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/canary.png)
* `edit-flags`: is improved.
    * It displays the meaning of each bit if `-v` option is provided.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/edit-flags-x64.png)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/edit-flags-arm.png)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/edit-flags-arm64.png)
* `unicorn-emulate`: is improved.
    * It reads and writes correctly to the address pointed to by `$fs`/`$gs`.
    * It supports a new mode to stop after executing N instructions (`-g`).
    * It shows changed memories.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/unicorn-emulate.png)
* `ropper`: is improved.
    * It does not reset autocomplete settings after calling imported ropper.
* `hexdump`: is improved.
    * It supports physical memory if under qemu-system.
    * It will retry with adjusting read size when failed reading memory.
    * By default, the same line is omitted.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/hexdump.png)
* `patch`: is improved.
    * It supports physical memory if under qemu-system.
    * Added some new modes: `pattern`, `hexstring`, `history`, `revert`, `nop`, `inf`, `trap` and `ret`.
        * `nop` command has been integrated into `patch` command.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/patch.png)
* `search-pattern`: is improved.
    * It supports when under qemu-system (in short, it works without `/proc/self/maps`)
    * It supports aligned search.
    * It supports hex string specification.
    * It also searches UTF-16 string if target string is ASCII.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/search-pattern.png)

### Other new features
* `pid`: prints pid.
* `filename`: prints filename.
* `auxv`: pretty prints ELF auxiliary vector.
    * Supported also under qemu-user (heuristic).
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/auxv.png)
* `argv`: pretty prints argv.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/argv.png)
* `envp`: pretty prints envp.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/envp.png)
* `gdtinfo`: pretty prints GDT sample.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/gdtinfo.png)
* `tls`: pretty prints TLS area.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/tls.png)
* `fsbase`,`gsbase`: pretty prints `$fs_base`, `$gs_base`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/fsbase_gsbase.png)
* `libc`/`ld`/`heapbase`/`codebase`: displays each of the base address.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/base.png)
* `fpu`/`mmx`/`sse`/`avx`: pretty prints FPU/MMX/SSE/AVX registers.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/fpu-mmx-sse-avx.png)
* `xmmset`: sets the value to xmm/ymm register simply.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/xmmset.png)
* `mmxset`: sets the value to mm register simply.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/mmxset.png)
* `exec-until`: executes until specified operation.
    * Supported following patterns of detection.
        * call
        * jmp
        * syscall
        * ret
        * indirect-branch (x86/x64 only)
        * all-branch (call || jmp || ret)
        * memory-access (detect "[")
        * specified-keyword-regex
        * specified-condition (expressions using register or memory values)
    * Please note that this command temporarily closes stdin and stderr on gdb.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/exec-until.png)
* `exec-next`: executes until next address.
    * This is useful for the operation with `rep` prefix.
* `add-symbol-temporary`: adds symbol information from command-line.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/add-symbol-temporary.png)
* `errno`: displays errno list or specified errno.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/errno.png)
* `u2d`: shows cast/transformation u64 <-> double/float.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/u2d.png)
* `pack`,`unpack`: shows transformation int <-> bytes/hex.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pack-unpack.png)
* `tohex`,`unhex`: shows transformation hex <-> bytes.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/tohex-unhex.png)
* `byteswap`: shows transformation little-endian <-> big-endian.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/byteswap.png)
* `hash-memory`: calculates the hash.
    * Supported: md5, sha1, sha224, sha256, sha384, sha512, crc16, crc32, crc64.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/hash-memory.png)
* `memcmp`: compares the contents of address A and B, whether virtual or physical.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/memcmp.png)
* `memcpy`: copies the contents from address A to B, whether virtual or physical.
* `is-mem-zero`: checks the contents of address range is all 0x00 or 0xff or not.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/is-mem-zero.png)
* `pdisas`: is a shortcut for `cs-dis $pc LENGTH=50 OPCODES`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pdisas.png)
* `ii`: is a shortcut for `x/50i $pc`.
    * It prints the value if it is memory access operation.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/ii.png)
* `version`: shows software version that gef used.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/version.png)
* `follow`: changes `follow-fork-mode` setting.
* `smart-cpp-function-name`: toggles `context.smart_cpp_function_name` setting.
* `seccomp`: invokes `seccomp-tools`.
* `onegadget`: invokes `one_gadget`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/onegadget.png)
* `ls`/`cat`: invokes `ls`/`cat` directly.
* `smart-memory-dump`: dumps all regions of the memory to each file.
* `mmap`: allocates a new memory (syntax sugar of `call mmap(...)`).
* `constgrep`: invokes `grep` under `/usr/include`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/constgrep.png)
* `time`: measures the time of the GDB command.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/time.png)
* `multi-line`: executes multiple GDB commands in sequence.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/multi-line.png)
* `rp`,`rp2`: invokes `rp++` with commonly used options.
    * Supports both rp++ v1 and v2.
* `cpuid`: shows the result of cpuid(eax=0,1,2...).
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/cpuid.png)
* `dasm`: disassembles the code by capstone.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/dasm.png)
* `asm-list`: lists up instructions. (only x86/x64)
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/asm-list.png)
    * This command uses x86data.js from https://github.com/asmjit/asmdb
* `syscall-search`: searches system call by regex.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/syscall-search.png)
* `dwarf-exception-handler`: dumps the DWARF exception handler informations.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/dwarf-exception-handler.png)
* `dynamic`: dumps the DYNAMIC area.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/dynamic.png)
* `linkmap`: dumps the linkmap with iterating.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/linkmap.png)
* `ret2dl-hint`: shows the structure used by Return-to-dl-resolve as hint.
* `srop-hint`: shows the code for Sigreturn-Oriented-Programming as hint.
* `dtor-dump`: dumps some destructor functions list.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/dtor-dump.png)
* `linklist-walk`: walks link list.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/linklist-walk.png)
* `ptr-demangle`: shows the demangled value of the value mangled by `PTR_MANGLE`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/ptr-demangle.png)
* `search-mangled-ptr`: searches the mangled value from RW memory.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/search-mangled-ptr.png)
* `capability`: shows the capabilities of the debugging process.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/capability.png)
* `arch-info`: shows architecture information.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/arch-info.png)

### Other
* The category is introduced in `gef help`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/gef-help.png)
* Combined into one file (from gef-extra).
    * The followings are moved from gef-extras.
        * `peek-pointers`, `current-stack-frame`, `xref-telescope`, `bytearray`, `bincompare`, `ftrace` and `v8deref`
    * This is because a single file is more attractive than ease of maintenance.
* The system-call table used by `syscall-args` is moved from gef-extras.
    * It was updated up to linux kernel 6.0.10 for each architecture.
* Removed some features I don't use.
    * `ida-interact`, `gef-remote`, `pie`, `pcustom`, `ksymaddr` and `shellcode`.
* Many bugs fix / formatting / made it easy for me to use.

## Memo (Japanese)
* Why i decided to make this
    * [gefを改造した話](https://hackmd.io/@bata24/rJVtBJsrP)
* The story behind each command, etc.
    * [bata24/gefの機能紹介とか](https://hackmd.io/@bata24/SycIO4qPi)
