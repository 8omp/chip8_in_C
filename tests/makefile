# ここを毎度書き換える
TEST_FILE = test_00E0_00EE_1NNN_2NNN.c

# 出来上がる実行ファイルの名前
TARGET = run_test

# コンパイラとオプション
CC = gcc
CFLAGS = -Wall -I../include/chip8.h

# コンパイルする対象
SRCS = $(TEST_FILE) ../src/chip8.c

# コマンド
all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)