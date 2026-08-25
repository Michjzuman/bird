bird: src/*.c
	clang src/*.c \
    -I "$(HOME)/.local/ncurses/include" \
    -L "$(HOME)/.local/ncurses/lib" \
	-I include \
	-l ncursesw \
	-o bird