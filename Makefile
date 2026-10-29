ARCH ?= amd64

.PHONY: all deps run iso debug clean

all: deps
	make -C arch/$(ARCH)

deps:
	make -C deps/limine limine

run:
	make -C arch/$(ARCH) run

iso:
	make -C arch/$(ARCH) iso

debug:
	make -C arch/$(ARCH) debug

clean:
	make -C arch/$(ARCH) clean