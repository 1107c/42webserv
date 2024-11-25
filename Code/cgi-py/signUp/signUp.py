#!/usr/bin/python3

import sys
import json
import secrets
import hashlib
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_dir)
file_path = '../../User/data.json'
# 랜덤한 salt 값 생성
salt = secrets.token_hex(16)
password = sys.argv[3] + salt
hashed_password = hashlib.sha256(password.encode()).hexdigest()
try:
	with open(file_path, 'r') as file:
		data = json.load(file)

except FileNotFoundError:
	data = {"users": [], "sessions": []}

# 새로운 데이터 추가
new_user = {
	"id": sys.argv[2],
	"salt": salt,
	"password": hashed_password
}

data["users"].append(new_user)

with open(file_path, 'w') as file:
	json.dump(data, file, indent=4)

body = "success"
print(f"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: {len(body)}\r\n\r\n{body}\r\n\r\n")