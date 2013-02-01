/* Onur YILMAZ		*/
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main () 
{

char okunan[200];
int i;
char temp;
int dijkstra(char*, double*, double*, double*,int*);
double evaluation_pf(double*, double*, int, double*,int);
double  output_dijk[200]={0};
double output_dijk_id[200]={0};
double  *p_output;
double  stack_dijk[200]={0};
double  *p_stack;
double *p_output_id;
long int aralik_sayisi;
int variable_sayisi;
double temp_alt;
double temp_ust;
double min, max;
double** datalar;
char temp_harf;
double temp_probability;
long int temp_eklencek_sayi;
long int t;
long int j,k;
long int exp;
int output_uzunlugu;
long int z;
int *p_output_uzunlugu=&output_uzunlugu;
long int experiment_sayisi;
long int toplam_oran;
double *araliklar;
double *kullanilan_array;
double *random_gelen_sayilar;
double *sonuclar_raw;
float *aralik_sayilar;
double sonuc_donen;
long int random_number;
double sonuclar_max;
double sonuclar_min;

srand(time(0));

p_stack=stack_dijk;
p_output=output_dijk;
p_output_id=output_dijk_id;

/* reading the input */
for(i=0;1;i++)
{
	scanf("%c", &temp);
	if(temp=='\n') 
	{
	okunan[i]='\0'; 
	break;
	}
	if(temp==' ' || temp=='\t') 
	i--; 
	else 
	okunan[i]=temp;
}


variable_sayisi=dijkstra(okunan, p_output,p_output_id, p_stack,p_output_uzunlugu); 

scanf(" %ld", &aralik_sayisi);
scanf(" %ld", &experiment_sayisi);

datalar=malloc(sizeof(double*)*variable_sayisi);

/* storing pointers for probability values */
for(i=0; i<variable_sayisi;i++)
	datalar[i]=(double *) malloc(sizeof(double)*(experiment_sayisi+10));

z=0;
for(i=0; i<variable_sayisi;i++)
{ 
scanf(" %c", &temp_harf);
scanf(" %lf", &temp_alt);
scanf(" %lf", &temp_ust);
kullanilan_array=datalar[i];
kullanilan_array[0]=temp_harf;  
k=1;
	for(j=0;j<aralik_sayisi;j++)
		{	/* creating an array for each variable for proportional randomization */
			scanf(" %lf", &temp_probability);
			temp_eklencek_sayi=temp_probability*experiment_sayisi;
			k=z;
			for(t=0;t<temp_eklencek_sayi;t++,z++)
			kullanilan_array[t+k]=((temp_alt+((temp_ust-temp_alt)/aralik_sayisi)*j+temp_alt+((temp_ust-temp_alt)/aralik_sayisi)*(j+1))/2);
		 } 
		
	while(z<experiment_sayisi)
	{
	kullanilan_array[z]=temp_ust; 
	z++;
	}	
}

random_gelen_sayilar=malloc(sizeof(double)*2*variable_sayisi+1);
sonuclar_raw=malloc(sizeof(double)*experiment_sayisi);

for(exp=0;exp<experiment_sayisi;exp++)
	{
		/* generating a random number and using it as index for the array created above */		
		for(i=0; i<variable_sayisi;i++)
		{ 
		kullanilan_array=datalar[i];
		random_gelen_sayilar[i*2]=kullanilan_array[0];
		random_number=rand()%experiment_sayisi+1;
		random_gelen_sayilar[(i*2)+1]=kullanilan_array[random_number];
		}
		/* storing the evaluated results of functions in an array */
		sonuc_donen=evaluation_pf(p_output,p_output_id, output_uzunlugu,random_gelen_sayilar,variable_sayisi);
		random_gelen_sayilar[exp]=sonuc_donen; 
	}

/* finding the minimum and the maximum of results */
sonuclar_max=sonuclar_raw[0];
sonuclar_min=sonuclar_raw[0];

for(exp=0;exp<experiment_sayisi;exp++)
	{
	if(sonuclar_raw[exp]>sonuclar_max) sonuclar_max=sonuclar_raw[exp];
	if(sonuclar_raw[exp]<sonuclar_min) sonuclar_min=sonuclar_raw[exp];
	}

/* printing the min and max of results */
printf("%f %f ", sonuclar_min, sonuclar_max);


araliklar=malloc(sizeof(double)*aralik_sayisi+1);

/* finding the intervals for evaluation results */
for(i=0; i<aralik_sayisi+1;i++)
	araliklar[i]=sonuclar_min+((sonuclar_max-sonuclar_min)/aralik_sayisi)*i;

aralik_sayilar=malloc(sizeof(double)*aralik_sayisi);

for(i=0; i<aralik_sayisi;i++)
	aralik_sayilar[i]=0;

/* counting the number of results in the related intervals */
for(i=0; i<aralik_sayisi;i++)
		{
		min=araliklar[i];
		max=araliklar[i+1];
			for(exp=0;exp<experiment_sayisi;exp++)
				if(sonuclar_raw[exp]>=min && sonuclar_raw[exp]<=max) 
				aralik_sayilar[i]++;
		}

toplam_oran=0;
for(i=0; i<aralik_sayisi;i++)
	toplam_oran+=aralik_sayilar[i];

/* calculating the probabilities of intervals */
for(i=0; i<aralik_sayisi;i++)
	aralik_sayilar[i]=aralik_sayilar[i]/toplam_oran;

/* printing the interval probabilities */
for(i=0; i<aralik_sayisi-1;i++)
printf("%f ",aralik_sayilar[i]);
printf("%f",aralik_sayilar[aralik_sayisi-1]);
printf("\n");
return 0;
}

