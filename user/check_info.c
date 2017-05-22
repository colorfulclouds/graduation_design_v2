#ifndef CHECK_INFO
#define CHECK_INFO
#include "check_info.h"
#endif

int check_result(void * info , int flag)
{
	int result;
	
	char *pattern_email = "^[a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\\.[a-zA-Z0-9_-]+)+$";	//email regex format
																					//other regex format in order to step
	char *pattern_password= "^[0-9a-zA-Z_#]{6,19}$";

	regex_t reg;
	int number = 1;
	regmatch_t pmatch[1];
	
	switch(flag)
	{
		case 1:
			// name
			break;
		case 2:
			result = regcomp(&reg , pattern_email , REG_EXTENDED);
			break;
		case 3:
			result = regcomp(&reg , pattern_password , REG_EXTENDED);
			break;
			
		default:
			//impossible here
			break;
	}
	
//char buf[100];	
	if( result != 0)
	{
		printf("[22001]FATAL ERROR!\n");
//regerror(result , &reg , buf , sizeof buf);
//printf("[]%s\n", buf);
		exit(EXIT_FAILURE);
	}
	
	result = regexec(&reg , (char *)info , number ,pmatch , 0);
	if(result == REG_NOMATCH)
	{
		regfree(&reg);
		return 0;
	}	
	if(result)
	{
		printf("[22002]FATAL ERROR!\n");
		exit(EXIT_FAILURE);
	}
	regfree(&reg);
	return 1;
}