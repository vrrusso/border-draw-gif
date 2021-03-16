#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "gifenc.h"
//estrutura que armazena os pontos iniciais da segmentação
typedef struct initial_point
{
    int x;
    int y;
    int max_diff;
    int sum;
    int depth;
}InitialPoint;
typedef struct pixel
{
    int intensity;
    int seg_region;
}Pixel;
int ReadInput(char ** file_name,InitialPoint ** ipoints);
int * ReadFile(int n_searches,char * file_name,Pixel *** image);
void TerminateDoublePointer(void ** ptr, int n);
void PrintBorders(int * measures,Pixel * *image);
int Searching(int * measures,Pixel ** image,InitialPoint * ipoint,int seg_index,int x,int y);//seek and destroy
void Terminate(void * ptr);
void Terminate(void * ptr)
{
    free(ptr);
}
void TerminateDoublePointer(void ** ptr,int n)
{
    for(int i=0;i<n;i++)
        Terminate(ptr[i]);
    free(ptr);
}
void PrintBorders(int * measures,Pixel **image)
{
	ge_GIF *gif  = ge_new_gif("borders.gif", measures[0],measures[1],
			(uint8_t []){
				0x00, 0x00, 0x00,
				0xFF, 0xFF, 0xFF,
			},
			2,0);
	/*for(int i=0;i<measures[0]*measures[1];i++)
		gif->frame[i]=1;
	ge_add_frame(gif,20);*/
		int k=0;
		for(int i=0;i<measures[1];i++)
		{
			for(int j=0;j<measures[0];j++)
			{
				unsigned char flag=1;
				if(i>0)
				{
					if(image[i][j].seg_region!=image[i-1][j].seg_region&&flag)
					{
						gif->frame[k]=0;
						flag=0;
					}
				}
				if(j>0)
				{
					if(image[i][j].seg_region!=image[i][j-1].seg_region&&flag)
					{
						gif->frame[k]=0;
						flag=0;
					}
				}
				if(i<measures[1]-1)
				{
					if(image[i][j].seg_region!=image[i+1][j].seg_region&&flag)
					{
						gif->frame[k]=0;
						flag=0;
					}
				}
				if(j<measures[0]-1)
				{
					if(image[i][j].seg_region!=image[i][j+1].seg_region&&flag)
					{
						gif->frame[k]=0;
						flag=0;
					}
				}
				if(flag)
					gif->frame[k]=1;
				k++;
			}
			ge_add_frame(gif,4);
		}
	ge_close_gif(gif);
}
int Searching(int * measures,Pixel ** image,InitialPoint * ipoint,int seg_index,int x,int y)
{
    if(image[x][y].seg_region!=-1)
        return 1;
    image[x][y].seg_region=seg_index;
    (*ipoint).depth++;
    (*ipoint).sum+=image[x][y].intensity;
    if(x-1>=0)
    {
        double diff=fabs(((double)image[x-1][y].intensity-((double)(*ipoint).sum/(double)(*ipoint).depth)));
        if(image[x-1][y].seg_region==-1&&diff<=(double)(*ipoint).max_diff)
            Searching(measures,image,ipoint,seg_index,x-1,y);
    }
    if(y+1<measures[0])
    {
        double diff=fabs(((double)image[x][y+1].intensity-((double)(*ipoint).sum/(double)(*ipoint).depth)));
        if(image[x][y+1].seg_region==-1&&diff<=(double)(*ipoint).max_diff)
            Searching(measures,image,ipoint,seg_index,x,y+1);
    }
    if(x+1<measures[1])
    {
        double diff=fabs(((double)image[x+1][y].intensity-((double)(*ipoint).sum/(double)(*ipoint).depth)));
        if(image[x+1][y].seg_region==-1&&diff<=(double)(*ipoint).max_diff)
            Searching(measures,image,ipoint,seg_index,x+1,y);
    }
    if(y-1>=0)
    {
        double diff=fabs(((double)image[x][y-1].intensity-((double)(*ipoint).sum/(double)(*ipoint).depth)));
        if(image[x][y-1].seg_region==-1&&diff<=(double)(*ipoint).max_diff)
            Searching(measures,image,ipoint,seg_index,x,y-1);
    }
    return 1;
}
//função que le a entrada de dados e armazena em variáveis
int ReadInput(char ** file_name, InitialPoint ** ipoints)
{
    int n_searches;
    if(((*file_name)=(char *)malloc(25*sizeof(char)))==NULL)
    {
        printf("ERROR");
        exit(1);
    }
    scanf("%s ",(*file_name));
    scanf("%d",&n_searches);
    if(((*ipoints)=(InitialPoint *)malloc(n_searches*sizeof(InitialPoint)))==NULL)
    {
        printf("ERROR");
        exit(1);
    }
    for(int i=0;i<n_searches;i++)
    {
        scanf("%d %d %d",&((*ipoints)[i].x),&((*ipoints)[i].y),&((*ipoints)[i].max_diff));
        (*ipoints)[i].sum=0;
        (*ipoints)[i].depth=0;
    }
    return n_searches;
}
int * ReadFile(int n_searches,char * file_name,Pixel *** image)
{
    FILE *fp;
    if((fp=fopen(file_name,"r"))==NULL)
    {
        printf("ERROR");
        exit(1);
    }
    char type[20];
    fscanf(fp,"%s",type);
    if(strcmp(type,"P2")!=0)
    {
        printf("ERROR");
        exit(1);
    }
    int * measures;
    measures=(int *)malloc(2*sizeof(int));
    fscanf(fp,"%d %d",&measures[0],&measures[1]);
    fscanf(fp,"%*d");
    if(((*image)=(Pixel **)malloc(measures[1]*sizeof(Pixel*)))==NULL)
    {
        printf("ERROR");
        exit(1);
    }
    for(int i=0;i<measures[1];i++)
    {
        if(((*image)[i]=(Pixel *)malloc(measures[0]*sizeof(Pixel)))==NULL)
        {
            printf("ERROR");
            exit(1);
        }
        for(int j=0;j<measures[0];j++)
        {
            fscanf(fp,"%d",&(*image)[i][j].intensity);
            (*image)[i][j].seg_region=-1;
        }
    }
    fclose(fp);
    return measures;
}
int main(void)
{
    char * file_name=NULL;
    int n_searches;
    int * measures;
    Pixel ** image;
    InitialPoint *ipoints;
    n_searches=ReadInput(&file_name,&ipoints);
    measures=ReadFile(n_searches,file_name,&(image));
    for(int i=0;i<n_searches;i++)
        Searching(measures,image,&ipoints[i],i,ipoints[i].x,ipoints[i].y);
    /*for (int i = 0; i < measures[1]; i++)
    {
        for (int j = 0; j < measures[0];j++)
        {
            printf("%3d",image[i][j].seg_region);
        }
        printf("\n");
        
    }*/
    PrintBorders(measures,image);
    Terminate(file_name);
    Terminate(ipoints);
    TerminateDoublePointer((void **)image,measures[1]);
    Terminate(measures);
    return 0;
}
