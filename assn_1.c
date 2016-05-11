#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>

int linearsearch(int searchInteger, int *k, int countKeySize)
{
	int temp;
	for(temp=0; temp<countKeySize; temp++)
		if(searchInteger==k[temp])
			return 1;
	return 0;
}

int binarysearch(int searchInteger, int *k, int start, int end)
{	
	if(end>=start)
	{
		int temp = (start+end)/2;
		if(searchInteger==k[temp])
			return 1;
		else if(searchInteger>k[temp])
			return binarysearch(searchInteger, k, temp+1, end);
		else
			return binarysearch(searchInteger, k, start, temp-1);
	}
	else return 0;
}

int binarySearchOnDisk(int searchInteger, FILE *keyFile, int start, int end)
{
if(end>=start)
	{
		int tempKey;
		int mid = (start+end)/2;
		fseek(keyFile, mid*sizeof(int),SEEK_SET);
		fread(&tempKey,sizeof(int),1,keyFile);
		if(searchInteger==tempKey)
			return 1;
		else if(searchInteger>tempKey)
			return binarySearchOnDisk(searchInteger, keyFile, mid+1, end);
		else
			return binarySearchOnDisk(searchInteger, keyFile, start, mid-1);
	}
	else return 0;
}

void inmemseq(char *key, char *seek)
{
	int countKeySize, countSeekSize, temp;
	int *k, *s, *h;
	struct timeval startTime, endTime, execTime;

	//Opening Seek file
	FILE *seekFile;
	seekFile = fopen(seek,"rb");

	//Counting number of seeks
	countSeekSize = 0;
	while(1)
	{
		fread(&temp,sizeof(int),1,seekFile);
		if(feof(seekFile)) break;
		countSeekSize++;
	}

	//Going back to start of file:
	clearerr(seekFile);
	fseek(seekFile,0,SEEK_SET);

	//Creating array S for Seek integers
	s = malloc(countSeekSize * sizeof(int));

	for(temp=0;temp<countSeekSize;temp++)
	{
		fread(&s[temp],sizeof(int),1,seekFile);
	}	

	fclose(seekFile);

	
	//Opening Key file
	FILE *keyFile;

	keyFile = fopen(key,"rb");

	//Counting number of keys
	countKeySize = 0;
	while(1)
	{
		fread(&temp,sizeof(int),1,keyFile);
		if(feof(keyFile)) break;
		countKeySize++;
	}

	//Going back to start of file:
	clearerr(keyFile);
	fseek(keyFile,0,SEEK_SET);

	//Creating array K for keys
	k = malloc(countKeySize * sizeof(int));

gettimeofday(&startTime, NULL);

	for(temp=0;temp<countKeySize;temp++)
	{
		fread(&k[temp],sizeof(int),1,keyFile);
	}	

	fclose(keyFile);

	//Now we have arrays k & s;
	//creating h
	h = calloc(countSeekSize, sizeof(int));

	//linear search
	for (temp=0;temp<countSeekSize;temp++)
	{
		h[temp] = linearsearch(s[temp],k, countKeySize);
	}
	gettimeofday(&endTime, NULL);
	for (temp=0;temp<countSeekSize;temp++)
	{
		if(h[temp]==0)
			printf( "%12d: No\n", s[temp] );
		else
			printf( "%12d: Yes\n", s[temp] );
	}
	
	execTime.tv_sec=endTime.tv_sec - startTime.tv_sec;
	if(endTime.tv_usec < startTime.tv_usec)
	{
		execTime.tv_sec=execTime.tv_sec-1;
		execTime.tv_usec=startTime.tv_usec - endTime.tv_usec;
	}
	else
		execTime.tv_usec=endTime.tv_usec - startTime.tv_usec;

	printf("Time: %ld.%06ld\n", execTime.tv_sec, execTime.tv_usec );

	free(k);
	free(s);
	free(h);

} //end inmemseq


