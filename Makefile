.PHONY: build debug release test clean fmt configure

# Default: configure + debug build
all: configure-debug build-debug

configure-debug:
	cmake --preset vcpkg-debug

configure-release:
	cmake --preset vcpkg-release

configure: configure-debug

build-debug:
	cmake --build cmake-build-debug

build-release:
	cmake --build cmake-build-release

build: build-debug

test: build-debug
	cd cmake-build-debug && ctest --output-on-failure

run: build-debug
	./cmake-build-debug/phase

fmt:
	find src include test -name '*.cpp' -o -name '*.hpp' -o -name '*.h' | xargs clang-format -i

fmt-check:
	find src include test -name '*.cpp' -o -name '*.hpp' -o -name '*.h' | xargs clang-format --dry-run -Werror

clean:
	rm -rf cmake-build-debug cmake-build-release
