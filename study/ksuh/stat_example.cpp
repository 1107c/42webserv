#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <ctime>


// stat은 파일 혹은 디렉터리 정보를 가져오는 데 사용됩니다.

int main() {
	const char* filepath = "a";
	struct stat fileStat;

	// stat 함수를 통해 파일 상태 가져오기
	if (stat(filepath, &fileStat) < 0) { // Failure = -1, Success = 0
		std::cerr << "Error: Could not retrieve file information." << std::endl;
		return 1;
	}

	// 파일 / 디렉터리 정보 확인
	if (S_ISREG(fileStat.st_mode)) {
		std::cout << filepath << " is a regular file." << std::endl;
	} else if (S_ISDIR(fileStat.st_mode)) {
		std::cout << filepath << " is a directory." << std::endl;
	} else {
		std::cout << filepath << " is of another type." << std::endl;
	}

	// 파일 정보 출력
	std::cout << "File: " << filepath << std::endl;
	std::cout << "Size: " << fileStat.st_size << " bytes" << std::endl;
	std::cout << "File Permissions: ";
	std::cout << ((fileStat.st_mode & S_IRUSR) ? "r" : "-");
	std::cout << ((fileStat.st_mode & S_IWUSR) ? "w" : "-");
	std::cout << ((fileStat.st_mode & S_IXUSR) ? "x" : "-");
	std::cout << ((fileStat.st_mode & S_IRGRP) ? "r" : "-");
	std::cout << ((fileStat.st_mode & S_IWGRP) ? "w" : "-");
	std::cout << ((fileStat.st_mode & S_IXGRP) ? "x" : "-");
	std::cout << ((fileStat.st_mode & S_IROTH) ? "r" : "-");
	std::cout << ((fileStat.st_mode & S_IWOTH) ? "w" : "-");
	std::cout << ((fileStat.st_mode & S_IXOTH) ? "x" : "-") << std::endl;
	std::cout << "Last Access: " << ctime(&fileStat.st_atime);
	std::cout << "Last Modification: " << ctime(&fileStat.st_mtime);
	std::cout << "Last Status Change: " << ctime(&fileStat.st_ctime);

	return 0;
}
