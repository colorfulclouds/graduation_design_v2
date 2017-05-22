#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <regex.h>



int main(void)
{
	
	int status, i;
	int err;
char errbuf[1024];
	
	regmatch_t pmatch[1];
	const size_t nmatch = 1;
	regex_t reg;
	const char * pattern = "^(?=.*\\d)(?=.*[a-z])(?=.*[A-Z]).{6,19}$";
	char * buf = "1228794ASm";
	
	regcomp(&reg , pattern , REG_EXTENDED);
	regerror(err,&reg,errbuf,sizeof(errbuf));
 	printf("err:%s\n",errbuf);
	err= regexec(&reg , buf , nmatch , pmatch , 0);

	regerror(err,&reg,errbuf,sizeof(errbuf));
 	printf("err:%s\n",errbuf);
	
	printf("l:%d\n", strlen("1228794458@qq.com"));
	printf("d:%d\n", pmatch[0].rm_eo-pmatch[0].rm_so);
	
	regfree(&reg);
	return 0;
}