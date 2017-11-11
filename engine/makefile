include ../definitions.mk

# Compiler flags
COMPILER=g++
COMPILER_FLAGS=-c -Wall --pedantic -std=c++14 -Iinclude
ARCHIVER=ar
ARCHIVER_FLAGS=rvs

# Project sources
SOURCES=$(wildcard src/*.cpp)

# Release build settings
RELEASE_OBJECTS=$(subst src/,obj/release/,$(subst .cpp,.o,$(SOURCES)))
RELEASE_COMPILER_FLAGS= -O3 -DNDEBUG

# Debug build settings
DEBUG_OBJECTS=$(subst src/,obj/debug/,$(subst .cpp,.o,$(SOURCES)))
DEBUG_COMPILER_FLAGS=-g -O0 -DDEBUG

# Shader compiler flags
SHADER_COMPILER=glslangValidator
SHADER_COMPILER_FLAGS=-V

# Shader sources and targets
FRAGMENT_SHADER_SOURCES=$(wildcard shaders/*.frag)
VERTEX_SHADER_SOURCES=$(wildcard shaders/*.vert)
FRAGMENT_SHADERS=$(subst shaders/,work/shaders/,$(subst .frag,_frag.spv,$(FRAGMENT_SHADER_SOURCES)))
VERTEX_SHADERS=$(subst shaders/,work/shaders/,$(subst .vert,_vert.spv,$(VERTEX_SHADER_SOURCES)))

# Release build rules
release: lib/release/$(ENGINE_LIBRARY) shaders

lib/release/$(ENGINE_LIBRARY): $(RELEASE_OBJECTS)
	@mkdir -p lib/release
	$(ARCHIVER) $(ARCHIVER_FLAGS) $@ $(RELEASE_OBJECTS)
	@echo "Engine release build finished"

-include $(subst obj/,deps/,$(subst .o,.d,$(RELEASE_OBJECTS)))

obj/release/%.o: src/%.cpp
	@mkdir -p obj/release deps/release
	$(COMPILER) $< -o $@ $(COMPILER_FLAGS) $(RELEASE_COMPILER_FLAGS)
	@$(COMPILER) $< -MM $(COMPILER_FLAGS) $(RELEASE_COMPILER_FLAGS) > deps/release/$*.d.tmp
	@sed "s/.*:/obj\/release\/$*.o:/" < deps/release/$*.d.tmp > deps/release/$*.d
	@rm -f deps/release/$*.d.tmp

# Debug build rules
debug: lib/debug/$(ENGINE_LIBRARY) shaders

lib/debug/$(ENGINE_LIBRARY): $(DEBUG_OBJECTS)
	@mkdir -p lib/debug
	$(ARCHIVER) $(ARCHIVER_FLAGS) $@ $(DEBUG_OBJECTS)
	@echo "Engine debug build finished"

-include $(subst obj/,deps/,$(subst .o,.d,$(DEBUG_OBJECTS)))

obj/debug/%.o: src/%.cpp
	@mkdir -p obj/debug deps/debug
	$(COMPILER) $< -o $@ $(COMPILER_FLAGS) $(DEBUG_COMPILER_FLAGS)
	$(COMPILER) $< -MM $(COMPILER_FLAGS) $(DEBUG_COMPILER_FLAGS) > deps/debug/$*.d.tmp
	@sed "s/.*:/obj\/debug\/$*.o:/" < deps/debug/$*.d.tmp > deps/debug/$*.d
	@rm -f deps/debug/$*.d.tmp

shaders: $(VERTEX_SHADERS) $(FRAGMENT_SHADERS)
	@echo "Shader compilation for engine finished"

work/shaders/%_frag.spv: shaders/%.frag shaders/*.glsl
	@mkdir -p work/shaders
	$(SHADER_COMPILER) $(SHADER_COMPILER_FLAGS) $< -o $@

work/shaders/%_vert.spv: shaders/%.vert shaders/*.glsl
	@mkdir -p work/shaders
	$(SHADER_COMPILER) $(SHADER_COMPILER_FLAGS) $< -o $@

clean:
	@rm -rf lib obj deps work
	@echo "Clean finished"
