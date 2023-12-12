## Build static gdbserver
I used debian-arm64 environment. See https://hackmd.io/lDob-hTUTfqIJyj0ahYY3A

```
wget https://ftp.gnu.org/gnu/gdb/gdb-14.1.tar.xz
tar xf gdb-14.1.tar.xz && cd gdb-14.1
apt install libgmp-dev libmpfr-dev
LDFLAGS=-static ./configure && make
scp -P 2222 root@localhost:/root/gdb-14.1/gdbserver/gdbserver .
```

## Build aarch64 buildroot
```
wget https://buildroot.org/downloads/buildroot-2023.11.tar.xz
tar xf buildroot-2023.11.tar.xz
cd buildroot-2023.11
cat << EOF >> board/qemu/aarch64-virt/linux.config
CONFIG_ARM64_HW_AFDBM=y
CONFIG_ARM64_PAN=y
CONFIG_AS_HAS_LDAPR=y
CONFIG_AS_HAS_LSE_ATOMICS=y
CONFIG_ARM64_LSE_ATOMICS=y
CONFIG_ARM64_USE_LSE_ATOMICS=y

CONFIG_AS_HAS_ARMV8_2=y
CONFIG_AS_HAS_SHA3=y
CONFIG_ARM64_RAS_EXTN=y
CONFIG_ARM64_CNP=y

CONFIG_ARM64_PTR_AUTH=y
CONFIG_ARM64_PTR_AUTH_KERNEL=y
CONFIG_CC_HAS_BRANCH_PROT_PAC_RET=y
CONFIG_CC_HAS_SIGN_RETURN_ADDRESS=y
CONFIG_AS_HAS_PAC=y
CONFIG_AS_HAS_CFI_NEGATE_RA_STATE=y

CONFIG_ARM64_AMU_EXTN=y
CONFIG_AS_HAS_ARMV8_4=y
CONFIG_ARM64_TLB_RANGE=y

CONFIG_AS_HAS_ARMV8_5=y
CONFIG_ARM64_BTI=y
CONFIG_CC_HAS_BRANCH_PROT_PAC_RET_BTI=y
CONFIG_ARM64_E0PD=y
CONFIG_ARM64_AS_HAS_MTE=y
CONFIG_ARM64_MTE=y

CONFIG_ARM64_EPAN=y
EOF

make qemu_aarch64_virt_defconfig
make
mkdir ~/qemu-aarch64-buildroot
cp output/images/{Image,rootfs.ext2} ~/qemu-aarch64-buildroot
```

## Start qemu-system
Add `mte=on`, `-cpu max` and `hostfwd` settings.

```
(host) cd ~/qemu-aarch64-buildroot

(host) qemu-system-aarch64 \
-machine virt,mte=on \
-cpu max \
-smp 1 \
-kernel Image \
-append "rootwait root=/dev/vda console=ttyAMA0" \
-netdev user,id=eth0,hostfwd=tcp:127.0.0.1:1234-:1234,hostfwd=tcp:127.0.0.1:1235-:1235 \
-device virtio-net-device,netdev=eth0 \
-drive file=rootfs.ext2,if=none,format=raw,id=hd0 \
-device virtio-blk-device,drive=hd0 \
-nographic
```

## Send the target binary and gdbserver
```
(host) python -m http.server 8080
(guest) wget http://HOST_IP:8080/main
(guest) wget http://HOST_IP:8080/gdbserver
(guest) chmod +x main gdbserver
```

## Start debugging
```
(guest) ./gdbserver 0.0.0.0:1234 ./main
(host) gdb-multiarch -q ./main -ex 'target remote :1234'
```
