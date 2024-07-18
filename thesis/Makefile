NAME=thesis
ABSTRACT=abstract
LATEXMKOPTS=#-pdf #enable for forcing pdflatex
LATEXMK=latexmk -f $(LATEXMKOPTS)

all:
	$(LATEXMK) $(NAME)
	$(LATEXMK) $(ABSTRACT)-cs
	$(LATEXMK) $(ABSTRACT)-en

clean:
	$(LATEXMK) -C

