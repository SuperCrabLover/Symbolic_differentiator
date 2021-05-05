#include "recursive_parser.h"
#include "tree_ops.h"
#include <getopt.h>
#include "PDFGen/pdfgen.h"

#define INPUT_SIZE 2000
#define FILE_NAME_SIZE 100
#define ANSWER_SIZE 99999


void make_pdf(char* orig, char* simp, char* diff_simp) {
    struct pdf_info info = {
        .creator = "Molozhavenko Alexander",
        .producer = "Molozhavenko Alexander",
        .title = "Let's wipe Stephen Wolfram's nose!",
        .author = "Molozhavenko Alexander",
        .subject = "Acronis",
        .date = "Today"
    };
    struct pdf_doc *pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, &info);
    pdf_set_font(pdf, "Times-Roman");
    pdf_append_page(pdf);
	pdf_add_text(pdf, NULL, "Let's wipe Stephen Wolfram's nose!", 12, 210, 750, PDF_BLACK);
    pdf_add_text(pdf, NULL, orig, 12, 50, 720, PDF_BLACK);
	pdf_add_text(pdf, NULL, simp, 12, 50, 690, PDF_BLACK);
	if (diff_simp[0] != '\0')
    {
		pdf_add_text(pdf, NULL, diff_simp, 12, 50, 660, PDF_BLACK);
	}
    pdf_save(pdf, "output.pdf");
    pdf_destroy(pdf);
}

int read_input(FILE* input_file, char* input)
{
	char new_let = getc(input_file);
	int i;
	for(i = 0; new_let != EOF; i++)
	{
		if (i == INPUT_SIZE - 3)
		{
			fprintf(stderr, "The input expression is too long!\n");
			return 0;
		}
		else
		{
			input[i] = new_let;
			new_let = getc(input_file);
		}
	}
	input[i] = '$';
	input[i+1] = '\0';
	return 1;
}

int main(int argc, char* argv[])
{
	char input_name[FILE_NAME_SIZE];
	FILE* input_file;
	char input[INPUT_SIZE];
	char c;
	char* simp;
	char* orig;
	char* copied;
	char* difed;
	char pdf_orig[ANSWER_SIZE];
	char pdf_simp[ANSWER_SIZE];
	char pdf_diff_simp[ANSWER_SIZE];
	pdf_orig[0] = '\0';
	pdf_simp[0] = '\0';
	pdf_diff_simp[0] = '\0';
	struct node* root;
	struct node* copy;
	 while ((c = getopt (argc, argv, "hds:")) != -1)
    {
        switch (c)
        {
            case 'h':
                printf(" -s is for simplifying the input suntax: -s <filename> \n -d is for simplifying differentiation and again simplifying the input -d <filename>\n");
                return 0;
            case 's':
				input_name[0] = '\0';
				strcat(input_name, argv[2]);
				input_file = fopen(input_name, "r");
				if (NULL == input_file)
				{
					fprintf(stderr, "Cannot open the file with name: %s\n", input_name);
					exit(-1);
				}
				if (read_input(input_file, input) == 0)
				{
					fclose(input_file);
					return 0;
				}
				fclose(input_file);

				root = get_g(input);
				orig = to_string(root->right_child);

				strcat(pdf_orig, " original: \0");
				strcat(pdf_orig, orig);

				printf("original: %s\n", orig);
				free(orig);
					
				tree_simplify(root);
				simp = to_string(root->right_child);

				strcat(pdf_simp, " simplified: \0");
				strcat(pdf_simp, simp);

				printf("simplified: %s\n", simp);

				make_pdf(pdf_orig, pdf_simp, pdf_diff_simp);

				free(simp);
				tree_delete(root);
                return 0;
            case 'd':
				input_name[0] = '\0';
				strcat(input_name, argv[2]);
				input_file = fopen(input_name, "r");
				if (NULL == input_file)
				{
					fprintf(stderr, "Cannot open the file with name: %s\n", input_name);
					exit(-1);
				}
				if (read_input(input_file, input) == 0)
				{
					fclose(input_file);
					return 0;
				}
				fclose(input_file);
				
				root = get_g(input);
				orig = to_string(root->right_child);
								
				strcat(pdf_orig, " original: \0");
				strcat(pdf_orig, orig);

				printf("original: %s\n", orig);
				free(orig);
					
				tree_simplify(root);
				simp = to_string(root->right_child);

				strcat(pdf_simp, " simplified: \0");
				strcat(pdf_simp, simp);

				printf("simplified: %s\n", simp);
				free(simp);

				copy = tree_copy(root);
				copied = to_string(copy);
				printf("copied: %s\n", copied);
				free(copied);

				copy = tree_diff(copy);
				difed = to_string(copy);

				printf("diffed: %s\n", difed);
				free(difed);
	
				tree_simplify(copy);
				char* diff_simp = to_string(copy->right_child);
				
				strcat(pdf_diff_simp, " diffed and simplified:  \0");
				strcat(pdf_diff_simp, diff_simp);

				printf("diffed and simplified: %s\n", diff_simp);
				free(diff_simp);
				
				make_pdf(pdf_orig, pdf_simp, pdf_diff_simp);
				
				tree_delete(copy);
				tree_delete(root);
				return 0;
			case '?':
                fprintf(stderr, "Error: unknown parameter!\n");
                exit(-1);
         }
    }
	return 0;
}
//5*(x+10*(2*x-sin(cos(x)*log(x+5))))+(x-x)$
//5*(x+10*(2*x-sin(cos(x)*log(x+5))))+(x-x)*((log(cos(log(x)+4*x))))$ перед log лишняя скобка 