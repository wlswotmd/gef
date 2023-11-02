#!/bin/sh -eux

if [ ! $(id -u) = 0 ]; then
    echo "[-] Detected non-root user."
    echo "[-] INSTALLATION FAILED"
    exit 1
fi

echo "[+] apt"
apt-get update
apt-get install -y gdb-multiarch binutils gcc file

echo "[+] download gef"
if [ -e /root/.gdbinit-gef.py ]; then
    echo "[-] /root/.gdbinit-gef.py already exists. Please delete or rename."
    echo "[-] INSTALLATION FAILED"
    exit 1
else
    wget -q https://raw.githubusercontent.com/bata24/gef/dev/gef.py -O /root/.gdbinit-gef.py
fi

echo "[+] setup gef"
STARTUP_COMMAND="source /root/.gdbinit-gef.py"
if [ ! -e /root/.gdbinit ] || [ "x$(grep "$STARTUP_COMMAND" /root/.gdbinit)" = "x" ]; then
    echo "$STARTUP_COMMAND" >> /root/.gdbinit
fi

echo "[+] INSTALLATION SUCCESSFUL"
exit 0
