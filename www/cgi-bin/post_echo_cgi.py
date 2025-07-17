#!/usr/bin/env python3
import os
import sys
from urllib.parse import parse_qs

def main():
    try:
        length = int(os.environ.get('CONTENT_LENGTH', 0))
    except ValueError:
        length = 0

    if length > 0:
        post_data = sys.stdin.read(length)
    else:
        post_data = sys.stdin.read()

    params = parse_qs(post_data)

    print("Content-Type: text/html\n")
    print("<html><body>")
    print("<h2>Received POST Data:</h2>")
    print("<ul>")
    for key, values in params.items():
        for value in values:
            print(f"<li>{key}: {value}</li>")
    print("</ul>")
    print("</body></html>")

if __name__ == "__main__":
    main()
