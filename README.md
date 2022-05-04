## Install

```bash
# Run with root user
wget -q https://raw.githubusercontent.com/bata24/gef/dev/install.sh -O- | sh
```

## Upgrade (replace itself)
```bash
python3 /root/.gdbinit-gef.py --upgrade
```

## Uninstall

```bash
rm -f /root/.gdbinit-gef.py /root/.gef.rc
```

## Added / Improved features

All of these features are experimental. Tested on Ubuntu 18.04 / 20.04 / 22.04 / Debian 10.x.

### qemu-system cooperation
* It works with qemu-system installed via apt, but qemu-6.x is recommended.
    * Start qemu with the `-s` option and listen on `localhost:1234`.
    * Attach with `gdb-multiarch -ex 'target remote localhost:1234'`.
* `qreg`: prints register values from qemu-monitor (allows to get like `$cs` even under qemu 2.x).
    * It is shortcut for `monitor info registers`.
    * It also prints the details of the each bit of the system register when x64/x86.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/qreg.png)
* `sysreg`: pretty prints system registers.
    * It is the result of `info registers` with filtering general registers.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/sysreg.png)
* `msr`: prints MSR (Model Specific Registers) values by embedding/executing dynamic assembly.
    * Supported on x64/x86 without `-enable-kvm`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/msr.png)
* `pagewalk`: prints pagetables from scanning of physical memory.
    * x64 (Supported: PML5T/PML4T)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pagewalk-x64.png)
    * x86 (Supported: PAE/Non-PAE)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pagewalk-x86.png)
    * ARM64 (Supported: EL0/EL1/EL2/VEL2/EL3)
        * ARM v8.3 base.
        * 32bit mode is NOT supported.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pagewalk-arm64.png)
    * ARM (Cortex-A only, LPAE/Non-LPAE, PL0/PL1)
        * PL2 is NOT supported.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pagewalk-arm.png)
* `xp`: is a shortcut for physical memory dump.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/xp.png)
* `ksymaddr-remote`: prints kallsyms informations from scanning of kernel memory (heuristic).
    * Supported: the symbol of kernel itself.
    * Unsupported: the symbol of kernel modules.
    * Supported on x64/x86/ARM64/ARM.
    * Supported on both kASLR is enabled or not.
    * Unsupported: to resolve no-function address when kernel built as `CONFIG_KALLSYMS_ALL=n`.
    * This command is faster than `vmlinux-to-elf`, but it fails to parse depending on the in-memory layout.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/ksymaddr-remote.png)
* `ksymaddr-remote-apply`: applies kallsyms informations obtained by `ksymaddr-remote` to gdb.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/ksymaddr-remote-apply.png)
* `vmlinux-to-elf-apply`: applies kallsyms informations obtained by `vmlinux-to-elf` to gdb.
    * Very slow, but probably more accurate than my implementation.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/vmlinux-to-elf-apply1.png)
    * Once you get symboled vmlinux file, you can reuse and apply it automatically even after rebooting qemu-system.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/vmlinux-to-elf-apply2.png)
* `slab`: dumps slab free-list (heuristic).
    * Original code: https://github.com/PaoloMonti42/salt
    * Supported on x64/x86/ARM64/ARM + SLUB.
    * Unsupported: SLAB, SLOB.
    * Supported on both kASLR is enabled or not.
    * Supported on both `CONFIG_SLAB_FREELIST_HARDENED` is `y` or `n`.
    * Supported on both the vmlinux symbol exists or not.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/slab.png)
* `thunk-hunter`: collects and displays the thunk addresses that are called automatically (x64/x86 only).
    * If this address comes from RW area, this is useful for getting RIP.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/thunk-hunter.png)
* `usermodehelper-hunter`: collects and displays information that is executed by `call_usermodehelper_setup`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/usermodehelper-hunter.png)
* `uefi-ovmf-info`: displays addresses of some important structures in each boot phase of UEFI when OVMF is used (heuristic).
    * Supported on x64 only.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/uefi-ovmf-info.png)
* `kbase`: prints kernel base address.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/kbase.png)
* `kversion`: displays the debugged kernel version.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/kversion.png)
* `kcmdline`: displays the debugged kernel startup cmdline.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/kcmdline.png)
* `ktask`: displays each task address.
    * It needs `CONFIG_KALLSYMS_ALL=y`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/ktask.png)
* `syscall-table-view`: prints system call table (x64/x86/ARM64/ARM only).
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/syscall-table-view.png)

