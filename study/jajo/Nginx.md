# 1. 클러스터 컴퓨터 실행 방법
```shell
docker run -p 8080:80 -d nginx
docker exec -it 21 bash

cd etc/nginx
```

# 2. nginx default.conf
```nginx
server {
    listen       80;
    listen  [::]:80;
    server_name  localhost;

    #access_log  /var/log/nginx/host.access.log  main;

    location / {
        root   /usr/share/nginx/html;
        index  index.html index.htm;
    }

    #error_page  404              /404.html;

    # redirect server error pages to the static page /50x.html
    #
    error_page   500 502 503 504  /50x.html;
    location = /50x.html {
        root   /usr/share/nginx/html;
    }

    # proxy the PHP scripts to Apache listening on 127.0.0.1:80
    #
    #location ~ \.php$ {
    #    proxy_pass   http://127.0.0.1;
    #}

    # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
    #
    #location ~ \.php$ {
    #    root           html;
    #    fastcgi_pass   127.0.0.1:9000;
    #    fastcgi_index  index.php;
    #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
    #    include        fastcgi_params;
    #}

    # deny access to .htaccess files, if Apache's document root
    # concurs with nginx's one
    #
    #location ~ /\.ht {
    #    deny  all;
    #}
}

```

## 2-1. nginx default.conf 파일 설명
### 1. `server { ... }`

이 부분은 Nginx 서버 블록(server block)을 정의하는 부분입니다. 각 서버 블록은 Nginx가 요청을 어떻게 처리할지 정의하는 영역입니다.

### 2. `listen 80;`

`listen [::]:80;`

- **`listen 80;`**: Nginx가 IPv4 환경에서 포트 80(HTTP 기본 포트)으로 들어오는 요청을 수신하도록 지정합니다.
- **`listen [::]:80;`**: Nginx가 IPv6 환경에서도 포트 80으로 들어오는 요청을 수신하도록 설정합니다. `[::]`는 IPv6의 "모든 주소"를 의미합니다.

### 3. `server_name localhost;`

- **`server_name localhost;`**: 이 서버 블록에 해당하는 요청을 처리할 도메인 이름을 지정합니다. 이 설정에서는 `localhost`로만 처리할 수 있는 요청을 정의합니다. 즉, `http://localhost`로 접근하는 요청을 이 서버 블록에서 처리하게 됩니다.

### 4. `#access_log /var/log/nginx/host.access.log main;`

- **`#`**: 이 줄은 주석처리된 상태입니다. 주석은 실행되지 않으며, 이 줄의 의미는 Nginx의 요청 로그를 `/var/log/nginx/host.access.log` 파일에 기록하고, 로그 형식을 `main`으로 지정한다는 것입니다. 주석을 해제하면 로그가 기록됩니다.

### 5. `location / { ... }`

- **`location / { ... }`**: URL 경로가 `/`로 시작하는 요청을 처리하는 `location` 블록입니다. 즉, 루트 경로로 오는 모든 요청에 대해 해당 블록이 실행됩니다.
    
    - **`root /usr/share/nginx/html;`**: 요청이 들어오면, `/usr/share/nginx/html` 디렉터리에서 파일을 찾으라는 설정입니다. 예를 들어, `http://localhost/index.html`에 대한 요청이 들어오면 Nginx는 `/usr/share/nginx/html/index.html` 파일을 제공하려 시도합니다.
        
    - **`index index.html index.htm;`**: `index`는 기본적으로 요청된 디렉터리에 해당하는 인덱스 파일을 지정합니다. 여기서는 `index.html` 또는 `index.htm`을 우선적으로 찾습니다. 만약 사용자가 `http://localhost/`로 접속했다면, Nginx는 `index.html`을 찾아서 응답합니다.
        

### 6. `#error_page 404 /404.html;`

- **`#`**: 주석 처리된 설정입니다. `error_page`는 에러 페이지를 설정하는 옵션인데, 주석을 해제하면 `404` 에러가 발생했을 때 `/404.html` 페이지를 표시하도록 지정할 수 있습니다.

### 7. `error_page 500 502 503 504 /50x.html;`

- **`error_page 500 502 503 504 /50x.html;`**: 서버 오류(500, 502, 503, 504)에 대해 `/50x.html` 페이지를 표시하도록 설정합니다. 예를 들어, 서버 내부 에러가 발생하면 `/50x.html` 페이지를 클라이언트에게 전달합니다.

### 8. `location = /50x.html { ... }`

- **`location = /50x.html { ... }`**: 정확하게 `/50x.html` URL로 오는 요청에 대한 처리를 정의합니다.
    
    - **`root /usr/share/nginx/html;`**: `/50x.html` 파일을 제공할 때 해당 파일을 `/usr/share/nginx/html/50x.html`에서 찾아 제공하도록 설정합니다.

