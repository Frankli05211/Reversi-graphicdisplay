CXX = g++
CXXFLAGS = -std=c++14 -g -Wall -MMD -Werror=vla
OBJECTS = main.o cell.o textdisplay.o grid.o window.o graphicsdisplay.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = reversi

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

clean :
	rm ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS} # reads the .d files and reruns dependencies
