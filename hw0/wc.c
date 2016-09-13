#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#ifndef WORD_IN
#define WORD_IN 1
#endif
#ifndef WORD_OUT
#define WORD_OUT 0
#endif

#ifndef LEN_LIMT
#define LEN_LIMT 1024 // every time read 1K content
#endif
static void
wordCount(char* buf, int len, int *wc, int *nlc, char isFirst){
	static int wordCount = 0, newlineCnt = 0;
	static char stage = WORD_OUT;
	if (isFirst)
	{
		wordCount = 0; newlineCnt = 0;
		stage = WORD_OUT;
	}
	int i;
	for (i = 0; i < len; ++i)
	{
		if (isalpha(buf[i]))
		{
			if(stage == WORD_OUT)
			{
				wordCount ++;
				stage = WORD_IN;
			}
		}
		// else if (buf[i] == '-')
		// {
		// 	// if stage is word_in nothing to do else is the same
		// }
		else if(buf[i] != '-'){
			stage = WORD_OUT;
		}
		if (buf[i] == '\n')
		{
			newlineCnt ++;
		}
	}
	*wc = wordCount;*nlc = newlineCnt;
}

int main(int argc, char const *argv[])
{
	FILE* pFile = fopen(argv[1], "rb" );
	int fileSize;
	char buf[LEN_LIMT];

	if (pFile == NULL)
	{
		fprintf(stderr, "no this file\n");
		exit(1);
	}

	// obtain file size
	fseek(pFile, 0, SEEK_END);
	fileSize = ftell(pFile);
	rewind(pFile);

	int wc, nlc, flen;
	char isFirst = 1;

	flen = fread(buf, 1 ,LEN_LIMT, pFile);
	if (flen > 0)
	{
		wordCount(buf, flen, &wc, &nlc, isFirst);
		isFirst = 0;
	}

	while(!feof(pFile)){
		flen = fread(buf, 1 ,LEN_LIMT, pFile);
		wordCount(buf, flen, &wc, &nlc, isFirst);
	}
	printf("%d %d %d\n", nlc, wc, fileSize);
	return 0;
}
