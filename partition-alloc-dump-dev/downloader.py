#!/usr/bin/python3

import sys
import os
import re
import subprocess
import requests
import json

def get_base_pos(current_version):
  r = requests.get("https://raw.githubusercontent.com/vikyd/chromium-history-version-position/master/json/ver-pos-os/version-position-Linux_x64.json")
  j = json.loads(r.text)

  vlist = [("{:04d}.{:02d}.{:05d}.{:03d}".format(*map(int, k.split("."))), v) for k, v in j.items()]
  curr_ver = "{:04d}.{:02d}.{:05d}.{:03d}".format(*map(int, current_version.split("."))) 

  best = None
  for ver, pos in vlist:
    if ver == curr_ver:
      return pos
    if ver < curr_ver:
      continue
    if best is None:
      best = (ver, pos)
      continue
    if ver < best[0]:
      best = (ver, pos)
      continue
  return best[1]

def download_binary(channel):
  print("#"*50)
  print("[*] channel: {:s}".format(channel))
  current_version = subprocess.getoutput(f"python3 omahaproxy.py --os=linux --channel={channel} --field=current_version")
  print("[*] current_version: {:s}".format(current_version))
  pos = get_base_pos(current_version)
  print("[*] near position: {:s}".format(pos))

  dirname = f"./chrome_{channel}"
  os.makedirs(dirname, exist_ok=True)
  if not os.path.exists(f"{dirname}/chrome-linux-{pos}.zip"):
    url = 'https://www.googleapis.com/download/storage/v1/b/chromium-browser-snapshots/o/Linux_x64%2F{:s}%2Fchrome-linux.zip?&alt=media'.format(pos)
    subprocess.getoutput(f"wget -O {dirname}/chrome-linux-{pos}.zip '{url}'")
  else:
    print("  already exists, download is skipped")

  res = subprocess.getoutput(f"curl -L https://crrev.com/{pos}")
  r = re.findall(r"<title>(\S+) .*?</title>", res)
  if r:
    commit = r[0]
    url_base = "https://source.chromium.org/chromium/chromium/src/+/main:"
    print("[*] commit hash: {:s}".format(commit))
    print("[*] struct base::PartitionRoot")
    print("    {:s}base/allocator/partition_allocator/partition_root.h;drc={:s}".format(url_base, commit))
    print("[*] struct base::internal::PartitionBucket:")
    print("    {:s}base/allocator/partition_allocator/partition_bucket.h;drc={:s}".format(url_base, commit))
    print("[*] struct PartitionSuperPageExtentEntry:")
    print("    {:s}base/allocator/partition_allocator/partition_page.h;drc={:s}".format(url_base, commit))
    print("[*] struct PartitionDirectMapExtent:")
    print("    {:s}base/allocator/partition_allocator/partition_direct_map_extent.h;drc={:s}".format(url_base, commit))
    print("[*] struct SlotSpanMetadata:")
    print("    {:s}base/allocator/partition_allocator/partition_page.h;drc={:s}".format(url_base, commit))
    print("[*] v{:s}.x / {:s} / {:s}".format(current_version.split(".")[0], pos, commit))

  print()
  return

if __name__ == '__main__':
  if len(sys.argv) == 1:
    channels = ["stable", "beta", "dev"]
  else:
    channels = sys.argv[1:]

  for chan in channels:
      if chan in ["stable", "beta", "dev"]:
        download_binary(chan)
      else:
        print("channel is stable, beta, or dev")
        exit()

  print("[*] done\n\n")
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
