#!/usr/bin/python3

import os

print("Content-Type: text/plain")
print("")
for k, v in os.environ.items():
    print(f"{k}={v}")