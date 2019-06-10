#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1

struct proc{
	char code[30];
	char codeop[7];
	char codeor[6];
	int label;
};

struct node{
	char val[30];
	char valfull[50];
	char type[20];
	char infunc;
	char inarray;
	struct node *next;
};

void push(struct node * head, char val[],char type[],char infunc,char inarray) {
    struct node *current = head;
    int a=0;
    while (current->next != NULL) {
    	current=current->next;
		if(strcmp(current->val,val) == 0 ){		
    		return;
		}
		else if(strcmp(current->type,type) == 0){
			a=1;	
		}
    }
    if(a){
    	current->next = malloc(sizeof(struct node));
    	strcpy(current->next->val,val);
    	strcpy(current->next->type,"\0");
    	current->next->next = NULL;
	}
    current = head;
    while(current->next != NULL){
    	current = current->next;
    if(strcmp(current->type,type)==0 && current->infunc == 0){
			strcat(current->valfull,",");
			strcat(current->valfull,val);
			if(inarray == 1){
				strcat(current->valfull,"[100]");
			}		
			return;		
		}
	}
    current->next = malloc(sizeof(struct node));
    strcpy(current->next->val,val);
    strcpy(current->next->valfull,val);
    if(inarray==1){
    	strcat(current->next->valfull,"[100]");
	}
    strcpy(current->next->type,type);
    current->next->infunc = infunc;
    current->next->next = NULL;
}

struct proc *compare(char str[]){
	char *code[5][5] = {{"PROCEDURE","BEGIN","END","MAIN()"},
						{"PRINT_LINE","GET_VAL"},
						{"IF","WHILE"},
						{"FOR"},
						{"RETURN","VOID","DEFINE"}};
	char *codecor[5][5] = {{"void","{","}","main()"},
						{"printf(","scanf("},
						{"if","while"},
						{"for("},
						{"return"," "," "}};
	
	struct proc *ps;
	ps = (struct proc*)malloc(sizeof(struct proc));
	int i,j,lab;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){		
			if(strcmpi(code[i][j],str)==0){
				strcpy(ps->code,codecor[i][j]);
				ps->label = i*10+j;
				return ps;
			}
		}
	}
	ps->label = 155;
	return ps;
}
struct proc *compareop(char str[]){
	char *datatypes[20] = 	{"ip","i","cp","c","file","ldp","ld","dp","d","lp","l","fp","f"};
	char *datatypescor[20] = {"int *","int","char *","char","FILE *","long double *","long double","double *","double","long *","long","float *","float"};
	char *datatypescor2[20] = {"d","d","c","c","0","Lf","Lf","lf","lf","ld","ld","f","f"};
	struct proc *ps;
	ps = (struct proc*)malloc(sizeof(struct proc));
	int i,j,k=0,len;		
	for(i=0;i<20;i++){
		k=0;
		len = strlen(datatypes[i]);
			while(str[k]==datatypes[i][k] &&  datatypes[i][k] != '\0'){
				k++;
			}
			if(k==len) {
				strcpy(ps->codeor,datatypes[i]);
				strcpy(ps->code,datatypescor[i]);
				strcpy(ps->codeop,datatypescor2[i]);
				ps->label = len;
				return ps;
			}
		
	}
			
	
	return NULL;
}

char *ops(char str[]){
	char *ops[2][6] = {{"<-","<",">",">=","<="},
						{"EQUAL","NOT_EQUAL","AND","OR"}};
	char *opscor[2][6] = {{"=","<",">",">=","<="},
						{"==","!=","&&","||"}};
	int i,j;
	for(i=0;i<2;i++){
		for(j=0;j<6;j++){
			if(strcmpi(ops[i][j],str)==0){
				return opscor[i][j];
			}
		}
	}
	return "\0";
}


