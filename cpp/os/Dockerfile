FROM debian:stable-slim

RUN apt update
RUN apt install -y g++ binutils  make
RUN apt install -y libc6-dev-i386 
RUN apt install -y xorriso

RUN mkdir /home/phisyx
COPY linker.ld /home/phisyx/linker.ld
COPY Makefile /home/phisyx/Makefile
COPY src/* /home/phisyx/src/
WORKDIR /home/phisyx

CMD ["bash"]
