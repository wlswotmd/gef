#!/bin/sh -eux

if [ "$(id -u)" != "0" ]; then
    echo "[-] Detected non-root user."
    echo "[-] INSTALLATION FAILED"
    exit 1
fi

echo "[+] apt"
apt-get update
apt-get install -y gdb-multiarch wget

GDBINIT_PATH="/root/.gdbinit"
GEF_PATH="$GDBINIT_PATH-gef.py"

echo "[+] download gef"
if [ -e "$GEF_PATH" ]; then
    echo "[-] $GEF_PATH already exists. Please delete or rename."
    echo "[-] INSTALLATION FAILED"
    exit 1
else
    wget -q https://raw.githubusercontent.com/bata24/gef/dev/gef.py -O "$GEF_PATH"
    if [ ! -s "$GEF_PATH" ]; then
        echo "Downloading $GEF_PATH failed."
        rm -f "$GEF_PATH"
        echo "[-] INSTALLATION FAILED"
        exit 1
    fi
fi

echo "[+] setup gef"
STARTUP_COMMAND="source $GEF_PATH"
if [ ! -e "$GDBINIT_PATH" ] || [ -z "$(grep "$STARTUP_COMMAND" "$GDBINIT_PATH")" ]; then
    echo "$STARTUP_COMMAND" >> "$GDBINIT_PATH"
fi

echo "[+] INSTALLATION SUCCESSFUL"
exit 0
