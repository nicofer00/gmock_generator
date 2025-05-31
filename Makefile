################################################
# Makefile guide for implementing at the project
# level. 
################################################
################################################
# Generic Variables
################################################
CMAKE := cmake
MAKE := make
LCOV := lcov

BUILD_DIR := build

ROOT_MAKEFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOT_DIR := $(dir $(ROOT_MAKEFILE))

ENV_NAME := .venv
SHELL_NAME := $(shell echo $$0)
################################################
# OS Specific Variables
################################################

ifeq ($(OS),Windows_NT)
	SYSTEM = Building on a Windows System
	GEN_NAME = "Unix Makefiles"
	MKDIR := mkdir
	EXE := .exe
	COPY := copy
ifeq ($(findstring sh,$(SHELL_NAME)),sh)
	SCRIPT := sh
	RM := rm -fR
	DELIM := /
else
	DELIM := \\
	SCRIPT := bat
	RM := rd /s /q
endif

else
	UNAME := $(shell uname -s)
	SYSTEM = Building on a Linux System
	GEN_NAME = "Unix Makefiles"
	MKDIR := mkdir
	RM := rm -fR
	DELIM := /
	EXE := 
	COPY := cp
	SCRIPT := sh
endif

# If no test case filter is defined, use wildcard filter to run all of them.
CASES ?= *
GCOVR = gcovr
GCOVR_FLAGS = --exclude-unreachable-branches --exclude-throw-branches -r .$(DELIM)$(BUILD_DIR) -f .*\.cpp -e ./test/.* -e .*\.h --html-details
GCOVR_OUTPUT_DIR = .$(DELIM)$(BUILD_DIR)$(DELIM)gcov
PROJECT_NAME ?= gmock_generator
export PROJECT_NAME GCOVR_OUTPUT_DIR GCOVR_FLAGS GCOVR DELIM
################################################
# Build Commands
################################################
define make_test
	@$(CMAKE) -S . -B $(BUILD_DIR) -G $(GEN_NAME) -DTEST:BOOL=1
	@$(CMAKE) --build $(BUILD_DIR)
endef

define run_test
	@.$(DELIM)$(BUILD_DIR)$(DELIM)$(PROJECT_NAME)$(EXE) --gtest_output="xml:$(BUILD_DIR)$(DELIM)$(PROJECT_NAME).xml" --gtest_filter=$(CASES)
endef

define coverage
	@@$(VENV)/python -m $(GCOVR) $(GCOVR_FLAGS)
endef

all:$(BUILD_DIR)
	$(call make_test)
	$(call run_test)
	$(call coverage)

test:
	$(call run_test)

report:
	$(call coverage)

mock:
	@python3 .$(DELIM)test$(DELIM)scripts$(DELIM)gmock_gen.py $(FILE) .$(DELIM)test$(DELIM)mocks

simple-test: $(BUILD_DIR) venv
	cmake -S . -B build
	cmake --build build
	cd build && ctest

lint:
	black scripts/

lint-check:
	black --check --verbose -- scripts/

clean:
	@$(RM) $(BUILD_DIR)
	@$(RM) $(VENVDIR)

$(BUILD_DIR):
	@echo $(SYSTEM)
ifeq ("$(wildcard $(BUILD_DIR))","")
	@$(MKDIR) $(BUILD_DIR)
	@$(MKDIR) $(BUILD_DIR)$(DELIM)gcov
	@$(MKDIR) $(BUILD_DIR)$(DELIM)mocks
else
	@$(RM) $(BUILD_DIR)
	@$(MKDIR) $(BUILD_DIR)
	@$(MKDIR) $(BUILD_DIR)$(DELIM)gcov
	@$(MKDIR) $(BUILD_DIR)$(DELIM)mocks
endif

include Makefile.venv