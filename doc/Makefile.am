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
