MARKDOWN = $(wildcard *.md)
PDFS= $(MARKDOWN:.md=.pdf)

all: $(PDFS)

clean:
	rm -rf $(PDFS)

%.pdf: %.md
	pandoc --from markdown --to latex -o $@ $<
