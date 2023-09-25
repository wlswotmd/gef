# FAQ


# About the environment

## Can I use GEF on an OS other than Ubuntu?
It's probably fine for regular Linux. I've used it on debian and WSL2.
Some users are running it on Arch Linux.
However, I have not confirmed that all commands work correctly.

## What kernel versions does GEF support?
I have confirmed that most commands work on versions 3.x ~ 6.5.x.
However, I have not verified every kernel version.
For example, certain symbols in some versions may not be supported by heuristic symbol detection.
Also, the structure may differ depending on the build config and the compiler that built the kernel.
So there may be environments where GEF does not work.
If you have any trouble, please report it on the issue page.

## GDB will not load GEF.
Are you using an environment other than Ubuntu 22.04?
This is probably because gdb does not support cooperation with python3.
Consider building gdb from source with `./configure --enable-targets=all --with-python=/usr/bin/python3 && make && make install`.

## Will each GEF command be more accurate if I have vmlinux with debug symbols?
No, whether vmlinux includes debug information has no effect on GEF behavior.
GEF always uses its own resolved address with `kallsyms-remote`.
It also performs its own heuristic structure member detection in each command.

## Does GEF support i386 16-bit mode (real mode)?
No, GEF does not support real mode.
Please consider using other scripts, such as [here](https://astralvx.com/debugging-16-bit-in-qemu-with-gdb-on-windows/).

## Is it possible to debug userland with GEF when using qemu-system?
Partially yes. I think it can be used when you want to track before and after a system call.
However, if KPTI is enabled, many kernel-related commands cannot be used.
The reason is that most memory access to kernel space is unavailable if KPTI is enabled.

## How do I break in userland when using qemu-system?
Use a hardware breakpoint.
When you are stopped inside the kernel, is it in the intended process context? If so, just use `break *ADDRESS` as usual.
But if you're stuck in the kernel context of a different user process than you expected, or in a kernel thread like `swapper/0`,
the virtual address of the process you wanted isn't mapped.
For this reason, software breakpoints that embed `0xcc` in virtual memory cannot be used in some situations.
However, hardware breakpoints can be used without any problems.


# About commands

## What command should I start with when debugging the kernel?
Try `vmlinux-to-elf-apply` or `ks-apply`, then `pagewalk` and `kchecksec`.
After that, try `slub-dump` and `buddy-dump` as well.
Other commands are less important, so check them with `gef help` if necessary.

## I prefer the AT&T style.
Please specify each time using the `set disassembly-flavor att` command.
Or, since the `set disassembly-flavor intel` command is executed in the GEF's main function, it may be a good idea to comment it out.
However, since GEF does not take AT&T syntax parsing into consideration, so some commands may do not work fine.
If you find a case where it doesn't work, please report it on the issue page.

## I don't like the color scheme.
Customize it using the `theme` command, then `gef save`. The config is saved to `~/.gef.rc` by default.
There is another option is disable colors. Try `gef config gef.disable_color True`.

## I don't want to add `-n` to every command to disable pager.
Try `gef config gef.always_no_pager True` then `gef save`.

## Is GEF possible to re-display the results of a command (for using less-pager)?
Basically you can't.
Please save as appropriate with `|$cat > /tmp/foo.txt` while the less-pager is running.
Or try `gef config gef.keep_pager_result True` then `gef save`. From next time onwards, temporary files will no longer be deleted.

Note: The `pagewalk` command and `pagewalk-with-hints` command can be redisplayed by `-c`.

## `ktask` (or other kernel related commands) does not work.
The kernel you are debugging may have been built with `CONFIG_RANDSTRUCT=y`.
In this case, except for a few commands, they will not work correctly.

## `vmmap` command does not recognize option.
When connected to qemu-system or vmware's gdb stub, the `vmmap` command is just redirected to the `pagewalk` command.
All options are ignored at this time. If you want to use some options, please use the `pagewalk` command instead of `vmmap` command.

## Is GEF possible to pass the result of a command to a shell command?
Yes, you can use built-in `pipe` command. For example, `pipe elf-info -n |grep .data` or `|pdisas |grep call`.


# About python

## Can I access each GEF command with python?
Yes, you can access by `__LCO__` that means loaded command objects. For example, `pi __LCO__["vmmap"]`.

## How can I get the instruction object?
You can get instruction object by `pi get_insn(addr=None)`.

There are also similar functions. Here are the list.
- `get_insn(addr=None)`
- `get_insn_next(addr=None)`
- `get_insn_prev(addr=None)`
- `gef_instruction_n(addr, n)`

## Are there any other globally accessible functions that are useful?
- Memory access
    - `write_memory(addr, data)`, `read_memory(addr, length)`
    - `is_valid_addr(addr)`
    - `read_int_from_memory(addr)`
    - `read_cstring_from_memory(addr, max_length=None, ascii_only=True)`
    - `read_physmem(paddr, size)`, `write_physmem(paddr, data)`
    - `read_physmem_secure(paddr, size)`, `write_physmem_secure(paddr, data)`
- Register access
    - `get_register(regname, use_mbed_exec=False, use_monitor=False)`
- Other
    - `str2bytes(x)`, `bytes2str(x)`
    - `slicer(data, n)`, `slice_unpack(data, n)`
    - `p8`, `p16`, `p32`, `p64`
    - `u8`, `u16`, `u32`, `u64`, `u128`


# About development schedule

## Are there any plans to support kernels for other architectures?
There are no plans.
If implemented in the future, the architecture must at least disclose the `pagewalk` process in detail.
This is because a memory map is required to obtain the symbols, and a `pagewalk` implementation is required to obtain the memory map.

## Are there any plans to support more architectures with Qemu-user?
Yes, however, it is becoming difficult to find new support targets.
This is because three things are required:

1. toolchain
    * linux-headers, binutils, gcc, glibc (or uclibc) are needed.
    * Prebuilt tarball is prefer.
2. qemu-user
3. gdb
    * It needs python3-support.

## Are future development plans listed somewhere?
No, I develop this forked GEF freely.


# About reporting, etc.

## I found a bug.
Please feel free to report it on the issue page. I will respond as soon as possible.

## Can you please add this feature? / I don't like the XXX, so please fix it.
I will consider it, so please report it on the issue page.
But this is a personal development, so I have the final say. I appreciate your understanding.

## Is there any information that should be provided when reporting?
You will need a screenshot or a copy of the terminal string when the problem occurred.
In addition, I am glad if there are the results of the `version` command and `arch-info` command.

## Is it okay to fork and modify?
Yes, however, please follow the license.


# About me

## What kind of environment are you developing in?
I don't use anything special.
I am using a test environment built with buildroot while viewing the Linux kernel source with Bootlin.
I also use images of kernel exploit task from variuos CTFs.

## Why is there no master branch?
I'm a git beginner, so one dev branch is the best I can do.

## Are you bad at English?
Yes, I mostly use Google Translate. This is the reason for the inconsistent writing style.
If the expression in English is strange, please feel free to correct it on the issue page.

## I would like to contact you about a GEF?
Find `bata___` on Discord of official GEF's server or Pwndbg's server.
Or send DM to [`@bata_24` on X](https://twitter.com/bata_24).


# Other memo (Japanese)
* Why I decided to make this
    * [gefを改造した話](https://hackmd.io/@bata24/rJVtBJsrP)
* The story behind each command, etc.
    * [bata24/gefの機能紹介とか](https://hackmd.io/@bata24/SycIO4qPi)
