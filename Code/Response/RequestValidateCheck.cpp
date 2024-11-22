#include "../Socket/Epoll.hpp"
#include "../UtilityHeader/Utility.hpp"

std::string Response::getErrorHeader(int error) {
	std::string header;
	switch (error) {
		case 403:
			header = "HTTP/1.1 403 Forbidden";
			break ;
		case 404:
			header = "HTTP/1.1 404 Not Found";
			break ;
		case 405:
			header = "HTTP/1.1 405 Method Not Allowed";
			break;
		case 413:
			header = "HTTP/1.1 413 Payload Too Large";
			break;
		case 500:
			header = "HTTP/1.1 500 Internal Server Error";
			break ;
		case 505:
			header = "HTTP/1.1 505 HTTP Version Not Supported";
			break ;
		default:
			header = "HTTP/1.1 403 Forbidden";
	}
	return header;
}

std::string Response::getErrorPath(int error) {
	std::string path;
	switch (error) {
		case 403:
			path = "ErrorHtml/403.html";
			break ;
		case 404:
			path = "ErrorHtml/404.html";
			break ;
		case 405:
			path = "ErrorHtml/405.html";
			break ;
		case 413:
			path = "ErrorHtml/413.html";
			break ;
		case 500:
			path = "ErrorHtml/500.html";
			break ;
		case 505:
			path = "ErrorHtml/505.html";
			break ;
		default:
			path = "ErrorHtml/404.html";
	}
	return path;
}

int Response::getValidate(Request& request) {
	std::string path = request.getMappingUrl();
	int error = 0;
	Location loc = request.getLocation();
	const std::vector<std::string>& index = request.getLocation().getIndex();
	for (size_t i = 0; i < index.size(); ++i) {
		std::string temp =  path + index[i];
		int errorTemp = checkPermissions(temp.c_str());
		if (!errorTemp) {
			request.setMappingUrl(temp);
			return 0;
		} else if (!error && errorTemp) error = errorTemp;
	}
	if (error) return error;
	if (!loc.getAutoindex()) return 403;
	return 0;
}

int Response::validateRequest(Request& request) {
	std::string path = request.getMappingUrl();
	std::cout << "######################3\n" << path << std::endl;
	std::string version = request.getVersion();
	if (version != "HTTP/1.1" && version != "HTTP/1.0") return 505;
	int error = checkPermissions(path.c_str());
	if (error) return error;
	if (request.getMethod() == "GET" && isDirectory(path.c_str())) {
		error = getValidate(request);
		if (error) return error;
	} else if (request.getMethod() == "POST") {
		error = getValidate(request);
		if (error) return error;
	} else if (request.getMethod() == "DELETE") {

	}
	return 0;
}