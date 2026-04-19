CC     = gcc
FLAGS  = -Wall -Wextra
SRCS   = main.c hp.c level.c item.c class.c store.c

# detecta o sistema operacional
ifeq ($(OS), Windows_NT)
	TARGET = rpg.exe
else
	TARGET = rpg
endif

$(TARGET): $(SRCS)
	$(CC) $(FLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f rpg rpg.exe