CC = gcc
CFLAGS = -Os -ffunction-sections -fdata-sections -Wall -I./include -s
LDFLAGS = -L./lib -Wl,-Bstatic -lraylib -Wl,-Bdynamic -lGL -lm -lpthread -ldl -lrt -lX11 -Wl,--gc-sections

BUILD_DIR = ./build
EXECUTABLE = $(BUILD_DIR)/main
RESOURCE_FOLDER = ./Resources

# all: $(EXECUTABLE)
# 	$(EXECUTABLE)

$(EXECUTABLE): ./src/main.c
	@mkdir -p $(BUILD_DIR)
	@cp -r $(RESOURCE_FOLDER) $(BUILD_DIR)/
	$(CC) $(CFLAGS) ./src/main.c -o $(EXECUTABLE) $(LDFLAGS)
	@echo "Build complete. Running application..."
	$(EXECUTABLE)

strip:
	strip $(EXECUTABLE)

play:
	$(EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR)
