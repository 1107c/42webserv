#!/usr/bin/python3
import datetime

current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

html = f"""<html>
<body>
    <h1>Python CGI Time</h1>
    <p>Current Time: {current_time}</p>
</body>
</html>"""

print(html)