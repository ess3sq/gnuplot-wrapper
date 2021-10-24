# Generate object file for static linking:
mkdir -p bin
g++ -Wall -Wextra -Werror -c plt.cc -o bin/gnuplot-wrapper.o
