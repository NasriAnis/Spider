ARCH ?=

all:
	make -C arch/$(ARCH)

run:
	make -C arch/$(ARCH) run

iso:
	make -C arch/$(ARCH) iso

debug:
	make -C arch/$(ARCH) debug

clean:
	make -C arch/$(ARCH) clean