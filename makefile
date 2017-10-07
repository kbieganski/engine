include definitions.mk

r: release
rel: release
release: engine-release example-release test-build

d: debug
deb: debug
debug: engine-debug example-debug

engine: engine-release
engine-release:
	@cd engine; make release

engine-debug:
	@cd engine; make debug

er: example-release-run
err: example-release-run
example-run: example-release-run
example-release-run: example-release
	@cd example/work; ../bin/release/example

edr: example-debug-run
example-debug-run: example-debug
	@cd example/work; ../bin/debug/example

example: example-release
example-release: engine-release
	@mkdir -p example/lib/release
	@cp engine/lib/release/$(ENGINE_LIBRARY) example/lib/release
	@cd example; make release
	@cp -rf engine/work example

example-debug: engine-debug
	@mkdir -p example/lib/debug
	@cp engine/lib/debug/$(ENGINE_LIBRARY) example/lib/debug
	@cd example; make debug
	@cp -rf engine/work example

t: test
test: test-build
	@cd test; make test

test-build: engine-release
	@mkdir -p test/lib
	@cp engine/lib/release/$(ENGINE_LIBRARY) test/lib
	@cd test; make build

c: clean
clean:
	@cd engine; make clean
	@cd example; make clean
	@rm -rf example/lib
	@cd test; make clean
	@rm -rf test/lib
