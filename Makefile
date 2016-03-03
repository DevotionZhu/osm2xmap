#
#    Copyright 2016 Semyon Yakimov
#
#    This file is part of Osm2xmap.
#
#    Osm2xmap is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Osm2xmap is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Osm2xmap.  If not, see <http://www.gnu.org/licenses/>.

CC          = g++
CFLAGS      = -Wall -std=c++11
LDFLAGS     = -lproj -lroxml
LINKER      = $(CC) -o
EXECUTABLE  = osm2xmap

SRCDIR      = .
OBJDIR      = obj
BINDIR      = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
RM       = rm -f

all: $(BINDIR)/$(EXECUTABLE)

$(BINDIR)/$(EXECUTABLE): $(OBJECTS)
	@$(LINKER) $@ $(OBJECTS) $(LDFLAGS)
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

