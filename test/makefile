include ../definitions.mk

# Compiler flags
COMPILER=g++
COMPILER_FLAGS=-c -Wall --pedantic -std=c++14 -Iinclude -I../$(ENGINE_INCLUDE)  -I../$(GLM_INCLUDE) -I../$(TINYOBJLOADER_INCLUDE) -O3 -DNDEBUG -DBOOST_TEST_DYN_LINK
LINKER=g++
LINKER_FLAGS=-s -lboost_unit_test_framework

# Test sources
SOURCES=$(wildcard src/*.cpp)

# Release build settings
OBJECTS=$(subst src/,obj/,$(subst .cpp,.o,$(SOURCES)))
LIBRARIES=lib/$(ENGINE_LIBRARY)

# Test build rules
test: build always
	@./bin/$(TEST_EXECUTABLE)
	@echo "Tests finished"

build: bin/$(TEST_EXECUTABLE)

bin/$(TEST_EXECUTABLE): $(OBJECTS) $(LIBRARIES)
	@mkdir -p bin
	$(LINKER) $(OBJECTS) $(LIBRARIES) -o $@ $(LINKER_FLAGS)
	@echo "Test build finished"

-include $(subst obj/,deps/,$(subst .o,.d,$(RELEASE_OBJECTS)))

obj/%.o: src/%.cpp
	@mkdir -p obj deps
	$(COMPILER) $< -o $@ $(COMPILER_FLAGS)
	@$(COMPILER) $< -MM $(COMPILER_FLAGS) > deps/$*.d.tmp
	@sed "s/.*:/obj\/$*.o:/" < deps/$*.d.tmp > deps/$*.d
	@rm -f deps/$*.d.tmp

clean:
	@rm -rf bin obj deps
	@echo "Clean finished"

always:
