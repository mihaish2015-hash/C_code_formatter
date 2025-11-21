#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NMAX 1000

void insert_char(int lin, int pos, char **mat, char c)
{
    int l=strlen(mat[lin])+1;
    for(int i=l;i>pos;i--)mat[lin][i]=mat[lin][i-1];
    mat[lin][pos]=c;
}

void remove_char(int lin, int pos, char **mat)
{
    int l=strlen(mat[lin]);
    for(int i=pos;i<l;i++)mat[lin][i]=mat[lin][i+1];
}

void remove_line(int lin, int *nr_lines, char **mat)
{
    (*nr_lines)--;
    for(int i=lin;i<*nr_lines;i++)strcpy(mat[i],mat[i+1]);
}

void insert_line(int lin, int *nr_lines, char **mat, char *arr)
{
    (*nr_lines)++;
    for(int i=*nr_lines;i>lin;i--)strcpy(mat[i],mat[i-1]);
    strcpy(mat[lin],arr);
}

void space(int n, char **mat)
{
    char operators[]="<>=&+-*/%^&|";
    char no_spaces_before[]=",;[])";
    char no_spaces_after[]="[(";
    char arr[]="+-";
    for (int i = 0; i < n; i++)
    {
        int j=0, k=-1;
        while(*(mat[i]+j))
        {
            if(strchr(no_spaces_after,mat[i][j]) || (mat[i][j]=='!' && mat[i][j+1]!='='))
            {
                if(mat[i][j]=='[' && mat[i][j-1]==' '){
                    remove_char(i,j-1,mat);
                    j--;
                }
                if(mat[i][j+1]==' ')remove_char(i,j+1,mat);
            }
            else if(strchr(no_spaces_before,mat[i][j]) || (strchr(arr,mat[i][j]) && mat[i][j]==mat[i][j+1])) 
            {
                if(mat[i][j-1]==' '){
                    remove_char(i,j-1,mat);
                    j--;
                }
                if(strchr(arr,mat[i][j]) && mat[i][j]==mat[i][j+1])j++;
                if(mat[i][j]==',' && mat[i][j+1]!=' ')insert_char(i,j+1,mat,' ');
            }
            else if(mat[i][j]=='!' && mat[i][j+1]=='=')
            {
                if(mat[i][j-1]!= ' '){
                    insert_char(i,j,mat,' ');
                    j++;
                }
                j++;
                if(mat[i][j+1]!=' ')insert_char(i,j+1,mat,' ');
            }
            else if(strchr(operators,mat[i][j]) && !strstr(mat[i],"include"))
            {
                if(mat[i][j-1]!= ' '){
                    insert_char(i,j,mat,' ');
                    j++;
                }
                if(strchr("=&|+-<>",mat[i][j+1]))j++;
                if(mat[i][j+1]!=' ')insert_char(i,j+1,mat,' ');
            }
            j++;
        }
    }
}

void enter(int *n, char **mat)
{
    char *loop[]={"for","if","else","while","do"};
    for(int i=0;i<*n;i++)
        for(int j=0;j<5;j++){
            char *a=strstr(mat[i],loop[j]);
            if(a)
                for(int k=0;k<5;k++){
                    char *p=strstr(a+1,loop[k]);
                    if(p && p!=a)
                    {
                        int l=p-mat[i];
                        insert_line(i+1,n,mat,p);
                        while(mat[i][--l]==' ')remove_char(i,l,mat);
                        mat[i][l+1]='\n';
                        mat[i][l+2]='\0';
                        break;
                    }
                }
        }
}

void indent(int n, char **mat)
{
    int tab[NMAX],s[NMAX],size=0;
    char *loop[]={"for","if","else","while","do"};
    for(int i=0;i<n;i++)
    {
        int val=0;
        for(int j=0;j<5;j++)
        {
            char *a=strstr(mat[i],loop[j]);
            if(a)
            {
                int br=0;
                val=1;
                if(strchr(mat[i],'{'))br=1;
                if(strchr(mat[i+1],'{'))
                {
                    int ok=1;
                    for(int k=0;k<5;k++)if(strstr(mat[i+1],loop[k]))ok=0;
                    if(ok)br=2;
                }
                if(strchr(mat[i],'}'))
                {
                   while(size && !s[size])size--;
                    tab[i]=--size; 
                    s[++size]=!(!br);
                }
                else{
                    tab[i]=size;
                    s[++size]=!(!br);
                }
            }
        }
        if(!val)
        {
            if(strchr(mat[i],'}'))
            {
                while(size && !s[size])size--;
                tab[i]=--size; 
            }
            else if(strchr(mat[i],'{'))s[++size]=1;
            else{
                tab[i]=size;
                while(size && !s[size])size--;
            }
        }
    }
    for(int i=0;i<n;i++)
    {
        int j=0,nrtab=0;
        while(mat[i][j++]=='\t')nrtab++;
        if(nrtab<tab[i])
            for(int k=0;k<tab[i]-nrtab;k++)insert_char(i,0,mat,'\t');
        else if(nrtab>tab[i])
            for(int k=0;k<nrtab-tab[i];k++)remove_char(i,0,mat);
    }
}

