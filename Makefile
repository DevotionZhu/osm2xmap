CC          = g++
CFLAGS      = -Wall
LDFLAGS     = -lproj
LINKER      = $(CC) -o
EXECUTABLE  = test

SRCDIR      = .
OBJDIR      = obj
BINDIR      = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
LIBS     := libroxml.a
RM       = rm -f

all: $(BINDIR)/$(EXECUTABLE)

$(BINDIR)/$(EXECUTABLE): $(OBJECTS)
	$(LINKER) $@ $(OBJECTS) $(LIBS) $(LDFLAGS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONEY: clean
clean:
	$(RM) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONEY: remove
	remove: clean
	$(RM) $(BINDIR)/$(EXECUTABLE)
	@echo "Executable removed!"
