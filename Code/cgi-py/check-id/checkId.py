import sys
import json
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_dir)
file_path = '../../User/data.json'

try:
	with open(file_path, 'r') as file:
		data = json.load(file)

		user_id = sys.argv[1].strip()
		match = any(user['id'] == user_id for user in data.get("sessions", []))

		if match:
			body = {
				"status": 'login',
				}
		else:
			body = {
				"status": "fail",
				}
	body = json.dumps(body)
	print(f"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: {len(body)}\r\n\r\n{body}")

except FileNotFoundError:
	body = json.dumps({
		"status": "fail",
		})
	print(f"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: {len(body)}\r\n\r\n{body}")
