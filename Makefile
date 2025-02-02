BUILD_DIR := build

all: build

build:
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR)

test: build
	@cd $(BUILD_DIR) && ctest --verbose && cd ..

test_log: build
	@cat $(BUILD_DIR)/Testing/Temporary/LastTest.log

clean:
	@rm -rf $(BUILD_DIR)

rebuild: clean build

run: build
	@$(BUILD_DIR)/frame_engine

