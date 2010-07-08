# A very stupid makefile xD

texfiles = $(shell find . -name '*\.tex')
auxfiles = $(shell find . -name '*\.aux')
tocfiles = $(shell find . -name '*\.toc')
logfiles = $(shell find . -name '*\.log')

# Not stop on errors.
texflags = -interaction=nonstopmode

# Uncomment to shut the fu***** pdflatex command.
#quiet = yes

ifdef quiet
texflags += > /dev/null
endif 


.PHONY = all clean veryclean

all : Informe.pdf

Informe.pdf : $(texfiles)
	pdflatex $(texflags) Informe.tex 
	pdflatex $(texflags) Informe.tex 
	
clean :
	@rm -vf $(auxfiles) $(tocfiles) $(logfiles) 

veryclean : clean
	@rm -vf Informe.pdf
