DEPS=clutter-1.0 mx-1.0
CXXFLAGS=`pkg-config --cflags $(DEPS)` -Wall -Werror -Wfatal-errors -O2
LIBS=`pkg-config --libs $(DEPS)` -rdynamic
OBJS=main.o utils.o

ifeq ($(shell uname -s),Darwin)
  CFLAGS += -DDARWIN
endif

all: run

%.o : %.cpp %.h
	g++ -c $(CXXFLAGS) $< -o $@

run: $(OBJS)
	g++ -Wall -g $(LIBS) $^ -o $@

.PHONY: clean

clean:
	rm -f run *.o

