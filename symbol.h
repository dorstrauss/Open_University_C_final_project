#include "finals.h"

struct symbol{
    char _name[SYMBOL_LENGTH];
    int _value;
    int _isCode;
    int _isdata;
    int _isIns;
    int _isExt;
    int _isEnt;
};


/*a function that gets the symbol details and create or edit it in the symbols table*/
int edit_or_create_symbol(struct symbol *symbols, int* symbols_index, char name[], int value, int code, int data, int ins, int ext, int ent)
{
	int i;
	char sName[MAX];

	/*coping the symbol name to sName*/
    if(name[strlen(name) -1] == ':')
    {
        strcpy(sName, name);
        sName[strlen(sName) -1] = '\0';
    } 
	else
	{
        strcpy(sName, name);
    }

	
	for(i = 0;i < *symbols_index;i++) /* checking if the symbol exist*/
	{
        if(!strcmp(symbols[i]._name,sName)) /*if the symbol exist*/
		{
            if(symbols[i]._isEnt)
			{
                return 1;
            }
            if(!(symbols[i]._isEnt) && ent)
			{
                symbols[i]._isEnt=ent;
                return 1;
            }
            else
			{
                if(ext && !symbols[i]._isExt)
				{
                    printf("\nthere is a symbol with this name without the external symbol\n");
                    return 0;
                }
                printf("\nthere is a symbol with this name\n");
                return 0;
            }
        }
    }

	/*creatin or editing the symbol in the symbols table*/
	strcpy(symbols[*symbols_index]._name, sName);
    symbols[*symbols_index]._value=value;
    symbols[*symbols_index]._isCode=code;
    symbols[*symbols_index]._isdata=data;
    symbols[*symbols_index]._isIns=ins;
    symbols[*symbols_index]._isExt=ext;
    symbols[*symbols_index]._isEnt=ent;
    *symbols_index = *symbols_index + 1;
    return 1;
		
}
