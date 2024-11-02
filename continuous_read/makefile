$(warning $(CURDIR) $(abspath  $(MAKEFILE_LIST)/..))

SKETCH?=$(notdir $(CURDIR))

doc :
	pandoc README.md -o README.pdf
	evince README.pdf


cbuild :
	idf.py build
	idf.py flash
	idf.py monitor
