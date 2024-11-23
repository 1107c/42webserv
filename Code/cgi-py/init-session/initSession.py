import sys
import json
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_dir)
file_path = '../../User/data.json'

try:
	with open(file_path, 'r+') as file:
		data = json.load(file)
		data["sessions"] = []

		body = {
			"status": 'success',
			}
		file.seek(0)
		file.truncate()

		# 수정된 데이터 덮어쓰기
		json.dump(data, file, indent=4)
		file.flush()  #
	body = json.dumps(body)
	print(f"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: {len(body)}\r\n\r\n{body}")

except FileNotFoundError:
	body = json.dumps({
		"status": "fail",
		})
	print(f"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: {len(body)}\r\n\r\n{body}")
