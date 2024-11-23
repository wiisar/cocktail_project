TARGETS=cocktail
all: $(TARGETS)
clean:
	$(RM) *.o $(TARGETS)
rendu: all
	rendu
tests: $(addprefix test_, $(TARGETS))
test_%: %
	@echo ==== $< ====
	@./$<
	@echo
.PHONY: all clean tests $(addprefix test-, $(TARGETS))
cocktail: cocktail.c
	@echo Compiling: cocktail.c
	@make -sC exam/cocktail -f ../lib/Makefile ../../cocktail
