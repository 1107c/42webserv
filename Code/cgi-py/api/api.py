import sys
import json
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_dir)
file_path = '../../html/api.js'
body = ""

try:
	with open(file_path, 'r') as file:
		body = file.read()
	print(f"HTTP/1.1 200 OK\r\nContent-Type: application/javascript\r\nContent-Length: {len(body)}\r\n\r\n{body}")

except FileNotFoundError:
	body = json.dumps({
		"status": "fail",
		})
	print(f"HTTP/1.1 404 OK\r\nContent-Type: text/plain\r\nContent-Length: {len(body)}\r\n\r\n{body}")
