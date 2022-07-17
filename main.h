#include "handler.h"
#include "finals.h"

/*functions declartions*/
int file_assembler(FILE * fptr, char * file_name);
int stage_two(FILE * fptr, int * ICF, int * DCF, struct symbol *symbols, int * symbol_index, char mc[MACHINE_CODES][MACHINE_CODE_ROW], int keeper[MAX_keeper]);

/*The function that gets the assembly text after macro-spreading and translate it to machine language*/
int file_assembler(FILE * fptr, char * file_name)
{
	int ICF = START_OF_ICF, DCF = 0, symbols_index = 0; /*Level 1*/
	int size = MAX_SYMBOLS;
	char machine_codes[MACHINE_CODES][MACHINE_CODE_ROW]; /*the array that will hold the instructions decoded in binary*/
	char data_codes[MACHINE_CODES][MACHINE_CODE_ROW]; /*the arrau that will hold the data decoded in binary*/
	int keeper[MAX_keeper];
	struct symbol* symbols;
	char mission[MAX][MAX];
	char data[MAX];
	int cur, words, i, j, L, temp, temp_len, label, number_of_arguments, flag = 0, counter = 0;

	memset(keeper, 0, MAX_keeper * sizeof(int)); /*initializing the keeper array with zeros*/

	symbols = (struct symbol*) malloc(size * sizeof(struct symbol)); /*creating space in the memory for all the symbols structs*/

	machine_code_reset(machine_codes); /*initializing the machine code array to start with only zeros*/
	machine_code_reset(data_codes); /*initializing the data code array to start with only zeros*/	

	while (fgets(data, MAX, fptr) != NULL) /*Level 2, going through every line of the input file*/
	{
		/*if the memory sapce for new symbols is about to run out, we increas the symbols memory size*/		
		if(symbols_index < size && symbols_index > (size - 5))
		{
            symbols = symbols_increase(symbols, &size);
        }

		for (i = 0; i < MAX; i++) /*initializing all the chars in mission to '\0'*/
		{
            memset(mission[i], '\0', MAX);
        }

		counter++; /*the line number in the original input file*/

		if(data[0] == '\0' || data[0] == '\n' || data[0] == 13) /*if this is an empty line we continue to the next line*/
		{
			continue;
		}

		i = 0; 
		cur = 0;
		words = 0;
		L = 0;
		temp_len = 0;
		label = 0;

		clear_path(data, &i); /*bringing i to the first non white space character*/

		if (data[i] == ';') /*if the first char is ; its the file name so we can skip this line*/
		{
			continue;
		}

		scan_row(data, mission, &words); /*going over the current line and enter the words into the mission array, each word in a seperate row*/


		/*debugging*/	
		printf("\n \n");
		j = 0;
		while(mission[j][0] != '\0')
		{
			printf("%s	",mission[j]);
			j++;
		}

		temp_len = strlen(mission[cur]);

		/*Level 3*/
		if(temp_len > 0 && mission[cur][temp_len - 1] == ':') /*if the first word of this line is a label*/
		{
			label = 1; /*Level 4, activating the label flag*/
			cur++;

			if(cur == words)
			{
				/*what to do? end of line to soon*/
			}

		}

		/*Level 5*/
		if(!strcmp(mission[cur], ".data") || !strcmp(mission[cur], ".string") || !strcmp(mission[cur], ".struct")) /*if the current word is a data instruction*/ 
		{
			if(label) /*Level 6, if the first word was a label*/
			{
				temp = edit_or_create_symbol(symbols, &symbols_index, mission[0], DCF, 0, 1, 0, 0, 0); /*creating the label as a data symbol*/ 
				/*if there was a problem with the symbol the function returns 0, and we raise a flag*/
				if(!temp)
				{
					flag = 1;
				}
			}

			L = words - (cur + 1); /*calculating how many words there are after the .string/.data/.struct*/

			data_handler(mission, & words, mission[cur], & ICF, & DCF, & flag, L, data_codes); /*Level 7, entering the data to the data table and updating the DC*/

            continue;

		}

		/*Level 8*/
		if(!strcmp(mission[cur], ".extern") ||
!strcmp(mission[cur], ".entry"))
		{
			/*Level 9*/

		    if (!strcmp(mission[cur], ".extern")) 
			{

		        cur = cur + 1;

		        if (cur == words) 
				{

		            /*what to do? end of file to soon*/

		        }

				while(mission[cur][0] != '\0')
				{            
					temp = edit_or_create_symbol(symbols, &symbols_index, mission[cur], 0, 0, 0, 0, 1, 0); /*saving the symbols as externals*/
					cur++;

		        	if (!temp) 
					{
		            	/*if exist without ext  error*/
		            	flag = 1;
		        	}

				}

		        continue; /*Level 10*/

		    }

		continue;

		}


		/*Level 11, this is an instruction row*/
        if (label) /*if there is a lebel we enter it as code label*/
		{

            temp = edit_or_create_symbol(symbols, & symbols_index, mission[0], ICF, 1, 0, 0, 0, 0);

            if (!temp) 
			{
                flag = 1;
            }

        }

		/*Level 12-13*/
		if (words > 0) 
		{
			/*calculating if we need  3 or 2 or 1 code lines to decode this action*/
			/*if the action word is invalid the function returns -1 */
            number_of_arguments = job_secure(mission[cur]); 
			
            if (number_of_arguments == -1) 
			{
                flag = 1;
                continue;
            }

            if (words - label - 1 != number_of_arguments) 
			{
                flag = 1;
                printf("the number of arguments: %d, are not the right. the action: %s needs: %d arguments",(words - label - 1), mission[cur], number_of_arguments);

                continue;
            }

            L = handle_machine_code_stage_one(mission, cur, &ICF,  machine_codes); /*decode the first word of the instuction and returning L (the number of machine code line this command will take)*/

			ICF = ICF + L; /*Level 14*/	

            continue;

        }
	}

	 /*Level 16*/
    if (flag) 
	{
        printf("\n there was a syntax error, moving to the next input file. \n");
        return flag;
    }

	/*Level 17, going over the symbols table and adding the IC to the data symbols*/
	for(j = 0;j < symbols_index;j++)
	{
		if(symbols[j]._isdata == 1)
		{
			symbols[j]._value = symbols[j]._value + ICF;
		}
	}


	/*debugging*/
	printf("\n \n \n Instruction code: \n");
	for(j = 0;j < ICF;j++)
	{
		printf("%d	(%.10s) \n", j,machine_codes[j]);
	}

	printf("Data code: \n");
	for(j = 0;j < DCF;j++)
	{
		printf("%d	(%.10s) \n",ICF + j,data_codes[j]);
	}

	printf("\nSymbols: \n");
	for(j = 0;j < symbols_index;j++)
	{
		printf("%d %s \n",symbols[j]._value,symbols[j]._name);
	}


	/*stage two*/

	return 0;
	
}
