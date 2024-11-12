#!/bin/bash

# 헬프 메시지 출력 함수
show_help() {
    echo "사용법: ft_make [옵션]"
    echo
    echo "옵션:"
    echo "  -g                컴파일된 객체 파일에 디버깅 심볼을 활성화합니다."
    echo "  --help            이 도움말 메시지를 표시하고 종료합니다."
    echo
    echo "이 스크립트는 C++ 프로젝트를 위한 Makefile을 생성합니다."
    echo
    echo "다음 정보를 입력하라는 메시지가 표시됩니다:"
    echo "  NAME              - 출력 실행 파일의 이름."
    echo "  SRC_DIR           - 소스 (.cpp) 파일이 있는 디렉토리."
    echo "  INC_DIR           - 헤더 (.hpp) 파일이 있는 디렉토리."
    echo
    echo "이 스크립트를 ~/.zshrc 파일에 alias로 추가하려면 다음 명령어를 사용하세요:"
    echo -e "  \033[1;33mecho 'alias ft_make=\"/path/to/ft_make\"' >> ~/.zshrc\033[0m"
    echo "그런 다음, 다음 명령어로 쉘 구성을 다시 로드하세요:"
    echo -e "  \033[1;33msource ~/.zshrc\033[0m"
    echo "여기서 /path/to/ft_make를 이 스크립트의 실제 경로로 바꿔주세요."
    exit 0
}

# --help 옵션 처리
if [ "$1" == "--help" ]; then
    show_help
fi

# 기본 CFLAGS 설정
CFLAGS="-Wall -Wextra -Werror -std=c++98"

# -g 옵션이 있는지 확인
if [ "$1" == "-g" ]; then
    CFLAGS="$CFLAGS -g"
fi

# 사용자 입력 받기
echo -n "NAME = "
read name

echo -n "SRC_DIR = "
read src_dir

echo -n "INC_DIR = "
read inc_dir

# SRC_DIR과 INC_DIR에서 파일 목록 가져오기
src_files=$(ls $src_dir/*.cpp 2> /dev/null | xargs -n 1 basename | tr '\n' ' ')

# Makefile 내용 작성
makefile_content="BLUE := \033[0;36m
RESET := \033[0m

RED_START := 255
GREEN_START := 0
RED_END := 0
GREEN_END := 255

PROGRESS = 0
COUNT = \$(words \$(SRCS))
DONE = 0
BAR_FILLED = 0
BAR_EMPTY = 60

C++ := c++
CFLAGS := $CFLAGS
RM := rm -f

NAME := $name

SRC_DIR := $src_dir
INC_DIR := $inc_dir"

makefile_content="$makefile_content
SRCS := $src_files
OBJS := \$(SRCS:.cpp=.o)

.PHONY : all start clean fclean re

all : \$(NAME)

\$(NAME) :
	@echo
	@make start --no-print-directory"
makefile_content="$makefile_content
	@\$(C++) \$(CFLAGS) \$(OBJS) "
makefile_content="$makefile_content-iquote \$(INC_DIR) -o \$(NAME)
	@echo \"  Completed! \"

start : \$(OBJS)

%.o : \$(SRC_DIR)/%.cpp
	@\$(C++) \$(CFLAGS) -c \$< -o \$@
	@\$(eval DONE := \$(shell echo \$\$((\$(DONE) + 1))))
	@\$(eval PROGRESS := \$(shell echo \$\$((\$(DONE) * 100 / \$(COUNT)))))
	@RED=\$\$((\$(RED_START) + \$(PROGRESS) * (\$(RED_END) - \$(RED_START)) / 100)); \\
	GREEN=\$\$((\$(GREEN_START) + \$(PROGRESS) * (\$(GREEN_END) - \$(GREEN_START)) / 100)); \\
	COLOR=\"\033[38;2;\$\$RED;\$\$GREEN;0m\"; \\
	printf \"\033[2A\033[K  Currently working on: \$(BLUE)\$<\$(RESET)\n\"; \\
	printf \"\$\$COLOR\033[K    %3d\$(RESET)%%│\$\$COLOR\" \$(PROGRESS); \\
	BAR_FILLED=\$\$((\$(PROGRESS) / 5 * 3)); \\
	BAR_EMPTY=\$\$((60 - \$\$BAR_FILLED)); \\
	for i in \$\$(seq 1 \$\$BAR_FILLED); do \\
		printf \"█\"; \\
	done; \\
	for i in \$\$(seq 1 \$\$BAR_EMPTY); do \\
		printf \" \"; \\
	done; \\
	printf \"\$(RESET)│\n\"

clean :"
makefile_content="$makefile_content
	@\$(RM) \$(OBJS)
	@echo \"  All object files are deleted.\""

makefile_content="$makefile_content

fclean :"
makefile_content="$makefile_content
	@\$(RM) \$(OBJS)
	@\$(RM) \$(NAME)
	@echo \"  All object files and \$(BLUE)\$(NAME)\$(RESET) are deleted.\""

makefile_content="$makefile_content

re :
	@make fclean --no-print-directory
	@echo
	@make all --no-print-directory
"

# Makefile 생성
echo "$makefile_content" > Makefile
echo "Makefile이 생성되었습니다."

