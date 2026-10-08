ARCH ?=

all:
	make -C arch/$(ARCH)

run:
	make -C arch/$(ARCH) run

clean:
	make -C arch/$(ARCH) clean