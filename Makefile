all:
		gcc PDFGen/pdfgen.c main.c parse_tree.c recursive_parser.c tree_ops.c -o a -lm