### Heap dump features
* `partition-alloc-dump-stable`: dumps partition-alloc free-list (heuristic).
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/partition-alloc-dump.png)
    * This command is reserved for the implementation of latest stable version of chromium.
        * Currently tested: v101.x / 982481 / 27de6227ca357da0d57ae2c7b18da170c4651438
        * https://commondatastorage.googleapis.com/chromium-browser-snapshots/index.html?prefix=Linux_x64/982481/
    * Supported on x64 only (maybe it works on x86/ARM/ARM64, but not tested).
    * It will try heuristic search if binary has no symbol.
* `partition-alloc-dump-beta`: dumps partition-alloc free-list (heuristic).
    * This command is reserved for the implementation of latest beta version of chromium.
        * Currently tested: v102.x / 993115 / 1c0f7c93c8de06acb82d7fbaac7ab31aa57ed37e
        * https://commondatastorage.googleapis.com/chromium-browser-snapshots/index.html?prefix=Linux_x64/993115/
* `partition-alloc-dump-dev`: dumps partition-alloc free-list (heuristic).
    * This command is reserved for the implementation of latest dev version of chromium.
        * Currently tested: v103.x / 997158 / 56e9cfac915155c80579a73376793d57d65927fb
        * https://commondatastorage.googleapis.com/chromium-browser-snapshots/index.html?prefix=Linux_x64/997158/
* `partition-alloc-dump-old1`: dumps partition-alloc free-list (heuristic).
    * For the implementation in 2021 Jul (tested on `Google CTF 2021 - fullchain`).
    * Not maintained for a while.
* `partition-alloc-dump-old2`: dumps partition-alloc free-list (heuristic).
    * For the implementation in 2020 Jun (tested on `0CTF 2020 - chromium fullchain`).
    * Not maintained for a while.
* `tcmalloc-dump`: dumps tcmalloc free-list (heuristic).
    * For tcmalloc, there are 3 major versions.
        1. tcmalloc that is a part of gperftools published in 2005: supported.
        2. tcmalloc that is included in chromium: supported. (For the implementation in 2020 Jun. Tested on `0CTF 2020 - chromium fullchain`).
        3. tcmalloc that is maintained in Google Inc. published in 2020: unsupported.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/tcmalloc-dump.png)
    * Not maintained for a while.
* `musl-dump`: dumps musl-libc unused chunks (heuristic).
    * Supported on x64/x86, based on musl-libc v1.2.2.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/musl-dump.png)

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
* `vmmap`: is improved.
    * It prints meomry map informations even when connecting to gdb stub like qemu-user (heuristic), intel pin and intel SDE.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/vmmap-qemu-user.png)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/vmmap-pin.png)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/vmmap-sde.png)
    * It is redirected to `pagewalk` when connecting to gdb stub of qemu-system.
* `registers`: is improved.
    * It also shows raw values of `$eflags` and `$cpsr`.
    * It prints current ring for x64/x86 when prints `$eflags` (from `$cs`).
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/registers-x64.png)
    * It prints current exception level for ARM64 when prints `$cpsr`.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/registers-arm64.png)
    * It prints current mode for ARM when prints `$cpsr`.
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
    * It prints ordinal numbers as well as offsets.
    * It prints if there are canary and ret-addr on the target area.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/telescope.png)
    * It supports blacklist address features (to avoid dying when touching the address mapped to the serial device).
    * It also shows the symbol if available.
* `procinfo`: is improved.
    * It prints some additional informations.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/procinfo.png)
* `elf-info`: is improved.
    * It prints Program Header and Section Header.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/elf-info.png)
    * It supports parsing from memory.
* `checksec`: is improved.
    * It prints whether Static or Dynamic.
    * It prints whether Stripped or not.
    * It detects canary against static stripped binary.
    * It prints whether Intel CET is enabled or not.
    * It prints whether RPATH/RUNPATH is set or not.
    * It prints if Clang CFI/SafeStack is used or not.
    * It prints whether System-ASLR is enabled or not.
    * It prints whether GDB ASLR setting is enabled or not.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/checksec.png)
* `got`: improved.
    * It prints not only GOT address but also PLT address.
    * It scans `.plt.sec` section if Intel CET is enabled.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/got.png)
* `canary`: is improved.
    * It prints all canary positions in memory.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/canary.png)
