CC = gcc
CFLAGS = -Wall -Iinclude -Llib -g
LIBS = -lallegro_monolith-debug

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/**/*.c) $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

all: $(BINDIR)/app

$(BINDIR)/app: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@if not exist $(subst /,\,$(dir $@)) md $(subst /,\,$(dir $@))
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/app