### 9. `# location ~ \.php$ { ... }`

- **`#`**: 주석 처리된 PHP 관련 설정입니다. 이 설정이 주석 해제되면 PHP 파일을 처리하는 설정으로 변경할 수 있습니다. 아래는 PHP 파일을 Apache나 다른 FastCGI 서버로 전달하는 설정입니다.
    
    - **`location ~ \.php$ { ... }`**: URL 경로가 `.php`로 끝나는 요청을 처리하는 블록입니다.
    - **`proxy_pass http://127.0.0.1;`**: PHP 요청을 로컬 서버의 127.0.0.1 IP로 전달하는 설정입니다. 이 경우 Apache 서버가 `127.0.0.1`에서 PHP를 처리하고 응답을 반환하도록 합니다.

### 10. `# location ~ \.php$ { ... }` (FastCGI 방식)

- **`#`**: 또 다른 PHP 파일 처리 방식에 대한 주석입니다. FastCGI를 사용하여 PHP 요청을 처리할 수 있습니다.
    
    - **`root html;`**: FastCGI 서버가 처리할 때 루트 디렉터리를 `html`로 설정합니다.
    - **`fastcgi_pass 127.0.0.1:9000;`**: FastCGI 서버에 PHP 요청을 전달합니다. 여기서는 FastCGI 서버가 로컬의 9000 포트에서 실행되고 있다고 가정합니다.
    - **`fastcgi_index index.php;`**: PHP 요청에 대한 기본 파일을 `index.php`로 설정합니다.
    - **`fastcgi_param SCRIPT_FILENAME /scripts$fastcgi_script_name;`**: `SCRIPT_FILENAME` 변수에 요청된 PHP 파일의 실제 경로를 지정합니다.
    - **`include fastcgi_params;`**: FastCGI 관련 설정들을 포함시킵니다.

### 11. `# location ~ /\.ht { ... }`

- **`#`**: `.htaccess` 파일에 대한 접근을 차단하는 설정입니다. `.htaccess` 파일은 보통 Apache 서버에서 사용되며, 보안상 Nginx가 이 파일을 처리할 필요는 없기 때문에 접근을 차단합니다.
    
    - **`deny all;`**: `.ht`로 시작하는 파일(일반적으로 `.htaccess`)에 대해 모든 접근을 차단합니다.



# 3. nginx conf 헤더
### 1. Nginx 설정 파일의 기본 구조

Nginx 설정 파일은 기본적으로 `nginx.conf`라는 이름의 파일에 위치하며, 일반적으로 `/etc/nginx/nginx.conf` 또는 `/usr/local/nginx/nginx.conf` 경로에 있습니다. 설정 파일은 기본적으로 여러 `http`, `server`, `location` 블록으로 구분되어 있습니다.

### 2. 주요 설정 항목

#### 2.1 `user`
```nginx
user www-data;
```

- **설명**: Nginx 프로세스가 실행될 때 사용하는 사용자와 그룹을 지정합니다. `www-data`는 일반적으로 웹 서버 프로세스를 위한 표준 사용자 이름입니다.


#### 2.2 `worker_processes`
```nginx
worker_processes auto;
```

- **설명**: Nginx가 실행할 워커 프로세스의 수를 지정합니다. `auto`로 설정하면 시스템의 CPU 코어 수에 맞춰 자동으로 워커 프로세스 수를 조정합니다.


#### 2.3 `pid`
```nginx
pid /var/run/nginx.pid;
```

- **설명**: Nginx 마스터 프로세스의 PID(프로세스 ID)가 저장될 파일의 경로를 지정합니다.


#### 2.4 `events`
```nginx
events {     
	worker_connections 1024; 
}
```

- **설명**: `events` 블록은 Nginx가 이벤트를 처리하는 방법을 정의합니다. `worker_connections`는 각 워커 프로세스가 동시에 처리할 수 있는 최대 연결 수를 설정합니다.


#### 2.5 `http`
```nginx
http {     
	include       mime.types;     
	default_type  application/octet-stream;     
	log_format    main '$remote_addr - $remote_user [$time_local] "$request" '                       '$status $body_bytes_sent "$http_referer" '                       '"$http_user_agent" "$http_x_forwarded_for"';     
	access_log    /var/log/nginx/access.log main;     
	sendfile on;     
	tcp_nopush on;     
	tcp_nodelay on;     
	keepalive_timeout 65;     
	types_hash_max_size 2048; 
}
```

- **설명**: `http` 블록은 HTTP 요청과 응답을 처리하는 데 관련된 설정들을 포함합니다. 이 블록 안에서는 MIME 타입, 로그 설정, 파일 전송, 연결 타임아웃 등의 설정을 정의합니다.
    
