#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

char string2[100][5000][30];
int it[100];
char repstring[100][30];
int arr[100];
int comparrSpam[45][100];
int comparrHam[550][100];
int checkRep[100];

int ReturnDistance(char *s1, char *s2) {
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len+1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return(column[s1len]);
}

void ComputeKMeans(char string[10000][30], int k, int word){
	int iterator=1;
	int rep,n,i,j=0,label;
	char *ptr;
	
	for(i=0;i<k;i++)
		it[i]=1;
	strcpy(string2[0][0],string[0]);
	//printf("%s", string2[0][0]);
	if(iterator==1){
		for(rep=1;rep<k;rep++){
			n = rand()% word+1;
			strcpy(string2[rep][0],string[n]);
			for(i=0;i<rep;i++){
				if(strcmp(string2[rep][0],string2[i][0])==0){
					n = rand()% word+1;
					strcpy(string2[rep][0],string[n]);
				}
			}
			//printf("%s\n",string2[rep][0]);
		}
	}
	int dis,d;
	while(j<word){
		dis=10000;
		for(i=0;i<k;i++){
			d = ReturnDistance(string[j],string2[i][0]);
			if(d<dis){
				label=i;
				dis=d;
			}
		}
		//printf("%d\n", label);
		strcpy(string2[label][it[label]++],string[j]);
		j++;
	}
	/*for(i=0;i<k;i++){
		for(j=0;j<it[i];j++){
			printf("Group %d  \t", i+1);
			printf("%s\n", string2[i][j]);
		}
		//printf("\n");
		
	}*/

	int sum=0;
	/*for(i=0;i<k;i++){
		printf("%d     ",it[i]);
		sum+=it[i];
	}
	printf("%d     \n", sum);
	*/
	int sumDist, min, z, chIndex;
	


	/*PLEASE CHECK HERE*/
	while(iterator<4){
		for(i=0;i<k;i++){
			min = 10000000;
			chIndex = 0;
			for(j=0;j<it[i];j++){
				sumDist = 0;
				for(z=0;z<it[i];z++)
					sumDist+=ReturnDistance(string2[i][j],string2[i][z]);
				if(sumDist<min){
					chIndex = j;
					min = sumDist;
				}
			}
		//printf("%d\t", chIndex);
		strcpy(string2[i][0],string2[i][chIndex]);
		it[i]=1;
	}
	//printf("\n");
	//for(rep=0;rep<k;rep++)
		//printf("%s\n",string2[rep][0]);

	j=0;
	while(j<word){
		dis=10000;
		for(i=0;i<k;i++){
			d = ReturnDistance(string[j],string2[i][0]);
			if(d<dis){
				label=i;
				dis=d;
			}
		}
		//printf("%d\n", label);
		strcpy(string2[label][it[label]++],string[j]);
		j++;
	}
	sum = 0;
	for(i=0;i<k;i++){
		//printf("%d     ",it[i]);
		sum+=it[i];
	}
	//printf("%d     \n", sum);

	iterator++;
	}

	printf("THE REPRESENTATIVE WORDS ARE:- \n");
	for(i=0;i<k;i++){
		strcpy(repstring[i],string2[i][0]);
		printf("%d. %s\n", i+1, repstring[i]);
	}

}

int * ComputeHistogram(char repstring[50][30], char wordstring[100][30], int k, int strwrd){
	int i,j;

	for(i=0;i<k;i++)
		arr[i]=0;

	for(i=0;i<k;i++){
		for(j=0;j<strwrd;j++){
			if(strcmp(repstring[i],wordstring[j])==0){
				arr[i]++;
			}
		}
	}

	return arr;
}

int ComputeL1Distance(int arr1[100],int k){
	int dist;
	int min = 0;
	int label = 0;
	int i,j;

	for(i=0;i<k;i++){
		min += (arr[i]-comparrHam[0][i])*(arr[i]-comparrHam[0][i]);
	}

	for(i=0;i<510;i++){
		dist = 0;
		for(j=0;j<k;j++)
			dist+=(arr[j]-comparrHam[i][j])*(arr[j]-comparrHam[i][j]);

		if(dist<min){
			min = dist;
		}
	}

	//printf("MIN HAM = %d\n", min);


	for(i=0;i<41;i++){
		dist = 0;
		for(j=0;j<k;j++)
			dist+=(arr[j]-comparrSpam[i][j])*(arr[j]-comparrSpam[i][j]);

		//printf("SPAM DIST = %d\n", dist);

		if(dist<min){
			label = 1;
		}
	}

	return label;
}






