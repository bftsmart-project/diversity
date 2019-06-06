.PHONY: clean list2 kv zero_plain array
FOLDERS = list2 kv zero_plain array

all: $(FOLDERS)

$(FOLDERS):
	cd $@ && $(MAKE) MAKEFLAGS=

clean:	
	cd list2; $(MAKE) clean MAKEFLAGS=
	cd kv; $(MAKE) clean MAKEFLAGS=
	cd zero_plain; $(MAKE) clean MAKEFLAGS=
	cd array; $(MAKE) clean MAKEFLAGS=
