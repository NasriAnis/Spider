ARCH ?=

all:
	make -C arch/$(ARCH)

run:
	make -C arch/$(ARCH) run

debug:
	make -C arch/$(ARCH) debug

clean:
	make -C arch/$(ARCH) clean