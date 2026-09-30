# Variables del compilador
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Configuración de modo (Release vs Debug)
ifeq "$(MODE)" "release"
CXXFLAGS += -O3
else
CXXFLAGS += -g3
endif

# Archivos y dependencias
CCFILES := $(wildcard *.cc)
HHFILES := $(wildcard *.hh)
OBJS := $(patsubst %.cc,%.o,$(CCFILES))

# Nombres de salida (Modificado para este proyecto)
EXEC = matching_engine.exe

# Regla principal para compilar el ejecutable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)

# Regla general para los archivos objeto
$(OBJS): $(HHFILES)

# Limpiar archivos generados (Compatible con Windows)
clean:
	-del /f /q $(EXEC) $(OBJS) 2>nul

.PHONY: clean