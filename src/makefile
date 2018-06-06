CXX = g++
CXXFLAGS = -std=c++0x -Wall -pedantic-errors -g

SRCS =  main.cpp DenseMatrix.cpp ElementList.cpp ListMatrix.cpp Simulate.cpp SparseMatrix.cpp
OBJS = ${SRCS:.cpp=.o}
HEADERS = DenseMatrix.h Element.h ElementList.h ListMatrix.h Matrix.h Simulate.h SparseMatrix.h

MAIN = matrix_power

all: ${MAIN}

${MAIN}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${MAIN}

.cpp.o:
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
	${RM} ${PROGS} ${OBJS} *.o *~.
