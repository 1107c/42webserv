#include "../inc/Location.hpp"

// Location name을 입력하고 있는데 이 부분이 필요할까 ? 라고 판단, Map의 Key값과 Value값이 같음 불필요하다생각
// 만들어놨어서 지우진않았음
void Location::setPath(const std::string& path) {
	std::string temp;
	size_t i = 0;

	while (i < path.length() && isspace(path[i])) {
		i++;
	}
	if (path[i] == '\0' || path[i] == '{')
		throw std::invalid_argument("invalid"); // < throw expected
	while (i < path.length() && !isspace(path[i])) {
		temp += path[i++];
	}
	if (!temp.empty()) {
		_path = temp;
	} else
		throw std::invalid_argument("invalid");
}

std::string Location::getPath() const{
	return _path;
}