CC := g++
CFLAGS := -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

SOURCES := source/tree.cpp source/main.cpp source/log.cpp source/data.cpp
OBJ := build/tree.o build/main.o build/log.o build/data.o

BUILD := build

INCLUDES := include
OUTPUT := akinator

$(OUTPUT): $(OBJ)
	@mkdir -p log
	@mkdir -p $(BUILD)
	$(CC) $(OBJ) -o $@

build/tree.o:
	@mkdir -p $(BUILD)
	@$(CC) $(CFLAGS) -I $(INCLUDES) -c source/tree.cpp -o $@

build/main.o:
	@mkdir -p $(BUILD)
	@$(CC) $(CFLAGS) -I $(INCLUDES) -c source/main.cpp -o $@

build/log.o:
	@mkdir -p $(BUILD)
	@$(CC) $(CFLAGS) -I $(INCLUDES) -c source/log.cpp -o $@

build/data.o:
	@mkdir -p $(BUILD)
	@$(CC) $(CFLAGS) -I $(INCLUDES) -c source/data.cpp -o $@

clean:
	rm build/*.o $(OUTPUT)
	rm -R log
