# Variables
SourcesDirectory = ./Sources
ObjectsDirectory = ./Objects
TemplatesDirectory = ./Templates

Sources = $(wildcard $(SourcesDirectory)/*.cpp)
Objects = $(patsubst $(SourcesDirectory)/%.cpp, $(ObjectsDirectory)/%.o, $(Sources))

# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Default Target
all: $(ObjectsDirectory) cpp-to-tpp main

# Rule to create the build directory
$(ObjectsDirectory):
	@mkdir -p $(ObjectsDirectory)

# Rule to compile .cpp files into .o files
$(ObjectsDirectory)/%.o: $(SourcesDirectory)/%.cpp
	@echo "Compiling $< -> $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Main compilation rule
main: $(Objects)
	@echo "Linking to create executable: main"
	@$(CXX) $(CXXFLAGS) $(Objects) -o $@

# Rule to switch .tpp to .cpp
tpp-to-cpp:
	@echo "Converting .tpp to .cpp files..."
	@for file in $(wildcard $(TemplatesDirectory)/*.tpp); do \
		dest="$${file%.tpp}.cpp"; \
		mv "$$file" "$$dest"; \
		echo "Moved $$file -> $$dest"; \
	done

# Rule to switch .cpp to .tpp
cpp-to-tpp:
	@echo "Converting .cpp to .tpp files..."
	@for file in $(wildcard $(TemplatesDirectory)/*.cpp); do \
		dest="$${file%.cpp}.tpp"; \
		mv "$$file" "$$dest"; \
		echo "Moved $$file -> $$dest"; \
	done

# Clean up
clean:
	@echo "Cleaning up build files..."
	@make tpp-to-cpp
	@rm -rf $(ObjectsDirectory) main