//In Memory Binary
void inmembin(char *key, char *seek)
{
	int countKeySize, countSeekSize, temp;
	int *k, *s, *h;
	struct timeval startTime, endTime, execTime;

	//Opening Seek file
	FILE *seekFile;
	seekFile = fopen(seek,"rb");

	//Counting number of keys
	countSeekSize = 0;
	while(1)
	{
		fread(&temp,sizeof(int),1,seekFile);
		if(feof(seekFile)) break;
		countSeekSize++;
	}

	//Going back to start of file:
	clearerr(seekFile);
	fseek(seekFile,0,SEEK_SET);

	//Creating array S for Seek integers
	s = malloc(countSeekSize * sizeof(int));

	for(temp=0;temp<countSeekSize;temp++)
	{
		fread(&s[temp],sizeof(int),1,seekFile);
	}	

	fclose(seekFile);

	gettimeofday(&startTime, NULL);

	//Opening Key file
	FILE *keyFile;

	keyFile = fopen(key,"rb");

	//Counting number of keys
	countKeySize = 0;
	while(1)
	{
		fread(&temp,sizeof(int),1,keyFile);
		if(feof(keyFile)) break;
		countKeySize++;
	}

	//Going back to start of file:
	clearerr(keyFile);
	fseek(keyFile,0,SEEK_SET);

	//Creating array K for keys
	k = malloc(countKeySize * sizeof(int));

	for(temp=0;temp<countKeySize;temp++)
	{
		fread(&k[temp],sizeof(int),1,keyFile);
	}	

	fclose(keyFile);

	//Now we have arrays k & s;
	//creating h
	h = calloc(countSeekSize, sizeof(int));

	//binary search
	for (temp=0;temp<countSeekSize;temp++)
	{
		h[temp] = binarysearch(s[temp],k, 0, countKeySize-1);
	}
	gettimeofday(&endTime, NULL);
	for (temp=0;temp<countSeekSize;temp++)
	{
		if(h[temp]==0)
			printf( "%12d: No\n", s[temp] );
		else
			printf( "%12d: Yes\n", s[temp] );
	}
	
	execTime.tv_sec=endTime.tv_sec - startTime.tv_sec;
	if(endTime.tv_usec < startTime.tv_usec)
	{
		execTime.tv_sec=execTime.tv_sec-1;
		execTime.tv_usec=startTime.tv_usec - endTime.tv_usec;
	}
	else
		execTime.tv_usec=endTime.tv_usec - startTime.tv_usec;

	printf("Time: %ld.%06ld\n", execTime.tv_sec, execTime.tv_usec );

	free(k);
	free(s);
	free(h);
} //end inmembin







//On Disk Sequential
void ondiskseq(char *key, char *seek)
{
	int countSeekSize, temp, keytemp;
	int *s, *h;
	struct timeval startTime, endTime, execTime;

	//Opening Seek file
	FILE *seekFile;
	seekFile = fopen(seek,"rb");

	//Counting number of seeks
	countSeekSize = 0;
	while(1)
	{
		fread(&temp,sizeof(int),1,seekFile);
		if(feof(seekFile)) break;
		countSeekSize++;
	}

	//Going back to start of file:
	clearerr(seekFile);
	fseek(seekFile,0,SEEK_SET);

	//Creating array S for Seek integers
	s = malloc(countSeekSize * sizeof(int));

	for(temp=0;temp<countSeekSize;temp++)
	{
		fread(&s[temp],sizeof(int),1,seekFile);
	}	

	fclose(seekFile);
	
	//Now we have array s;
	//creating h
	h = calloc(countSeekSize, sizeof(int));


	gettimeofday(&startTime, NULL);

	//Opening Key file
	FILE *keyFile;
	keyFile = fopen(key,"rb");

	clearerr(keyFile);
	fseek(keyFile,0,SEEK_SET);
	
	//linear search
	for (temp=0;temp<countSeekSize;temp++)
	{
	//printf("Searching %d position which is %d\n",temp,s[temp]);
		while(1)
		{
			//printf("In while loop\n");
			fread(&keytemp,sizeof(int),1,keyFile);
			if(feof(keyFile))
			{
			//printf("End of file reached\n\n");
				break;
			}
			if(s[temp]==keytemp)
			{
			//printf("Found %d\n", temp);
				h[temp]=1;
				break;
			}
		}
		clearerr(keyFile);
		fseek(keyFile,0,SEEK_SET);
	}

	gettimeofday(&endTime, NULL);
	
	fclose(keyFile);

	for (temp=0;temp<countSeekSize;temp++)
	{
		if(h[temp]==0)
			printf( "%12d: No\n", s[temp] );
		else
			printf( "%12d: Yes\n", s[temp] );
	}
	
	execTime.tv_sec=endTime.tv_sec - startTime.tv_sec;
	if(endTime.tv_usec < startTime.tv_usec)
	{
		execTime.tv_sec=execTime.tv_sec-1;
		execTime.tv_usec=startTime.tv_usec - endTime.tv_usec;
	}
	else
		execTime.tv_usec=endTime.tv_usec - startTime.tv_usec;

	printf("Time: %ld.%06ld\n", execTime.tv_sec, execTime.tv_usec );

	free(s);
	free(h);
} //end ondiskseq