int main(){
	FILE *ps,*c;
	struct proc *cody,*cody2,*cody3;
	int i=0,j=0,k=0,l=0,$count,len,len2,a=0;
	cody = (struct proc*)malloc(sizeof(struct proc));
	cody2 = (struct proc*)malloc(sizeof(struct proc));
	cody3 = (struct proc*)malloc(sizeof(struct proc));
	//char filename[50];
	//printf("Enter the file name:");
	//scanf("%s",filename);
	ps = fopen("YP_P1.txt","r+");
	c = fopen("Ccode2.c","w+");
	if(ps==NULL || c==NULL){
		printf("ERROR");
		exit(1);
	}
	
	struct node *head;
	head = malloc(sizeof(struct node));
	head->next = NULL;

	char str[75],str1[75],str2[75],str3[20],str4[20];
	char ch,inarray=0;
	char strarr[5][10];
	fprintf(c,"#include <stdio.h>\n");
	
	fseek(ps,0,SEEK_SET);
	while(!feof(ps)){
		fscanf(ps,"%s",str);
		if(strcmp(str,"DEFINE")==0){
			fscanf(ps,"%s",str);
			fscanf(ps,"%s",str);
		}
		if(strcmp(str,"PROCEDURE")==0){
			fgets(str,50,ps);
			while(str[i] != '\0'){
				memset(str2,0,strlen(str2));
				j=0;
				while((str[i] >= 'a') && (str[i] <= 'z')){
					str2[j] = str[i];
					i++;
					j++;
				}
				if(str[i] == '_'){
					k=0;
					while((str[i+1] >= 'a') && (str[i+1] <= 'z')){
						str1[k] = str[i+1];
						i++;
						k++;
					}
					cody2 = compareop(str1);
					if(str[i+1]=='['){
							inarray = 1;
					}
					else{
						inarray = 0;
					}
					push(head,str2,cody2->code,1,inarray);
				}
				i++;
			}
		}
		i=0;
		cody = compare(str);
		strcpy(str1,ops(str));
		if(cody->label == 155 && strcmp(str1,"\0") == 0){
			while(str[i] != '\0'){
				memset(str2,0,strlen(str2));
				memset(str1,0,strlen(str1));
				j=0;
				while((str[i] >= 'a') && (str[i] <= 'z')){
					str2[j] = str[i];
					i++;
					j++;
				}
				if(str[i] == '_'){
					k=0;
					while((str[i+1] >= 'a') && (str[i+1] <= 'z')){
						str1[k] = str[i+1];
						i++;
						k++;
					}
					cody2 = compareop(str1);
					if(str[i+1]=='['){
							inarray = 1;
					}
					else{
						inarray = 0;
					}
					
				push(head,str2,cody2->code,0,inarray);
				}
				i++;
			}
		}
	}
	
	fseek(ps,0,SEEK_SET);
	i=0;
	char strret[10];
	while(!feof(ps)){
		fscanf(ps,"%s",str1);
		if(strcmp(str1,"RETURN")==0){
			len = ftell(ps);
			fscanf(ps,"%s",str1);
			if(strcmp(str1,"VOID")==0){
				strcpy(cody3->code,"void");
			}
			else{
				fseek(ps,len,SEEK_SET);
				ch = fgetc(ps);
				ch = fgetc(ps);
				while(ch != '_'){
					strret[i] = ch;
					ch = fgetc(ps);
				}
				fgets(str1,5,ps);
				cody3 = compareop(str1);
			}
		}
	}

	fseek(ps,0,SEEK_SET);
	while(!feof(ps)){
		len2 = ftell(ps);	
		fscanf(ps,"%s",str);	
		cody = compare(str);
		switch (cody->label){
			case 0:			//PROCEDURE
				i=0;
				len = ftell(ps);
				fscanf(ps,"%s",str1);
				if(strcmp(str1,"MAIN()")==0){
					fprintf(c, "%s %s",cody->code,"main()");
					fputs("{\n",c);
					struct node *current = head;
					while(current->next != NULL){
					current = current->next;
					if(strcmp(current->type,"\0") != 0 && current->infunc ==0){
					fprintf(c,"%s ",current->type);
					fprintf(c,"%s;\n",current->valfull);
					printf("%s ",current->type);
					printf("%s;\n",current->valfull);
					}
					}
				}
				else{
					fprintf(c,"%s ",cody3->code);
					fseek(ps,len,SEEK_SET);
					ch = fgetc(ps);
					while(ch != '('){
						ch = fgetc(ps);
						fputc(ch,c);						
					}
						ch =fgetc(ps);
						if (ch != ')'){
						
						while(ch != '_'){
							str3[i]	= ch;
							i++;
							ch = fgetc(ps);
						}
		
						
						len = ftell(ps);
						fgets(str1,5,ps);
						cody3 = compareop(str1);
						fseek(ps,len,SEEK_SET);
						fputs(cody3->code,c);
						fputc(' ',c);
						for(j=0;j<i;j++){
							fputc(str3[j],c);
						}
						while(ch != '\n'){
							if(ch == ','){
								fputc(',',c);
								ch = fgetc(ps);
								i=0;
								while(ch != '_'){
									str3[i] = ch;
									i++;
									ch = fgetc(ps);
								}
								len = ftell(ps);
								fgets(str1,5,ps);
								cody3 = compareop(str1);
								fseek(ps,len,SEEK_SET);
								fputs(cody3->code,c);
								fputc(' ',c);
								for(j=0;j<i;j++){
								fputc(str3[j],c);
								}
							}
							ch = fgetc(ps);
						}
					}
					fputs("){\n",c);
					struct node *current = head;
					while(current->next != NULL){
					current = current->next;
					if(strcmp(current->type,"\0") != 0  && current->infunc ==0){
					fprintf(c,"%s ",current->type);
					fprintf(c,"%s;\n",current->valfull);
					printf("%s ",current->type);
					printf("%s;\n",current->valfull);
					}
					}
					
				}
			fscanf(ps,"%s",str);
			break;
			case 1:						//BEGIN
				fprintf(c,"%s\n",cody->code);
				break;
			case 2: 					//END
				fprintf(c,"%s\n",cody->code);	
				break;
			case 10: case 11:			//PRINT_LINE & GET_VAL
				fputs(cody->code,c);
				i=0;
				j=0;
				k=0;
				l=0;
				a=0;
				ch = fgetc(ps);
				ch = fgetc(ps);
				while(ch != '\n'){
					if(ch == '$'){
						ch = '%';
						fputc(ch,c);
						ch=fgetc(ps);
						j=0;
						while(ch != '_'){
							strarr[i][j] = ch;
							j++;
							ch = fgetc(ps);
						}
						
						len = ftell(ps);
						fgets(str1,5,ps);
							for(k=0;k<5;k++){
								if(str1[k] == '['){
									while(str1[k] != '_'){
									strarr[i][j] = str1[k];
									k++;
									j++;
								}
								strarr[i][j] = ']';
							}
						}
						
						j++;
						i++;
						cody2 = compareop(str1);
						fputs(cody2->codeop,c);
						len2 = strlen(cody2->codeor);
						
						fseek(ps,len+len2-1,SEEK_SET);
						ch = fgetc(ps);
						a=1;
					}
					else if(ch == '['){
						while(ch != ']'){
							ch = fgetc(ps);
						}
					}
					else{
						if(ch == '"' && a)
						fputs("\\n",c);
						fputc(ch,c);
						a=1;
					}
					ch = fgetc(ps);
					
				}
			for(k=0;k<i;k++){
				fputc(',',c);
				if(cody->label==11){
					fputc('&',c);
				}
				for(l=0;l<j;l++){
					fputc(strarr[k][l],c);
				}
			}
			fputs(");\n",c);
			for(l=0;l<j;l++){
				memset(strarr[l],0,20);
			}
			break;
		case 20:   case 21:							//IF  & WHILE
			fputs(cody->code,c);
			fscanf(ps,"%s",str1);
			while(strcmp(str1,"THEN") !=0 && strcmp(str1,"DO") != 0){
			a=0;
				str2[0] = '\0';
				strcpy(str2,ops(str1));
				if(str2[0] != '\0'){
					fprintf(c," %s ",str2);
				}
				else{
					i=0;
					while(str1[i] != '\0'){
						if (str1[i] == '_'){
							a=1;
							while((str1[i+1] >= 'a') && (str1[i+1] <= 'z')){
								i++;
								a=0;
							}
						}
						else if(!a){
							fputc(str1[i],c);
						}
						i++;
					}
				}
				strcpy(str1,"\0");
				fscanf(ps,"%s",str1);
			}

			break;
		
		case 30:							//FOR							
		fputs(cody->code,c);
		fscanf(ps,"%s",str1);
		i=0;
		j=0;
		memset(str2,0,strlen(str2));
		while(str1[i] != '_'){
			str2[j] = str1[i];
			i++;
			j++;
		}
		strcpy(str3,str2);
		k=j;
		fscanf(ps,"%s",str1);
		if(strcmp(str1,"<-")==0){
			str2[j] = '=';
			j++;
		}
		i=0;
		fscanf(ps,"%s",str1);
		while(str1[i] != '.'){
			str2[j] = str1[i];
			i++;
			j++;
		}
		str2[j] = ';';
		j++;
		len = strlen(str3);
		for(l=0;l<len;l++){
			str2[j] = str3[l];
			j++;
		}
		printf("%s ",str2);
		str2[j] = '<';
		j++;
		i+=2;
		while(str1[i] != '_'){
			str2[j] = str1[i];
			i++;
			j++;
		}
		str2[j] = ';';
		strcat(str2,str3);
		strcat(str2,"++)");
		fprintf(c,"%s",str2);
		fscanf(ps,"%s",str);
		
		break;
		case 40:							//RETURN
			fscanf(ps,"%s",str1);
			if(strcmpi(str1,"VOID") != 0){
			fputs("return ",c);
			fputs(strret,c);
			fputs(";\n",c);
				}
			break;
		case 42:							//DEFINE
			
			fscanf(ps,"%s",str);
			i=0;
			j=0;
			memset(str1,0,strlen(str1));
			memset(str2,0,strlen(str2));
			while (str[i] != '_'){
				str2[j] = str[i];
				i++;
				j++;
			}
			i++;
			j=0;
			while((str[i] >= 'a') && (str[i] <= 'z')){
				str1[j] = str[i];
				j++;
				i++;
			}
			cody2 = compareop(str1);
			fprintf(c,"%s ",cody2->code);
			fprintf(c,"%s",str2);
			while(str[i] != '\0'){
				fputc(str[i],c);
				i++;
			}
			fputs(";\n",c);
			
		break;
			default:						//ATAMA
				do{
					i=0;
					if(strcmp(str,"<-")!=0){
						a=0;
						while(str[i] != '\0'){
								if(str[i]== '_'){
								
									a=1;
										while((str[i+1] >= 'a') && (str[i+1] <= 'z')){
										i++;
										a=0;
										}
								}
									
								else if (!a){
									fputc(str[i],c);
								}
							
								i++;
						}
						a=0;
					}
					else{
						fputs(" = ",c);
					}
					ch = fgetc(ps);
					if(ch != '\n'){
						fscanf(ps,"%s",str);
					}
					if(ch == '\n'){
						fputc(';',c);
					}
				}while(ch != '\n');
				fputs("\n",c);
			break;
		}		
	}
	
	
	fclose(ps);
	fclose(c);
	system("gcc Ccode2.c -o Ccode2.exe");
	system("Ccode2.exe");
	return 0;
}