void loops(int *n, char **mat)
{
    char operators[]="<>=&+-*/%^&|";
    char *loop[]={"for","if","else","while","do"};
    for(int i=0;i<*n;i++)
    {
        for(int j=0;j<5;j++){
            char *a=strstr(mat[i],loop[j]);
            if(a)
            {
                int br=0;
                if(strchr(mat[i],'{'))br=1;
                if(strchr(mat[i+1],'{'))
                {
                    int ok=1;
                    for(int k=0;k<5;k++)if(strstr(mat[i+1],loop[k]))ok=0;
                    if(ok)br=2;
                }
                int k=0;
                if(strchr(mat[i],'('))
                    k=strchr(mat[i],'(')-mat[i];
                if(k && mat[i][k-1]!=' ')insert_char(i,k,mat,' ');
                if(br==2){
                    remove_line(i+1,n,mat);
                    int k=a-mat[i];
                    while(mat[i][k]>='a' && mat[i][k]<='z')k++;
                    char *ptr=mat[i]+k;
                    while(strchr(ptr,')'))
                        ptr=strchr(ptr,')')+1;
                    k=ptr-mat[i];
                    insert_char(i,k,mat,' ');
                    insert_char(i,k+1,mat,'{');
                }
                if(br==1)
                {
                    char *ptr=strchr(mat[i],'{');
                    int k=ptr-mat[i];
                    if(mat[i][k-1]!=' ')insert_char(i,k,mat,' ');
                }
                if(strchr(mat[i],';'))
                {
                    if(strstr(mat[i],"for"))
                    {
                        char *p=strchr(mat[i],';');
                        while(strchr(p,';')){
                            int l=strchr(p,';')-mat[i];
                            if(mat[i][l+1]!=' ')insert_char(i,l+1,mat,' ');
                            p=strchr(p,';')+1;
                        }
                    }
                    char *ptr=strchr(mat[i],')')+1;
                    while(strchr(ptr,')'))
                    ptr=strchr(ptr,')')+1;
                    int found=0;
                    int kk=ptr-mat[i];
                    for(int y=0;y<12;y++)if(strchr(mat[i]+kk,operators[y]))found=1;
                    if(found)
                    {
                        insert_line(i+1,n,mat,ptr);
                        mat[i][kk]='\n';
                        mat[i][kk+1]='\0';
                    }
                }
                char *p1=strstr(mat[i],"else");
                if(p1)
                {
                    int ind=p1-mat[i],f=0;
                    for(int x=0;x<ind && !f;x++)if(mat[i][x]=='}')f=1;
                    if(!f){
                        remove_line(i-1,n,mat);
                        while(mat[i-1][0]!='e')remove_char(i-1,0,mat);
                        insert_char(i-1,0,mat,' ');
                        insert_char(i-1,0,mat,'}');
                        i--;
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int nrlin=0;

    //dinamically allocate memory for the matrix of characters
    char **mat=malloc(NMAX*sizeof(char *));
    for (int i = 0;i < NMAX;i++)mat[i]=malloc(NMAX*sizeof(char));

    //read the matrix from the file given as argument
    FILE *f=fopen(argv[1],"r");
    while(fgets(mat[nrlin],NMAX,f))nrlin++;

    //style the matrix by giving its adress to the following functions
    for(int i=0;i<5;i++)
    {
        space(nrlin, mat);
        enter(&nrlin,mat);
        loops(&nrlin, mat);
        indent(nrlin, mat);
    }

    //display the new matrix of characters
    for(int i =0;i<nrlin;i++)fprintf(stdout,"%s",mat[i]);

    //free the memory allocated for the matrix
    for(int i=0;i<NMAX;i++)free(mat[i]);
    free(mat);
}
