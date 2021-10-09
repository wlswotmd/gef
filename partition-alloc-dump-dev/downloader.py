#!/usr/bin/python3

import sys
import os
import re
import subprocess

def get_available_url(pos):
  pos = int(pos)
  while pos:
    print("  trying pos: {:d}".format(pos))
    url = 'https://www.googleapis.com/download/storage/v1/b/chromium-browser-snapshots/o/Linux_x64%2F{:d}%2Fchrome-linux.zip?&alt=media'.format(pos)
    ret = subprocess.getoutput(f"curl -LI '{url}'")
    if "HTTP/2 200" in ret:
      print("[*] download url: {:s}".format(url))
      return url, pos
    pos -= 1
  return None, None

def download_binary(channel):
  print("#"*50)
  print("[*] channel: {:s}".format(channel))
  current_version = subprocess.getoutput(f"python3 omahaproxy.py --os=linux --channel={channel} --field=current_version")
  print("[*] current_version: {:s}".format(current_version))
  base_pos = subprocess.getoutput(f"python3 omahaproxy.py --os=linux --channel={channel} --field=branch_base_position")
  print("[*] branch_base_position: {:s}".format(base_pos))

  url, pos = get_available_url(base_pos)
  if url is None:
    print("[-] not found available url")
    return

  dirname = f"./chrome_{channel}"
  os.makedirs(dirname, exist_ok=True)
  if not os.path.exists(f"{dirname}/chrome-linux-{pos}.zip"):
    subprocess.getoutput(f"wget -O {dirname}/chrome-linux-{pos}.zip '{url}'")
  else:
    print("  already exists, download is skipped")

  res = subprocess.getoutput(f"curl -L https://crrev.com/{pos}")
  r = re.findall(r"<title>(\S+) .*?</title>", res)
  if r:
    commit = r[0]
    print("[*] commit hash: {:s}".format(commit))
    print("[*] src: https://source.chromium.org/chromium/chromium/src/+/main:base/allocator/partition_allocator/partition_root.h;drc={:s}".format(commit))

  print()
  return

if __name__ == '__main__':
  for channel in ["stable", "beta", "dev"]:
    download_binary(channel)
  print("[*] done")
  print()
  print()

  print("[*] memo")
  print("  [term1]")
  print("    cd www && python3 -m http.server 8080")
  print("  [term2]")
  print("    cd chrome_stable/chrome-linux/")
  print("    rm -rf /tmp/u && sudo -u nobody ./chrome --headless --disable-gpu --remote-debugging-port=1338 --user-data-dir=/tmp/u --enable-logging=stderr http://0:8080/inf-loop.html")
  print("  [term3 (for renderer process)]")
  print("""    gdb -q -p $(ps -ef | grep -- "--[t]ype=renderer" | awk '{print $2}')""")
  print("  [term3 (for browser process)]")
  print("""    gdb -q -p $(ps -ef | grep ./chrome | grep -v type | awk '{print $2}')""")
