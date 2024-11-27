import sys
import json
import os
from datetime import datetime, timedelta, timezone

script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_dir)
file_path = '../../User/data.json'

def is_expired(time):
	now = datetime.now(timezone.utc)
	expire_time = datetime.strptime(time, "%a, %d %b %Y %H:%M:%S %z")
	return True if now >= expire_time else False

try:
	with open(file_path, 'r+') as file:
		data = json.load(file)

		session_id = sys.argv[1].strip()
		idx, match = -1, False
		login_id = ""

		for i, item in enumerate(data.get("sessions", [])):
			if item['session_id'] == session_id:
				if is_expired(item['expire_time']):
					del(data["sessions"][i])
					file.seek(0)
					file.truncate()
					json.dump(data, file, indent=4)
					file.flush()
				else:
					login_id = item['id']
					match = True
				break
		if match:
			body = {
				"status": 'login',
				"id": login_id,
				"path": sys.argv[2],
				}
		else:
			body = {
				"status": "fail",
				}
	body = json.dumps(body)
	print(f"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: {len(body)}\r\n\r\n{body}")

except FileNotFoundError:
	body = json.dumps({
		"status": "error",
		})
	print(f"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: {len(body)}\r\n\r\n{body}")
