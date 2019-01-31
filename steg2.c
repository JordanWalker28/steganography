/*
 * 
 * Coursework F28HS2
 * Part A and B
 * By Jordan Walker
 * H00222194
 * 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*Using a Link list as the primary Data Structure */

/*Part A Structures*/

//Create the linked list
typedef struct NODE 
{
	char *val;
	struct NODE *next;
}NODE;

//Hold the Pixel of the image
typedef struct PIXEL 
{
	int R, G, B;
}PIXEL;

//Hold the PPM Image
typedef struct PPM 
{
	char format[2];;
	NODE *comments;
    int w, h;
	int max;
	PIXEL * arr;
}PPM;

/**
 * function used to deepcopy the linkedlist holding the comments.
 * it returns a pointer to a deepcopy of the list passed as a parameter;
 */
//Copy the linked list
//Points to the copy
NODE *copy(NODE *first) 
{
	NODE *second = NULL, *previous = NULL;

	while (first != NULL) 
	{
		NODE * temp = (NODE *) malloc(sizeof(NODE));
		temp->val = first->val;
		temp->next = NULL;

		if (second == NULL) 
		{
			second = temp;
			previous = temp;
		} 
		else 
		{
			previous->next = temp;
			previous = temp;
		}
		first = first->next;
	}
	return second;
}


 //Copy the PPM File Returning Pointer
struct PPM * createPPM(PPM *old) 
{
	PPM *new = (PPM*) malloc(sizeof(PPM));

	strcpy(new->format, old->format); 
	new->comments = copy(old->comments); 

	new->h = old->h;
	new->w = old->w;
	new->max = old->max;

	new->arr = (PIXEL *) malloc(old->h * old->w * sizeof(PIXEL));
	memcpy(new->arr, old->arr, old->h * old->w * sizeof(PIXEL));
	
	return new;
}

/*Part B */


//Function to read a PPM file and store the structure
struct PPM * getPPM(FILE * fd) 
{
	PPM *image = (PPM *) malloc(sizeof(PPM));

	fscanf(fd, "%s", image->format);
	if (strcmp(image->format, "P3") != 0) 
	{
		printf("Invalid Image Type");
		exit(0);
	}
	char c = getc(fd);
	image->comments = (NODE *) malloc(sizeof(NODE));
	NODE *temp = image->comments;
	while ((c = getc(fd)) == '#') 
	{
		fseek(fd, -1, SEEK_CUR); 
		char str[50];
		fgets(str, 50, fd);
		temp->val = (char *) malloc(strnlen(str, 50));
		strcpy(temp->val, str);
		temp->val[strlen(temp->val) - 1] = 0;
		temp->next = (NODE *) malloc(sizeof(NODE));
		temp = temp->next;
		temp->next = NULL;
	}

	fseek(fd, -1, SEEK_CUR);
	fscanf(fd, "%d", &image->w);
	fscanf(fd, "%d", &image->h);
	fscanf(fd, "%d", &image->max);

	image->arr = (PIXEL *) malloc(image->h * image->w * sizeof(PIXEL));

	int t = 0;
	int j = 0;
	
	while (j < image->h * image->w)
   {
		t = fscanf(fd, "%d", &image->arr[j].R);
		t = fscanf(fd, "%d", &image->arr[j].G);
		t = fscanf(fd, "%d", &image->arr[j].B);
		j = j + 1;
	}

	return image;
}

//Encode The message into the PPM file
struct PPM * encode(char * text, struct PPM * i) 
{
	PPM * str = createPPM(i); 
	int random;
	srand((unsigned) time(NULL));
	int randMax = (i->h * i->w) / (strlen(text)+1);

	random = rand() % randMax; 
	
	if(random<1)
	{
		random = 1; 
	}
	
	int k =0;
	int j = random;
	
	//Red fields swapped with ASCII int
	while (k < strlen(text)) 
	{
		if(str->arr[j].R == text[k])
		{
			j = j+1; // if the values are the same we encode in the next pixel.
		}
		else
		{
		str->arr[j].R = text[k]; 
		k = k+1;
		j = j + random;
		}
	}

	return str;
}

//Function Decodes the messages comparing the new copy to the original
char * decode(struct PPM * i1, struct PPM * i2) 
{
	int i = 0;
	int j = 0;
	
	char *str = (char *) malloc(255);

	while (i < i1->h * i1->w)
	{
		if(i1->arr[i].R != i2->arr[i].R)
		{
		str[j] = i2->arr[i].R;
		j = j+1;
		}
		i = i + 1;
	}

	str = realloc(str,i);

	return str;
}

/**
 * function to print the PPM structure
 */
void showPPM(struct PPM * i) 
{
	printf("%s\n", i->format); //print format

	//print comments
	NODE *n = i->comments;
	while (n->next != NULL)
	{
		printf("%s\n", n->val);
		n = n->next;
	}

	//print width, height and max
	printf("%d %d\n%d\n", i->w, i->h, i->max);

	//print the array containing the pixels
	int j;
	for (j = 0; j < i->h * i->w; ++j) 
	{
		printf("%d %d %d\n", i->arr[j].R, i->arr[j].G, i->arr[j].B);
	}

	return;
}

int main (int argc, char ** argv)
{
	
	if(argc != 3 && argc != 4)
	{ 
		printf("Encode 3 arguments, Decode 4\n");
		exit(0);
	}
	
	else if(argc ==3)
	{	
		if(strncmp(argv[1], "e",1) != 0)
		{ 
			printf("non valid commands %s for %d arguments\n", argv[1], argc);
			exit(0);
		}
		
		FILE * f = fopen(argv[2], "r");
		PPM * ppm1 = getPPM(f);
		fclose(f);
		char * toEnc =(char*) malloc(255 * sizeof(char));
		fgets (toEnc, 255, stdin);
		PPM * encoded = encode(toEnc, ppm1);
		showPPM(encoded);
	}
	
	else
	{
		if(strncmp(argv[1], "d",1) != 0)
		{
			printf("non valid commands %s for %d arguments\n", argv[1], argc);
			exit(0);	
		}
		FILE * file1 = fopen(argv[2], "r");
		FILE * file2 = fopen(argv[3], "r");
		PPM * ppm1 = getPPM(file1);
		PPM * ppm2 = getPPM(file2);
		char * dec = decode(ppm1, ppm2);
		printf("%s", dec); 
	}

}