int main(){
	FILE *fileptr1, *fileptr2, *fileptr3, *fileptr4, *fileptr5, *fileptr6;
	fileptr1 = fopen("english.txt", "r");
	fileptr2 = fopen("words.txt","w+");
	fileptr3 = fopen("spam.txt","w+");
	fileptr4 = fopen("nospam.txt","w+");
	fileptr5 = fopen("testspam.txt","w+");
	fileptr6 = fopen("testham.txt","w+");

	

	char c,cW[30], cStr[999];
	int i=0,j=0, size=2, s=0,h=0, wsize,z, lim,label,countSpam=0, countHam=0, wrongAns=0;
	float eff;


	lim = 75;

	for(i=0;i<45;i++)
		for(j=0;j<lim;j++)
			comparrSpam[i][j]=0;

		for(i=0;i<550;i++)
			for(j=0;j<lim;j++)
				comparrHam[i][j]=0;

			i=0;
			j=0;

	char string[10000][30], wordstring[100][30];
	int *ptr;
	int test[100];

	if(fileptr1==NULL){
		printf("No file found\n");
	}
	else{
		while((!feof(fileptr1)) && (s<510)){
			c=getc(fileptr1);
			while((c>='a' && c<='z')||(c>='A' && c<='Z')||(c>='0' && c<='9')){
					cW[i]=tolower(c);
					i++;
					c=getc(fileptr1);
				}
			cW[i]=' ';
			cW[i+1]='\0';

			if(strcmp(cW, "ham ")==0 || strcmp(cW,"spam ")==0){
				s++;
			}

			if(i>1 && strcmp(cW, "ham ")!=0 && strcmp(cW,"spam ")!=0 && strcmp(cW,"the ")!=0){	
				fputs(cW, fileptr2);
				strcpy(string[j],cW);
				j++;
				}	
				i=0;
			}

			printf("TOTAL NUMBER OF STRINGS MADE:-     %d\n", j);
	}

	ComputeKMeans(string, lim,j);

	s=0;
	fseek(fileptr1,0,SEEK_SET);

	while(fgets(cStr,999,fileptr1)!=0){
			if(cStr[strlen(cStr)-5]=='p'){
					cStr[strlen(cStr)-6]='\n';
					cStr[strlen(cStr)-5]='\0';
				if(s<41){
					fputs(cStr, fileptr3);
					s++;
				}
				else
					fputs(cStr, fileptr5);

			}
			else{
					cStr[strlen(cStr)-5]='\n';
					cStr[strlen(cStr)-4]='\0';
				if(h<510){
					fputs(cStr,fileptr4);
					h++;
				}
				else
					fputs(cStr, fileptr6);
			}
	}	


	printf("MADE NEW FILE \n");

	fseek(fileptr3, 0, SEEK_SET);
	i=0; s=0;
	while(s<41){
		wsize = 0;
		c = getc(fileptr3);
		while(c!='\n'){
			while(c>='a' && c<='z' ||(c>='A' && c<='Z') ||(c>='0' && c<='9')){
				cW[i] = tolower(c);
				i++;
				c=getc(fileptr3);
			}
			cW[i] = ' ';
			cW[i+1] = '\0';

			if(i>1){
				strcpy(wordstring[wsize],cW);
				wsize++;
			}
			i=0;

			if(c!='\n')
				c = getc(fileptr3);

		}
		s++;
		for(z=0;z<wsize;z++){
		}

		ptr = ComputeHistogram(repstring,wordstring,lim,wsize+1);

			for(j=0;j<lim;j++)
				comparrSpam[s-1][j] = ptr[j];

	}

	/*printf("Spam set\n");
	for(i=0;i<41;i++){
		for(j=0;j<lim;j++)
			printf("%d ", comparrSpam[i][j]);
		printf("\n");
	}*/

	fseek(fileptr4, 0, SEEK_SET);
	i=0; s=0;
	while(s<510){
		wsize = 0;
		c = getc(fileptr4);
		while(c!='\n'){
			while(c>='a' && c<='z' ||(c>='A' && c<='Z') ||(c>='0' && c<='9')){
				cW[i] = tolower(c);
				i++;
				c=getc(fileptr4);
			}
			cW[i] = ' ';
			cW[i+1] = '\0';

			if(i>1){
				strcpy(wordstring[wsize],cW);
				wsize++;
			}
			i=0;

			if(c!='\n')
				c = getc(fileptr4);

		}
		s++;
		for(z=0;z<wsize;z++){
		}

		ptr = ComputeHistogram(repstring,wordstring,lim,wsize+1);

			for(j=0;j<lim;j++)
				comparrHam[s-1][j] = ptr[j];

	}

	/*printf("Ham set\n");
	for(i=0;i<510;i++){
		for(j=0;j<lim;j++)
			printf("%d ", comparrHam[i][j]);
		printf("\n");
	}*/
	
	printf("Total test cases: 533\n Test spam cases: 41\n Test ham cases: 492\n");
	
	printf("TRAIN CASES\n");
	
	fseek(fileptr3, 0, SEEK_SET);
	i=0; s=0;
	while(s<41){
		wsize = 0;
		c = getc(fileptr3);
		while(c!='\n'){
			while(c>='a' && c<='z' ||(c>='A' && c<='Z') ||(c>='0' && c<='9')){
				cW[i] = tolower(c);
				i++;
				c=getc(fileptr3);
			}
			cW[i] = ' ';
			cW[i+1] = '\0';

			if(i>1){
				strcpy(wordstring[wsize],cW);
				wsize++;
			}
			i=0;

			if(c!='\n')
				c = getc(fileptr3);

		}
		s++;

		ptr = ComputeHistogram(repstring,wordstring,lim,wsize+1);

			label = ComputeL1Distance(test,lim);

			if(label==1){
				countSpam++;
			}

			else{
				countHam++;
				wrongAns++;

			}

	}
	printf("CORRECTLY READ     %d\t INCORRECTLY READ     %d\n",countSpam,countHam);
	countHam=0;
	countSpam=0;


	printf("SPAM CASES\n");
	
	fseek(fileptr5, 0, SEEK_SET);
	i=0; s=0;
	while(s<41){
		wsize = 0;
		c = getc(fileptr5);
		while(c!='\n'){
			while(c>='a' && c<='z' ||(c>='A' && c<='Z') ||(c>='0' && c<='9')){
				cW[i] = tolower(c);
				i++;
				c=getc(fileptr5);
			}
			cW[i] = ' ';
			cW[i+1] = '\0';

			if(i>1){
				strcpy(wordstring[wsize],cW);
				wsize++;
			}
			i=0;

			if(c!='\n')
				c = getc(fileptr5);

		}
		s++;

		ptr = ComputeHistogram(repstring,wordstring,lim,wsize+1);

			for(j=0;j<lim;j++)
				test[j] = ptr[j];

			label = ComputeL1Distance(test,lim);

			if(label==1){
				countSpam++;
			}

			else{
				countHam++;
				wrongAns++;
			}

	}
	printf("CORRECTLY READ     %d\t INCORRECTLY READ     %d\n",countSpam,countHam);
	countHam=0;
	countSpam=0;


	printf("HAM CASES\n");
	fseek(fileptr6, 0, SEEK_SET);
	i=0; s=0;
	while(s<492){
		wsize = 0;
		c = getc(fileptr6);
		while(c!='\n'){
			while(c>='a' && c<='z' ||(c>='A' && c<='Z') ||(c>='0' && c<='9')){
				cW[i] = tolower(c);
				i++;
				c=getc(fileptr6);
			}
			cW[i] = ' ';
			cW[i+1] = '\0';

			if(i>1){
				strcpy(wordstring[wsize],cW);
				wsize++;
			}
			i=0;

			if(c!='\n')
				c = getc(fileptr6);

		}
		s++;

		ptr = ComputeHistogram(repstring,wordstring,lim,wsize+1);

			for(j=0;j<lim;j++)
				test[j] = ptr[j];

			label = ComputeL1Distance(test,lim);

			if(label==1){
				countSpam++;
				wrongAns++;
			}

			else
				countHam++;

	}
	printf("CORRECTLY READ     %d\t INCORRECTLY READ     %d\n",countHam, countSpam);

	eff = (574.0 - wrongAns)/574.0;
	eff*=100;
	printf("EFFICIENCY      : %f\n", eff);
	
	


	fclose(fileptr1);
	fclose(fileptr2);
	fclose(fileptr3);
	fclose(fileptr4);
	fclose(fileptr5);
	fclose(fileptr6);


	return 0;
}