/* Function for converting infix notation to postfix notation */
/* Algorithm is retrieved from  en.wikipedia.org/wiki/Shunting-yard_algorithm */
/* Used ID's for functions and operators are as follows: 
	~	0.880
	sin	0.881
	cos	0.882
	sqrt	0.883
	ln	0.884
	+	0.994
	-	0.993
	*	0.995
	/	0.996
	^	0.997
	(	0.771
	)	0.772
*/

int dijkstra(char *alinan, double  *p_output, double *p_output_id, double *p_stack,int *p_output_uzunlugu)
{
int i;
double okunansayi;
double *stack_dolas;
double *output_dolas;
double *output_id_dolas;
int output_sayar=-1;
int stack_sayar=-1;
int yeni_harf;
int j;
int tempharf;
int harfsayisi;

/* assigning pointers */
stack_dolas=p_stack;
output_dolas=p_output;
output_id_dolas=p_output_id;

i=0;

while(1)
{
/* if there are no more tokens to read */
if(alinan[i]=='\0') 
{
		while(stack_sayar>=0)
		{
		output_sayar++;		
		output_dolas[output_sayar]=stack_dolas[stack_sayar];
		output_id_dolas[output_sayar]=1; 
		stack_sayar--;
		}
break;
}

else
{	/* if the read token is unary minus */
	if(alinan[i]=='~') 
	{
	stack_sayar++;	
	stack_dolas[stack_sayar]=0.880;
	i++;	
	}
	/* if the read token is cosine function */	
	else if(alinan[i]=='c') 
	{
	stack_sayar++;
	stack_dolas[stack_sayar]=0.882;
	i=i+3;	
	}
	/* if the read token is sine function */	
	else if(alinan[i]=='s' && alinan[i+1]=='i') 
	{
	stack_sayar++;
	stack_dolas[stack_sayar]=0.881;
	i=i+3;	
	}
	/* if the read token is sqrt function */
	else if(alinan[i]=='s' && alinan[i+1]=='q') 
	{
	stack_sayar++;	
	stack_dolas[stack_sayar]=0.883;
	i=i+4;	
	}
	/* if the read function is ln function */
	else if(alinan[i]=='l') 
	{
	stack_sayar++;	
	stack_dolas[stack_sayar]=0.884;
	i=i+2;	
	}	
	/* if the read token is a variable */
	else if(alinan[i]>='A' && alinan[i]<='Z') 
	{
	output_sayar++;
	output_dolas[output_sayar]=alinan[i];
	output_id_dolas[output_sayar]=2;
	i++;
	}
	/* if the read token is "(" */
	else if(alinan[i]=='(') 
	{
	stack_sayar++;	
	stack_dolas[stack_sayar]=0.771;
	i=i+1;	
	}	
	/* if the read token is ^ operator */
	else if(alinan[i]=='^') 
	{	
		stack_sayar++; 
		stack_dolas[stack_sayar]=0.997; 
		i=i+1;	
	}
	/* if the read token is * operator */
	else if(alinan[i]=='*') 
	{
		while(stack_dolas[stack_sayar]==0.996 || stack_dolas[stack_sayar]==0.995 || stack_dolas[stack_sayar]==0.997)
		{
		output_sayar++;
		output_dolas[output_sayar]=stack_dolas[stack_sayar];
		output_id_dolas[output_sayar]=1;		
		stack_sayar--;
		}
	stack_sayar++;	
	stack_dolas[stack_sayar]=0.995;
	i=i+1;	
	}
	/* if the read token is binary minus */
	else if(alinan[i]=='-') 
	{
		while(stack_dolas[stack_sayar]>=0.993 && stack_dolas[stack_sayar]<=0.997)
		{
		output_sayar++;		
		output_dolas[output_sayar]=stack_dolas[stack_sayar];
		output_id_dolas[output_sayar]=1;
		stack_sayar--;
		}
	stack_sayar++;	
	stack_dolas[stack_sayar]=0.993;
	i=i+1;	
	}
	/* if the read token is binary + */
	else if(alinan[i]=='+') 
	{
		while(stack_dolas[stack_sayar]>=0.993 && stack_dolas[stack_sayar]<=0.997)
		{
		output_sayar++;		
		output_dolas[output_sayar]=stack_dolas[stack_sayar];
		output_id_dolas[output_sayar]=1;
		stack_sayar--;
		}
	stack_sayar++;	
	stack_dolas[stack_sayar]=0.994;
	i=i+1;	
	}
	
	/* if the read token is operator "/" */ 
	else if(alinan[i]=='/') 
	{
		while(stack_dolas[stack_sayar]==0.996 || stack_dolas[stack_sayar]==0.995 || stack_dolas[stack_sayar]==0.997)
		{
		output_sayar++;		
		output_dolas[output_sayar]=stack_dolas[stack_sayar];
		output_id_dolas[output_sayar]=1;
		stack_sayar--;
		}
	stack_sayar++;	
	stack_dolas[stack_sayar]=0.996;
	i=i+1;	
	}

	/* if the read token is ")" */
	else if(alinan[i]==')') 
	{
	while(stack_dolas[stack_sayar]!=0.771)
		if(stack_dolas[stack_sayar]>=0.993 && stack_dolas[stack_sayar]<=0.996)
		{	
		output_sayar++;		
		output_dolas[output_sayar]=stack_dolas[stack_sayar];		
		output_id_dolas[output_sayar]=1.0;
		stack_dolas[stack_sayar]=0.0;
		stack_sayar--;			
		}
	
	stack_dolas[stack_sayar]=0.0;
	stack_sayar--; 

	if(stack_dolas[stack_sayar]>=0.880 && stack_dolas[stack_sayar]<=0.884)
		{
		output_sayar++;
		output_dolas[output_sayar]=stack_dolas[stack_sayar];	
		output_id_dolas[output_sayar]=1.0;			
		stack_sayar--;
		} 
	
	i=i+1;
	}
	/* if the read token is a number */
	else
	{
	okunansayi=atof(&alinan[i]);
	for(;alinan[i]<='9' && alinan[i]>='0'; i++);
	if(alinan[i]=='.') 
		{
		i++;
		for(;alinan[i]<='9' && alinan[i]>='0'; i++);
		}		
	output_sayar++;
	output_dolas[output_sayar]=okunansayi;
	
	}
		
}
}

/* counting the numbers of variables */
harfsayisi=0;
for(i=0;i<=200;i++)
{
yeni_harf=0;
	if(p_output_id[i]==2)
	{
	tempharf=p_output[i];
	yeni_harf=1;					
		for(j=0;j<i;j++)
		{
			if(p_output_id[j]==2)
			{
				if(p_output[j]!=tempharf)
				yeni_harf=yeni_harf*1; 
				else 
				yeni_harf=0;
			}
		} 
					
	}
harfsayisi+=yeni_harf;
}

return harfsayisi;

/*end of infix to postfix function */
}