//On Disk Binary
void ondiskbin(char *key, char *seek)
{
	int countKeySize, countSeekSize, temp, tempKey;
	int *s, *h;
	struct timeval startTime, endTime, execTime;

	//Opening Seek file
	FILE *seekFile;
	seekFile = fopen(seek,"rb");

	//Counting number of keys
	countSeekSize = 0;
	while(1)
	{
		fread(&temp,sizeof(int),1,seekFile);
		if(feof(seekFile)) break;
		countSeekSize++;
	}

	//Going back to start of file:
	clearerr(seekFile);
	fseek(seekFile,0,SEEK_SET);

	//Creating array S for Seek integers
	s = malloc(countSeekSize * sizeof(int));

	for(temp=0;temp<countSeekSize;temp++)
	{
		fread(&s[temp],sizeof(int),1,seekFile);
	}	
	
	fclose(seekFile);
	
	//Now we have array s;
	//creating h
	h = malloc(countSeekSize*sizeof(int));

	gettimeofday(&startTime, NULL);

	//Opening Key file
	FILE *keyFile;
	keyFile = fopen(key,"rb");
	countKeySize=0;
	while(1)
	{
		fread(&temp,sizeof(int),1,keyFile);
		if(feof(keyFile)) break;
		countKeySize++;
	}

	clearerr(keyFile);

	//binary search
	for (temp=0;temp<countSeekSize;temp++)
	{
		h[temp] = binarySearchOnDisk(s[temp],keyFile, 0, countKeySize-1);
		clearerr(keyFile);
	}

	gettimeofday(&endTime, NULL);
	
	fclose(keyFile);

	for (temp=0;temp<countSeekSize;temp++)
	{
		if(h[temp]==0)
			printf( "%12d: No\n", s[temp] );
		else
			printf( "%12d: Yes\n", s[temp] );
	}
	
	execTime.tv_sec=endTime.tv_sec - startTime.tv_sec;
	if(endTime.tv_usec < startTime.tv_usec)
	{
		execTime.tv_sec=execTime.tv_sec-1;
		execTime.tv_usec=startTime.tv_usec - endTime.tv_usec;
	}
	else
		execTime.tv_usec=endTime.tv_usec - startTime.tv_usec;

	printf("Time: %ld.%06ld\n", execTime.tv_sec, execTime.tv_usec );	

	free(s);
	free(h);
}

int main(int parameterCount, char* argument[])
{
	//check if parameters passed is correct:
	if(parameterCount!=4)
	{
		printf("\nInvalid input\n");
		return 0;
	}	

	if(!strcmp(argument[1],"--mem-lin"))	//nIn-Memory Sequential
		inmemseq(argument[2], argument[3]);

	else if(!strcmp(argument[1],"--mem-bin"))	//In-Memory Binary
		inmembin(argument[2], argument[3]);

	else if(!strcmp(argument[1],"--disk-lin"))	//On-Disk Sequential
			ondiskseq(argument[2], argument[3]);			

	else if(!strcmp(argument[1],"--disk-bin"))	//On-Disk Binary
		ondiskbin(argument[2], argument[3]);
	else
		printf("\nInvalid mode\n");

	return 0;
} //end main