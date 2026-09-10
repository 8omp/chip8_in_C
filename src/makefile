# 出来上がる実行ファイルの名前
TARGET = chip8

# コンパイラとオプション
CC = gcc
CFLAGS = -Wall -I ../include/

# コンパイルする対象
SRCS = test_sdl.c chip8.c

# コマンド
all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) -lSDL2

clean:
	rm -f $(TARGET)