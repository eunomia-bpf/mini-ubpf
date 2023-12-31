# BPF compiler
BPF_CC = clang
# BPF C flags
BPF_CFLAGS = -O2 -target bpf -c -g
# BPF source files
BPF_SRCS = $(wildcard *.bpf.c)
# BPF object files
BPF_OBJS = $(BPF_SRCS:.c=.o)

ARCH ?= $(shell uname -m | sed 's/x86_64/x86/' \
			 | sed 's/arm.*/arm/' \
			 | sed 's/aarch64/arm64/' \
			 | sed 's/ppc64le/powerpc/' \
			 | sed 's/mips.*/mips/' \
			 | sed 's/riscv64/riscv/' \
			 | sed 's/loongarch64/loongarch/')
VMLINUX := vmlinux/$(ARCH)/vmlinux.h
INCLUDES := -Iinclude/ -I$(dir $(VMLINUX))

all: $(BPF_OBJS) base.btf

%.bpf.o: %.bpf.c
	$(BPF_CC) $(BPF_CFLAGS) $(INCLUDES) -D__TARGET_ARCH_$(ARCH) $< -o $@

base.btf:
	clang -g -c btf-base.c -o btf-base.bpf.o
	pahole  --btf_encode_detached base.btf btf-base.bpf.o

clean:
	rm -f $(BPF_OBJS) base.btf
