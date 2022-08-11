#include "main.h"

int main(int argc,char** argv)
{
	int i, am_files_created;
	char files[MAX][MAX];
	char* file_name;
	FILE* fp = NULL;

	/*going over the files, for each file creating an output file after macros replacment*/
	am_files_created = macro_handler(argc, argv, files); 

	/*going over the files after the macro-spreding, for each file creating a machine language output file*/
	for(i = 0;i < am_files_created;i++) 
	{
		fp = fopen(files[i], "r");
		file_name = strtok(files[i],"."); /*getting the file num without the .am*/
		printf("\nFile: %s\n",file_name);
		file_assembler(fp,files[i]); 
		fclose(fp);
	}	

	return 0;
}
