# Supported mode

## Qemu-system cooperation
* Usage
    * Start qemu-system with the `-s` option and listen on `localhost:1234`.
        * If you want to change the listening port, use like `-gdb tcp::9876` option.
    * Attach with `gdb-multiarch -ex 'target remote localhost:1234'`.
    * Or `gdb-multiarch -ex 'set architecture TARGET_ARCH' -ex 'target remote localhost:1234'` (for old qemu).
* Supported architectures
    * x86, x64, ARM and ARM64
    * i8086 (16-bit) is supported experimentally.
* Note
    * Most commands should work fine unless `CONFIG_RANDSTRUCT=y`.
    * It works with any version qemu-system, but latest version is recommended.
    * It is preferable to run qemu-system on `localhost`.
        * If you run qemu-system on remotely (another host), you can not handle SecureWorld's memory.
    * See [docs/FAQ.md](https://github.com/bata24/gef/blob/dev/docs/FAQ.md) for more informations.

## Qemu-user cooperation
* Usage
    * Start qemu-user with the `-g 1234` option and listen on `localhost:1234`.
    * Attach with `gdb-multiarch /PATH/TO/BINARY -ex 'target remote localhost:1234'`.
    * Or `gdb-multiarch -ex 'set architecture TARGET_ARCH' -ex 'target remote localhost:1234'` (for old qemu).
* Supported architectures
    * See [docs/QEMU-USER-SUPPORTED-ARCH.md](https://github.com/bata24/gef/blob/dev/docs/QEMU-USER-SUPPORTED-ARCH.md)
* Note
    * It works with any version qemu-user, but latest version is recommended.
        * This is because from qemu-user 8.1, `info proc mappings` command is available, which makes memory map generation faster.
        * In some architectures this may not be possible (e.g. `x86_64`), in which case it will fall back to heuristic detection.
    * It is preferable to run qemu-user on `localhost`.
        * If you run qemu-user on remotely (another host), you can not use the memory patching.

## Intel Pin/SDE cooperation
* Usage for Intel Pin
    * Listen with `pin -appdebug -appdebug_server_port 1234 -t obj-intel64/inscount0.so -- /PATH/TO/BINARY`.
    * Attach with `gdb-multiarch /PATH/TO/BINARY -ex 'target remote localhost:1234'`.
* Usage for Intel SDE
    * Listen with `sde64 -debug -debug-port 1234 -- /PATH/TO/BINARY`.
    * Attach with `gdb-multiarch /PATH/TO/BINARY -ex 'target remote localhost:1234'`.
* Supported architectures
    * x64 only.
* Note
    * It runs very slowly and is not recommended.

## Qiling framework cooperation
* Usage
    * `qltool run -f /PATH/TO/BINARY --rootfs / --gdb :1234`.
    * Or write a harness. See [here](https://docs.qiling.io/en/latest/debugger/).
    * Attach with `gdb-multiarch /PATH/TO/BINARY -ex 'target remote localhost:1234'`.
* Supported architectures
    * x86, x64, ARM and ARM64.
* Note
    * On ARM64, the flag register is not available, so the branch taken/not detected is incorrect.
    * This is an experimental support.

## KGDB cooperation
* Usage
    * Host OS
        * Configure the serial port as a named pipe in your two (debugger/debuggee) virtual machine settings, such as VMware or VirtualBox.
    * Debuggee
        * Build the kernel as `CONFIG_KGDB=y`. Ubuntu has supported it by default.
        * Edit `/etc/default/grub` and append `kgdboc=ttyS0,115200 kgdbwait` to the end of `GRUB_CMDLINE_LINUX_DEFAULT`.
        * Then `update-grub && reboot`.
    * Debugger
        * Attach with `gdb-multiarch -ex 'target remote /dev/ttyS0'`.
* Supported architectures
    * x64 only.
* Note
    * It needs gdb 12.x or after.
    * It runs very slowly and is not recommended. Ctrl+C interrupt does not work.
    * Many commands are UNSUPPORTED in KGDB mode, because there is no way to access physical memory and control registers.

### VMware cooperation
* Usage
    * Host OS
        * Add following configurations to vmx file.
            * `debugStub.listen.guest64 = "TRUE"`
            * `debugStub.listen.guest64.remote = "TRUE"`
            * `debugStub.hideBreakpoints = "TRUE"`
            * `debugStub.port.guest64 = "1234"`
            * See [here](https://communities.vmware.com/t5/VMware-Fusion-Discussions/Using-debugStub-to-debug-a-guest-linux-kernel/td-p/394906).
        * Start the guest OS normally.
    * Debugger
        * Attach with `gdb-multiarch -ex 'target remote <ipaddr>:1234'`.
* Supported architectures
    * x64 only.
* Note
    * It runs faster than KGDB mode and Ctrl+C interrupt works, but it is still slow.
    * Access to physical memory and control registers is possible thanks to the `monitor` command.

### rr cooperation
* Usage
    * It can be used just like normal GDB.
* Note
    * This is an experimental support.
    * Some commands may not work.
