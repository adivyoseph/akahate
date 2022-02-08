#include <stdlib.h>
#include <stdio.h>

#include <libfyaml.h>

//gcc -L/usr/local/lib -lfyaml  main.c -o test

int main(int argc, char *argv[])
{

    static const char *yaml = 
		"invoice: 34843\n"
		"date   : !!str 2001-01-23\n"
		"bill-to: &id001\n"
		"    given  : Chris\n"
		"    family : Dumars\n"
		"    address:\n"
		"        lines: |\n"
		"            458 Walkman Dr.\n"
		"            Suite #292\n";
	struct fy_document *fyd = NULL;
	int rc, count, ret = EXIT_FAILURE;
	unsigned int invoice_nr;
	char given[256 + 1];

	printf("%s\n", fy_library_version());

    if (argc == 1)
		fyd = fy_document_build_from_string(NULL, yaml, FY_NT);
	else
		fyd = fy_document_build_from_file(NULL, argv[1]);
	if (!fyd) {
		fprintf(stderr, "failed to build document");
		goto fail;
	}

 	/* get the invoice number and the given name */
	count = fy_document_scanf(fyd,
			"/invoice %u "
			"/bill-to/given %256s",
			&invoice_nr, given);
	if (count != 2) {
		fprintf(stderr, "Failed to retreive the two items\n");
		goto fail;
	}

	/* print them as comments in the emitted YAML */
	printf("# invoice number was %u\n", invoice_nr);
	printf("# given name is %s\n", given);


	return EXIT_SUCCESS;
fail:
	fy_document_destroy(fyd);	/* NULL is OK */

	return ret;
}
