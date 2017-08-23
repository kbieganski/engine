include ../definitions.mk

# Compiler flags
COMPILER=g++
COMPILER_FLAGS=-c -Wall --pedantic -std=c++14 -Iinclude -I../$(ENGINE_INCLUDE)
LINKER=g++
LINKER_FLAGS=-lglfw

# Project sources
SOURCES=$(wildcard src/*.cpp)

# Release build settings
RELEASE_OBJECTS=$(subst src/,obj/release/,$(subst .cpp,.o,$(SOURCES)))
RELEASE_COMPILER_FLAGS=-O3 -DNDEBUG
RELEASE_LINKER_FLAGS=-s
RELEASE_LIBRARIES=lib/release/$(ENGINE_LIBRARY)

# Debug build settings
DEBUG_OBJECTS=$(subst src/,obj/debug/,$(subst .cpp,.o,$(SOURCES)))
DEBUG_COMPILER_FLAGS=-g -O0 -DDEBUG
DEBUG_LINKER_FLAGS=
DEBUG_LIBRARIES=lib/debug/$(ENGINE_LIBRARY)

# Release build rules
release: bin/release/$(EXAMPLE_EXECUTABLE)

bin/release/$(EXAMPLE_EXECUTABLE): $(RELEASE_OBJECTS) $(RELEASE_LIBRARIES)
	@mkdir -p bin/release
	$(LINKER) $(RELEASE_OBJECTS) $(RELEASE_LIBRARIES) -o $@ $(LINKER_FLAGS) $(RELEASE_LINKER_FLAGS)
	@echo "Example release build finished"

-include $(subst obj/,deps/,$(subst .o,.d,$(RELEASE_OBJECTS)))

obj/release/%.o: src/%.cpp
	@mkdir -p obj/release deps/release
	$(COMPILER) $< -o $@ $(COMPILER_FLAGS) $(RELEASE_COMPILER_FLAGS)
	@$(COMPILER) $< -MM $(COMPILER_FLAGS) $(RELEASE_COMPILER_FLAGS) > deps/release/$*.d.tmp
	@sed "s/.*:/obj\/release\/$*.o:/" < deps/release/$*.d.tmp > deps/release/$*.d
	@rm -f deps/release/$*.d.tmp

# Debug build rules
debug: bin/debug/$(EXAMPLE_EXECUTABLE)

bin/debug/$(EXAMPLE_EXECUTABLE): $(DEBUG_OBJECTS) $(DEBUG_LIBRARIES)
	@mkdir -p bin/debug
	$(LINKER) $(DEBUG_OBJECTS) $(DEBUG_LIBRARIES) -o $@ $(LINKER_FLAGS) $(DEBUG_LINKER_FLAGS)
	@echo "Example debug build finished"

-include $(subst obj/,deps/,$(subst .o,.d,$(DEBUG_OBJECTS)))

obj/debug/%.o: src/%.cpp
	@mkdir -p obj/debug deps/debug
	$(COMPILER) $< -o $@ $(COMPILER_FLAGS) $(DEBUG_COMPILER_FLAGS)
	$(COMPILER) $< -MM $(COMPILER_FLAGS) $(DEBUG_COMPILER_FLAGS) > deps/debug/$*.d.tmp
	@sed "s/.*:/obj\/debug\/$*.o:/" < deps/debug/$*.d.tmp > deps/debug/$*.d
	@rm -f deps/debug/$*.d.tmp

clean:
	@rm -rf bin obj deps
	@echo "Clean finished"
