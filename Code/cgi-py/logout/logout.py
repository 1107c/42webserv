import sys
import json
import os
from datetime import datetime

script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_dir)
file_path = '../../User/data.json'

try:
	with open(file_path, 'r+') as file:
		data = json.load(file)

		user_id = sys.argv[1].strip()
		match = -1
		cookie = "\r\n"

		for idx in range(len(data["sessions"])):
			id = data["sessions"][idx].get("session_id", "")
			if id == user_id:
				match = idx
				break
		if match != -1:
			del(data["sessions"][match])
			body = {
				"status": "success",
				}
			file.seek(0)
			file.truncate()
			json.dump(data, file, indent=4)
			file.flush()
			cookie = f"Set-Cookie: session_id={user_id}; Expires={datetime(1970, 1, 1)}; Path=/; HttpOnly; Secure; SameSite=Strict\r\n\r\n"
		else:
			body = {
				"status": "fail",
				}
	body = json.dumps(body)
	print(f"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: {len(body)}\r\n{cookie}{body}")

except FileNotFoundError:
	body = json.dumps({
		"status": "fail",
		})
	print(f"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: {len(body)}\r\n\r\n{body}")
