#!/usr/bin/python3

import sys

def main():
    print("Enter text (press Ctrl+D or Ctrl+Z to end):")
    for line in sys.stdin:
        # Strip newline characters and reverse the line
        transformed_line = line.strip()[::-1]
        print(transformed_line)

if __name__ == "__main__":
    main()