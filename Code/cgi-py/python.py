#!/usr/bin/python3
import datetime

current_time = datetime.datetime.now().strftimase("%Y-%m-%d %H:%M:%S")

html = f"""<html>
<body>
    <h1>Python CGI Time</h1>
    <p>Current Time: {current_time}</p>
</body>
</html>"""
header = f"""HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: {len(html)}\r\n\r\n"""

print(header + html)