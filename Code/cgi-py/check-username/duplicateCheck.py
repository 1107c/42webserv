import sys
import json
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_dir)
file_path = '../../User/data.json'
try:
	with open(file_path, 'r') as file:
		data = json.load(file)
	user_id = sys.argv[2]
	user_exists = any(user['id'] == user_id for user in data.get("users", []))
	
	if user_exists:
		body = "duplicate"
	else:
		body = "available"

	print(f"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: {len(body)}\r\n\r\n{body}")

except FileNotFoundError:
	body = "available"
	print(f"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: {len(body)}\r\n\r\n{body}")
