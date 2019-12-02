/*
             ,----------------,              ,---------,
        ,-----------------------,          ,"        ,"|
      ,"                      ,"|        ,"        ,"  |
     +-----------------------+  |      ,"        ,"    |
     |  .-----------------.  |  |     +---------+      |
     |  |                 |  |  |     | -==----'|      |
     |  |  Trabalho 3     |  |  |     |         |      |
     |  |  ICC            |  |  |/----|`---=    |      |
     |  |  Victor Russo   |  |  |   ,/|==== ooo |      ;
     |  |  11218855       |  |  |  // |(((( [33]|    ,"
     |  `-----------------'  |," .;'| |((((     |  ,"
     +-----------------------+  ;;  | |         |,"     
        /_)______________(_/  //'   | +---------+
   ___________________________/___  `,
  /  oooooooooooooooo  .o.  oooo /,   \,"-----------
 / ==ooooooooooooooo==.o.  ooo= //   ,`\--{)B     ,"
/_==__==========__==_ooo__ooo=_/'   /___________,"



*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
//estrutura que armazena os pontos iniciais para as buscas pela segmentação, contendo as coordenadas, o critério, soma das intensidades
//de tal regiao e a quantidade de pixel em dada região
typedef struct initial_point
{
    int x;
    int y;
    int max_diff;
    int sum;
    int depth;
}InitialPoint;
//estrutura que armazena os pixels da imagem, sendo image[x][y] o pixel de coordenada x e y||||armazena a intensidade e a qual região
//de segmentação o pixel pertence
typedef struct pixel
{
    int intensity;
    int seg_region;
}Pixel;

//protótipo das funções
int ReadInput(char ** file_name,InitialPoint ** ipoints);
int * ReadFile(int n_searches,char * file_name,Pixel *** image);
void TerminateDoublePointer(void ** ptr, int n);
void PrintBorders(int * measures,Pixel * *image);
int Searching(int * measures,Pixel ** image,InitialPoint * ipoint,int seg_index,int x,int y);//seek and destroy
void Terminate(void * ptr);

//função para liberar ponteiros do tipo *
void Terminate(void * ptr)
{
    free(ptr);
}

//funcao para liberar ponteiros do tipo * *
void TerminateDoublePointer(void ** ptr,int n)
{
    for(int i=0;i<n;i++)
        Terminate(ptr[i]);
    free(ptr);
}
//função que le a entrada de dados e armazena o nome da imagem a ser tratada,o numero de busca 
//,e os pontos iniciais armazenados na estrutura ipoints
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
//função que le a imagem, armazenando a intensidade dos pixels na estrutura image, além de iniciar o segmento a qual 
//os pixels pertencem como -1||retorna as dimensões da imagem na variavel measures onde measures[0]=width measures[1]=height
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
//funcao de processamento da imagem, busca recurssivamente os pontos que pertencem a tal segmento
//alterando seu valor seg_region de acordo com seg_index
//após passar por essa função a image estará segmentada em diferentes regiões
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
//função que imprime os pontos de borda, conferindo os 4 vizinhos e imprimndo no formato certo quando necessário
void PrintBorders(int * measures,Pixel **image)
{
    for(int i=0;i<measures[1];i++)
    {
        for(int j=0;j<measures[0];j++)
        {
            unsigned char flag=1;
            if(i>0)
            {
                if(image[i][j].seg_region!=image[i-1][j].seg_region&&(flag==1))
                {
                    printf("(%d, %d)\n",i,j);
                    flag=0;
                }
            }
            if(j>0)
            {
                if(image[i][j].seg_region!=image[i][j-1].seg_region&&(flag==1))
                {
                    printf("(%d, %d)\n",i,j);
                    flag=0;
                }
            }
            if(i<(measures[1]-1))
            {
                if(image[i][j].seg_region!=image[i+1][j].seg_region&&(flag==1))
                {
                    printf("(%d, %d)\n",i,j);
                    flag=0;
                }
            }
            if(j<(measures[0]-1))
            {
                if(image[i][j].seg_region!=image[i][j+1].seg_region&&(flag==1))
                {
                    printf("(%d, %d)\n",i,j);
                    flag=0;
                }
            }
        }
    }
}
//função main, com o fluxo principal do programa
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
    PrintBorders(measures,image);
    Terminate(file_name);
    Terminate(ipoints);
    TerminateDoublePointer((void **)image,measures[1]);
    Terminate(measures);
    return 0;
}