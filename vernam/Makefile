# このディレクトリ内のファイルをコンパイルするためのMakefile

CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm

# デフォルトのターゲット
all: vernam

# ターゲットの依存関係
vernam: vernam.o

# ファイルのコンパイル
.c:
	$(CC) $(CFLAGS) -c $<


# ターゲットのクリーンアップ
clean:
	rm -f vernam vernam.o
