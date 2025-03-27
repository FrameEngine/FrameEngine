BUILD_DIR = build
CMAKE = cmake
CMAKE_BUILD_TYPE ?= Debug

.PHONY: all build clean rebuild build_tests rebuild_tests run_test run_tests help clean_cache run

all: build

clean_cache:
	@rm -f $(BUILD_DIR)/CMakeCache.txt
	@echo "CMake cache cleaned"

configure: clean_cache
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) .. -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)

build: configure
	@cd $(BUILD_DIR) && $(CMAKE) --build . --config $(CMAKE_BUILD_TYPE)
	@echo "Build complete"

build_project: configure
	@cd $(BUILD_DIR) && $(CMAKE) --build . --config $(CMAKE_BUILD_TYPE) --target frame_engine
	@echo "Project build complete"

build_tests: configure
	@cd $(BUILD_DIR) && $(CMAKE) --build . --config $(CMAKE_BUILD_TYPE) --target build_all_tests
	@echo "Tests build complete"

build_single_test:
ifndef TEST_PATH
	@echo "Error: TEST_PATH is not specified"
	@echo "Usage: make build_single_test TEST_PATH=path/to/test/file"
	@exit 1
else
	@echo "Selected build dir ${BUILD_DIR}"
	@cd $(BUILD_DIR) && $(CMAKE) --build . --config $(CMAKE_BUILD_TYPE) --target $(shell basename $(TEST_PATH) .cpp)
	@echo "Test $(shell basename $(TEST_PATH)) built"
endif

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Build directory removed"

rebuild: clean build

rebuild_project: clean build_project

rebuild_tests: clean build_tests

run_test:
ifndef TEST_PATH
	@echo "Error: TEST_PATH is not specified"
	@echo "Usage: make run_test TEST_PATH=path/to/test/file"
	@exit 1
else
	@test_name=$(shell basename $(TEST_PATH) .cpp); \
	cd $(BUILD_DIR); \
	if [ ! -f "./$$test_name" ]; then \
		echo "Test binary not found. Building first..."; \
		$(CMAKE) --build . --config $(CMAKE_BUILD_TYPE) --target $$test_name; \
	fi; \
	./$$test_name
endif

run_tests: build_tests
	@echo "Running all tests..."
	@cd $(BUILD_DIR) && ctest --verbose
	@echo "All tests completed"

run:
	@cd $(BUILD_DIR) && ./frame_engine

help:
	@echo "Usage:"
	@echo "  make                - Build the entire project"
	@echo "  make build_project  - Build only the main project (no tests)"
	@echo "  make build_tests    - Build only all tests"
	@echo "  make build_single_test TEST_PATH=path/to/test.cpp - Build specific test"
	@echo "  make clean          - Remove build directory"
	@echo "  make rebuild        - Clean and rebuild everything"
	@echo "  make rebuild_project - Clean and rebuild only the project"
	@echo "  make rebuild_tests  - Clean and rebuild only tests" 
	@echo "  make run_test TEST_PATH=path/to/test.cpp - Run specific test"
	@echo "  make run_tests      - Run all tests"
	@echo "  make run            - Run project"
	@echo "  make help           - Show this help message"
	@echo "  make clean_cache    - Remove only CMake cache files"
