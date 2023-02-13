# @authors   pintoved (Pinto Veda)
#            cgreenbe (Classie Greenbeard)

#------------------------------------------------#
#   INGREDIENTS                                  #
#------------------------------------------------#
# NAME           project name
# VERSION        build version
#
# OS             operating system
# FILES          all .cc .h files
#
# INSTALL_DIR    installation path
# FILES          executable file name
#
# cmake          build system executable

NAME         := AlgorithmicTrading
VERSION      := 1.0

OS           := $(shell uname -s)
FILES        := $(shell find . -regex '.*\(cc\|h\)' -not -path "*shared*")

BUILD_DIR = ../build
INSTALL_DIR = ../install

#------------------------------------------------#
#   RECIPES                                      #
#------------------------------------------------#
# all        default goal
# install
# uninstall
# clean      remove binary
# dvi
# dist
# build      build program
# linter     run code style check
# linter-fix run code style fix
# cppcheck   run static code analys

all: run

run: build
ifeq ($(OS), Darwin)
		open $(BUILD_DIR)/$(NAME).app
else
		$(BUILD_DIR)/$(NAME)
endif

install: build
	mkdir -p $(OBJ_DIR)
	cp -r build/$(OBJ_NAME) $(OBJ_DIR)

uninstall:
	rm -rfv $(OBJ_DIR)

dvi:
	latex docs.tex docs.dvi

dist: build 
	tar -czvf $(NAME)-$(VERSION).tar.gz . docs.dvi Makefile					

build:
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

clean:
	rm -rfv $(BUILD_DIR)* $(INSTALL_DIR) logs/ \
	*.tar.gz *.aux *.log *.dvi *.out *.toc  *.user

linter:
	clang-format -n -style=Google $(FILES)
	
linter-fix:
	clang-format -i -style=Google $(FILES)

cppcheck:
	cppcheck --language=c++ --std=c++17 \
	--enable=all \
	--suppress=missingInclude \
	--suppress=unusedFunction \
	$(FILES)

#------------------------------------------------#
#   SPEC                                         #
#------------------------------------------------#

.PHONY: install uninstall clean dvi dist build linter cppcheck
.SILENT:
