#!/usr/bin/env python3
import os
import sys
import cgi

def main():
    form = cgi.FieldStorage()

    fileitem = form['uploadfile'] if 'uploadfile' in form else None

    print("Content-Type: text/html\n")

    if fileitem is None or not fileitem.filename:
        print("<html><body><h2>No file uploaded</h2></body></html>")
        return

    # Sanitize filename (basic)
    filename = os.path.basename(fileitem.filename)

    # Save to disk
    with open(f"uploads/{filename}", "wb") as f:
        f.write(fileitem.file.read())

    print("<html><body>")
    print(f"<h2>File '{filename}' uploaded successfully!</h2>")
    print(f'<img src="/{filename}" alt="Uploaded Image" />')
    print("</body></html>")

if __name__ == "__main__":
    main()