* `edit-flags`: is improved.
    * It prints the meaning of each bit if `-v` option is provided.
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/edit-flags-x64.png)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/edit-flags-arm.png)
        * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/edit-flags-arm64.png)
* `unicorn-emulate`: is improved.
    * It reads and writes correctly to the address pointed to by `$fs`/`$gs`.
    * It supports a new mode to stop after executing N instructions (`-g`).
    * It supports execution under qemu-usermode partially (It is unable to emulate TLS if TLS is not initialized).
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
    * Added 3 new modes: `patch pattern`, `patch history` and `patch revert`.
    * Added a new option to write repeatedly up to the specified length for `patch string`.
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
* `magic`: is useful addresses resolver in gilbc.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/magic.png)
* `libc`/`ld`/`heapbase`/`codebase`: prints each of the base address.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/base.png)
* `fpu`/`mmx`/`sse`/`avx`: pretty prints FPU/MMX/SSE/AVX registers.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/fpu-mmx-sse-avx.png)
* `xmmset`: sets the value to xmm/ymm register simply.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/xmmset.png)
* `mmxset`: sets the value to mm register simply.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/mmxset.png)
* `exec-until`: executes until specific operation.
    * Supported on x64/x86/ARM64/ARM for call/jmp/syscall/ret/memory-access/specific-keyword-regex/specific-condition.
    * Supported on x64/x86 for indirect-branch.
    * Please note that this command temporarily closes stdin and stderr on gdb.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/exec-until.png)
* `exec-next`: executes until next address.
    * This is useful for the operation with `rep` prefix.
* `add-symbol-temporary`: adds symbol information from command-line.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/add-symbol-temporary.png)
* `errno`: prints errno list or specific errno.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/errno.png)
* `u2d`: is cast/transformation u64 <-> double/float.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/u2d.png)
* `pack`,`unpack`: is transformation int <-> bytes.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pack.png)
* `hash-memory`: calculations hash.
    * Supported: md5, sha1, sha224, sha256, sha384, sha512, crc16, crc32, crc64.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/hash-memory.png)
* `memcmp`: compares the contents of address A and B, whether virtual or physical.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/memcmp.png)
* `is-mem-zero`: checks the contents of address range is all 0x00 or 0xff or not.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/is-mem-zero.png)
* `byteswap`: is transformation little-endian <-> big-endian.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/byteswap.png)
* `pdisas`: is a shortcut for `cs-dis $pc LENGTH=50 OPCODES`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/pdisas.png)
* `ii`: is a shortcut for `x/50i $pc`.
    * It prints the value if memory access operation.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/ii.png)
* `version`: shows software version that gef used.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/version.png)
* `follow`: changes `follow-fork-mode` setting.
* `smart-cpp-function-name`: toggles `context.smart_cpp_function_name` setting.
* `seccomp`: invokes `seccomp-tools`.
* `onegadget`: invokes `one_gadget`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/onegadget.png)
* `ls`/`cat`: invokes `ls`/`cat` directly.
* `constgrep`: invokes `grep` under `/usr/include`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/constgrep.png)
* `time`: measures the time of the GDB command.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/time.png)
* `rp`: invokes `rp++` with commonly used options.
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
* `linkmap`: dumps linkmap with iterating.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/linkmap.png)
* `ret2dl-hint`: show the structure used by ret2dl as hint.
* `srop-hint`: show the code for srop as hint.
* `dtor-dump`: dumps some destructor functions list.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/dtor-dump.png)

### Other
* Replace the unicode character to ASCII.
    * I don't want to use double-byte characters.
    * ex: &#x27a4; to `>`.
    * ex: &#x2713; to `OK`.
* The category is introduced in `gef help`.
    * ![](https://raw.githubusercontent.com/bata24/gef/dev/images/gef-help.png)
* Combined into one file (from gef-extra).
    * `peek-pointers`, `current-stack-frame`, `xref-telescope`, `bytearray`, `bincompare`, `ftrace` and `v8deref` are moved from gef-extras.
    * This is because a single file is more attractive than ease of maintenance.
* The system-call table used by `syscall-args` is moved from gef-extras.
    * It was updated up to linux kernel 5.16.16 (only x64/x86/ARM64/ARM).
    * Since there are many exceptions at system calls for each architecture, arguments information of system call was picked up manually.
* Removed some features I don't use.
    * `ida-interact`, `gef-remote`, `pie` and `pcustom`.
* Many bugs fix / formatting / made it easy for me to use.
