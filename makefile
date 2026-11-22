.PHONY: all clean DR DC DX

all: DR DC DX

DR:
	$(MAKE) -C DR

DC:
	$(MAKE) -C DC

DX:
	$(MAKE) -C DX

clean:
	$(MAKE) -C DR clean
	$(MAKE) -C DC clean
	$(MAKE) -C DX clean