/* Function for evaluating post-fix notation */
/* Algorithm is retrieved from en.wikipedia.org/wiki/Reverse_Polish_Notation */
/* Same ID's are used for operators and functions in the previous function */

double evaluation_pf(double  *p_output, double *p_output_id, int uzunluk,double *random_gelen_sayilar, int variable_sayisi)
{

double *stack_dolas;
double *output_dolas;
double *output_id_dolas;
double stack[200]={0}; 

int stack_sayar=-1;
int i;
int j;

float temp;
float temp_harf;

/* assigning pointer values */
stack_dolas=stack;
output_dolas=p_output;
output_id_dolas=p_output_id;

/* changing the random values for variables */
for(i=0;i<2*variable_sayisi;i++)
{
temp_harf=random_gelen_sayilar[i*2];
	for(j=0;j<=uzunluk;j++)
		if(output_id_dolas[j]==2.0 && output_dolas[j]==temp_harf)	
			{			
			output_dolas[j]=random_gelen_sayilar[i*2+1];
			output_id_dolas[j]=temp_harf;
			}
}

for(i=0; i<=uzunluk; i++)
{
if(output_id_dolas[i]!=1.0)
	{
	stack_sayar++; stack_dolas[stack_sayar]=output_dolas[i];
	}

else
{
		/* if the read token is + */
		if( output_dolas[i]==0.994)
		{
		temp=stack_dolas[stack_sayar]+stack_dolas[stack_sayar-1];
		stack_sayar=stack_sayar-2;
		stack_sayar++;
		stack_dolas[stack_sayar]=temp;
		}

		/* if the read token is - */
		else if(output_dolas[i]==0.993)
		{
		temp=stack_dolas[stack_sayar-1]-stack_dolas[stack_sayar];
		stack_sayar=stack_sayar-2;
		stack_sayar++;
		stack_dolas[stack_sayar]=temp;
		}

		/* if the read token is / */
		else if( output_dolas[i]==0.996)
		{
		temp=stack_dolas[stack_sayar-1]/stack_dolas[stack_sayar];
		stack_sayar=stack_sayar-2;
		stack_sayar++;
		stack_dolas[stack_sayar]=temp;
		}		

		/* if the read token is * */
		else if( output_dolas[i]==0.995)
		{
		temp=stack_dolas[stack_sayar]*stack_dolas[stack_sayar-1];
		stack_sayar=stack_sayar-2;
		stack_sayar++;
		stack_dolas[stack_sayar]=temp;
		}
		
		/* if the read token is ^ */
		else if( output_dolas[i]==0.997)
		{
		temp=pow(stack_dolas[stack_sayar-1],stack_dolas[stack_sayar]);
		stack_sayar=stack_sayar-2;
		stack_sayar++;
		stack_dolas[stack_sayar]=temp;
		}
		
		/* if the read token is ~ */
		else if( output_dolas[i]==0.880)
		{
		temp=-stack_dolas[stack_sayar];
		stack_dolas[stack_sayar]=temp;
		}

		/* if the read token is sin */
		else if( output_dolas[i]==0.881)
		{
		temp=sin(stack_dolas[stack_sayar]);
		stack_dolas[stack_sayar]=temp;
		}

		/* if the read token is cos */
		else if( output_dolas[i]==0.882)
		{
		temp=cos(stack_dolas[stack_sayar]);
		stack_dolas[stack_sayar]=temp;
		}

		/* if the read token is sqrt */
		else if( output_dolas[i]==0.883)
		{
		temp=sqrt(stack_dolas[stack_sayar]);
		stack_dolas[stack_sayar]=temp;
		}
		
		/* if the read token is ln */
		else if( output_dolas[i]==0.884)
		{
		temp=log(stack_dolas[stack_sayar]);
		stack_dolas[stack_sayar]=temp;
		}	
		else;
}
}
	/* replacing the random variables with variable names */
	for(j=0;j<=uzunluk;j++)
		if(output_id_dolas[j]!=0 && output_id_dolas[j]!=1)	
			{			
			output_dolas[j]=output_id_dolas[j];
			output_id_dolas[j]=2.0;
			} 
		else;

return stack[stack_sayar];

/*End of evaluation function */
}


/* End of code */
