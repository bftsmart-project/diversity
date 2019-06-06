FOLDERS = zero list2 list kv array
.PHONY: clean zero list2 list kv array

all: $(FOLDERS)

$(FOLDERS):
	cd $@ && $(MAKE) MAKEFLAGS=

clean:
	cd zero; $(MAKE) clean MAKEFLAGS=
	cd list2; $(MAKE) clean MAKEFLAGS=
	cd list; $(MAKE) clean MAKEFLAGS=
	cd kv; $(MAKE) clean MAKEFLAGS=
	cd array; $(MAKE) clean MAKEFLAGS=
