ACLOCAL_AMFLAGS = -I m4

if DOXYGEN

doc:
	@(doxygen)

clean-local:
	@rm -rf html/

.PHONY: doc

else

doc:
	@(echo "Please install doxygen to get actual documentation")
endif

datadir= $(prefix)/share/@PACKAGE_NAME@/doc
dist_data_DATA = img/timer.png