- **주요 항목**:
    - **`include mime.types;`**: MIME 타입 정의 파일을 포함하여 다양한 파일 확장자에 맞는 콘텐츠 타입을 자동으로 설정합니다.

    - **`access_log`**: 접속 로그를 저장할 파일과 로그 형식을 지정합니다.

    - **`sendfile on;`**: `sendfile`은 파일을 읽고 클라이언트로 직접 전송하는 방식입니다. `on`으로 설정하면 성능이 향상될 수 있습니다.

    - **`keepalive_timeout`**: 연결을 유지할 최대 시간을 설정합니다.

    - **`types_hash_max_size`**: MIME 타입 해시 테이블의 최대 크기를 설정합니다.

#### 2.6 `server` 블록

`server` 블록은 웹 서버가 처리할 요청을 정의합니다. 각 서버 블록은 특정 도메인이나 IP 주소에 대한 설정을 포함할 수 있습니다.

```nginx
server {     
	listen 80;     
	server_name example.com;     
	root /var/www/example.com;     
	index index.html;  
	        
	location / {         
		try_files $uri $uri/ =404;     
	} 
}
```

- **`listen`**: 해당 서버 블록이 요청을 수신할 포트와 주소를 지정합니다. 예를 들어, `listen 80;`은 HTTP 요청을 받는 기본 포트입니다.
    
- **`server_name`**: 이 서버 블록이 처리할 도메인 이름을 지정합니다. 예를 들어, `example.com`에 대한 요청을 처리합니다.
    
- **`root`**: 해당 서버 블록의 루트 디렉터리를 설정합니다. 예를 들어, `/var/www/example.com`은 이 서버 블록에 대한 웹 콘텐츠가 위치하는 디렉터리입니다.
    
- **`index`**: 디렉터리 요청에 대해 기본으로 제공할 파일을 지정합니다. 예를 들어, `index.html` 또는 `index.php` 등이 있을 수 있습니다.
    

#### 2.7 `location` 블록

`location` 블록은 특정 URL 경로에 대한 요청을 처리하는 방법을 정의합니다. `server` 블록 내에 위치하며, URL 경로에 따라 다양한 방식으로 요청을 처리할 수 있습니다.

```nginx
location /images/ {     
	root /var/www/example.com; 
}  

location = /index.html {     
	root /var/www/example.com;     
	access_log off; 
}
```
- **`location /images/`**: `/images/`로 시작하는 모든 요청을 처리합니다. 이 요청에 대해 `/var/www/example.com/images/` 디렉터리 내의 파일을 제공하게 됩니다.
    
- **`location = /index.html`**: 정확히 `/index.html`로 요청이 왔을 때만 해당 설정을 적용합니다. 이 요청은 `/var/www/example.com/index.html` 파일을 제공하며, `access_log`를 비활성화합니다.
    
- **`try_files`**: 파일이 존재하면 해당 파일을 제공하고, 존재하지 않으면 다른 처리를 하도록 지정할 수 있습니다. 예를 들어, `try_files $uri $uri/ =404;`는 요청된 URI가 실제로 존재하지 않으면 404 에러를 반환합니다.
    

#### 2.8 기타 유용한 설정들

- **`error_page`**: 특정 HTTP 상태 코드에 대해 사용자 정의 에러 페이지를 설정합니다.
    
    ```nginx
    error_page 404 /404.html;
    ```
    
    - **설명**: `404` 오류가 발생하면 `/404.html` 파일을 클라이언트에게 반환합니다.


- **`proxy_pass`**: 다른 서버로 요청을 전달하는 설정입니다. 예를 들어, PHP 요청을 처리할 때 Apache나 FastCGI 서버로 요청을 전달할 수 있습니다.
    
```nginx
location ~ \.php$ {     
	fastcgi_pass 127.0.0.1:9000;     
	fastcgi_param SCRIPT_FILENAM  /var/www/example.com$fastcgi_script_name;     
	include fastcgi_params; 
}
```
    
    - **설명**: `.php`로 끝나는 요청은 `127.0.0.1:9000`에서 실행되는 FastCGI 서버로 전달됩니다.


- **`rewrite`**: URL을 재작성할 때 사용합니다. 예를 들어, 특정 URL 패턴을 다른 URL로 리디렉션할 수 있습니다.
    ```nginx
    rewrite ^/oldpage$ /newpage permanent;
    ```
    
    - **설명**: `/oldpage`로 오는 요청을 `/newpage`로 영구 리디렉션합니다.


- **`deny`/`allow`**: 특정 IP 주소의 접근을 허용하거나 차단하는 설정입니다.
    ```nginx
    location /admin/ {     
	    allow 192.168.1.0/24;     
	    deny all; 
	}```
    
    - **설명**: `/admin/` 경로에 대해 `192.168.1.0/24` 네트워크에서의 접근만 허용하고, 그 외의 접근은 모두 차단합니다.

---
