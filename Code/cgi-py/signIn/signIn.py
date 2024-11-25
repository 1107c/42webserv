import sys
import json
import os
import hashlib
import secrets
from datetime import datetime, timedelta, timezone
from email.utils import format_datetime

def get_cookie_expire_time():
	# 현재 시간
	now = datetime.now(timezone.utc)

	# 현재 시간에 1시간 추가
	expires_time = now + timedelta(hours=1)

	# 쿠키용 RFC 1123 형식으로 변환
	cookie_expires = format_datetime(expires_time)
	return cookie_expires

def generate_session_id(user_id):
    random_string = secrets.token_hex(16)
    session_id = f"{user_id}_{random_string}"
    return session_id

def hash_pwd(password):
	return hashlib.sha256(password.encode()).hexdigest()

script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_dir)
file_path = '../../User/data.json'
path = 200

try:
	with open(file_path, 'r+') as file:
		data = json.load(file)

		user_id = sys.argv[2].strip()
		user_pwd = sys.argv[3].strip()
		match = any(user['id'] == user_id and user['password'] == hash_pwd(user_pwd + user['salt'].strip())
				 for user in data.get("users", []))
		cookie = "\r\n"

		if match:
			session_id = generate_session_id(user_id)
			match_session = any(user['id'] == user_id for user in data.get("sessions", []))
			if match_session:
				body = {
					"status": f'{user_id} is already in session'
				}
			else:
				path = 302
				expire = get_cookie_expire_time()
				cookie = f"Set-Cookie: session_id={session_id}; Expires={expire}; Path=/; HttpOnly; Secure; SameSite=Strict\r\n\r\n"
				body = {
					"status": "success",
					"id": user_id,
				}
				new_user = {
					"id": user_id,
					"session_id": session_id,
					"expire_time": expire
				}
				data["sessions"].append(new_user)
				file.seek(0)
				json.dump(data, file, indent=4)
		else:
			body = {"status": "fail"}
	body = json.dumps(body)
	print(f"HTTP/1.1 {path} OK\r\nContent-Type: text/plain\r\nContent-Length: {len(body)}\r\n{cookie}{body}")

except FileNotFoundError:
	body = json.dumps({"status": "fail"})
	print(f"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: {len(body)}\r\n\r\n{body}")
