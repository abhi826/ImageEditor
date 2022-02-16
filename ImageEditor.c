#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
    unsigned int width;
    unsigned int height;
    unsigned char** arr;
};
void deallocateMem(struct Node* node){
    unsigned char ** arr=node->arr;
    unsigned int height=node->height;
    unsigned int i=0;
    for(i=0;i<height;i++){
        free(arr[i]);
    }
    free(arr);
    
    


}
void skipWhiteSpace(FILE* fp){
    int ch;
    while((ch=fgetc(fp))!=EOF&&isspace(ch)){
        ;
    }
    fseek(fp,-1,SEEK_CUR);
}
void processFile(struct Node* node, const char* inputFile){
    char charSeq[3];
    char maxSeq[4];
    unsigned int i=0,j=0;
     FILE* fp=fopen(inputFile,"rb");
     skipWhiteSpace(fp);
     fgets(charSeq,sizeof(charSeq),fp);
     skipWhiteSpace(fp);
     fscanf(fp,"%u %u",&(node->width),&(node->height));
     skipWhiteSpace(fp);
     fgets(maxSeq,sizeof(maxSeq),fp);
     fgetc(fp); 
     node->arr=malloc(sizeof(unsigned char*)*node->height);
     for(i=0;i<node->height;i++){
         (node->arr)[i]=malloc(sizeof(unsigned char)*node->width);
          
     }
     for(i=0;i<node->height;i++){
         for(j=0;j<node->width;j++){
             node->arr[i][j]=fgetc(fp);
         }

     }
     fclose(fp);


}
void writeFile(struct Node* node, const char* outputFile){
    FILE* fp=fopen(outputFile,"wb");
    unsigned int i=0,j=0;
    fprintf(fp,"P5\n");
    fprintf(fp,"%d %d\n",node->width,node->height);
    fprintf(fp,"255\n");
    for(i=0;i<node->height;i++){
        for(j=0;j<node->width;j++){
            fputc(node->arr[i][j],fp);
        }
    }
    fclose(fp);
    
}

void monochrome(struct Node* node){
    unsigned int row=node->height;
    unsigned int col=node->width;
    unsigned int i=0,j=0;
    for(i=0;i<row;i++){
        for(j=0;j<col;j++){
            unsigned char pixel=node->arr[i][j];
            if((pixel & 0xC0)!=0){
                node->arr[i][j]=0xFF;
            }
            else{
                node->arr[i][j]=0x00;
            }

        }
    }
}
void negative(struct Node* node){
    unsigned int row=node->height;
    unsigned int col=node->width;
    unsigned int i=0,j=0;
    for(i=0;i<row;i++){
        for(j=0;j<col;j++){
            node->arr[i][j]=~(node->arr[i][j]);
        }
    }
}
void darken(struct Node* node){
    unsigned int row=node->height;
    unsigned int col=node->width;
    unsigned int i=0,j=0;
    for(i=0;i<row;i++){
        for(j=0;j<col;j++){
            node->arr[i][j]=(node->arr[i][j])>>1;
        }
    }

}
void flipVertical(struct Node* node){
    unsigned int row=node->height;
    unsigned int col=node->width;
    unsigned int j=0;
    for(j=0;j<col;j++){
        unsigned int tPointer=0;
        unsigned int bPointer=row-1;
        while(tPointer<bPointer){
            unsigned char temp=node->arr[tPointer][j];
            node->arr[tPointer][j]=node->arr[bPointer][j];
            node->arr[bPointer][j]=temp;
            tPointer++;
            bPointer--;
        }
    }
}
void flipHorizontal(struct Node* node){
    unsigned int row=node->height;
    unsigned int col=node->width;
    unsigned int i=0;
    for(i=0;i<row;i++){
        unsigned int lPointer=0;
        unsigned int rPointer=col-1;
        while(lPointer<rPointer){
            unsigned char temp=node->arr[i][lPointer];
            node->arr[i][lPointer]=node->arr[i][rPointer];
            node->arr[i][rPointer]=temp;
            lPointer++;
            rPointer--;
        }
    }
}
int cmpfunc (const void * a, const void * b) {
   return ( *(unsigned char*)a - *(unsigned char*)b );
}
void sort(struct Node* node){
    unsigned int row=node->height;
    unsigned int col=node->width;
    unsigned int i=0;
    for(i=0;i<row;i++){
        qsort(node->arr[i],col,sizeof(unsigned char),cmpfunc);
    }

}

void blur(struct Node* node){
    unsigned int row=node->height;
    unsigned int col=node->width;
    unsigned int i=0;
    unsigned int j=0;
    unsigned char** blurArr= malloc(sizeof(unsigned char*)*row);

    for(i=0;i<row;i++){
        blurArr[i]=malloc(sizeof(unsigned char)*col);
    }
    for(i=0;i<row;i++){
        for(j=0;j<col;j++){
            if(i==0&&j==0){
                blurArr[i][j]=(node->arr[i][j]+node->arr[i+1][j]+node->arr[i][j+1]+node->arr[i+1][j+1])/4;

            }
            
           else if(i==0&&j==col-1){

               blurArr[i][j]=(node->arr[i][j]+node->arr[i][j-1]+node->arr[i+1][j]+node->arr[i+1][j-1])/4;

           }
           
            else if(i==row-1&&j==0){
                blurArr[i][j]=(node->arr[i][j]+node->arr[i-1][j]+node->arr[i][j+1]+node->arr[i-1][j+1])/4;

            }
            
            else if(i==row-1&&j==col-1){
                blurArr[i][j]=(node->arr[i][j]+node->arr[i][j-1]+node->arr[i-1][j]+node->arr[i-1][j-1])/4;

            }
            else if(j==0){
                blurArr[i][j]=(node->arr[i][j]+node->arr[i][j+1]+node->arr[i+1][j]+node->arr[i+1][j+1]+node->arr[i-1][j]+node->arr[i-1][j+1])/6;

            }
            else if(j==col-1){
                blurArr[i][j]=(node->arr[i][j]+node->arr[i][j-1]+node->arr[i-1][j]+node->arr[i-1][j-1]+node->arr[i+1][j]+node->arr[i+1][j-1])/6;
                
            }
            
            else if(i==0){
                
                blurArr[i][j]=(node->arr[i][j]+node->arr[i][j-1]+node->arr[i][j+1]+node->arr[i+1][j]+node->arr[i+1][j-1]+node->arr[i+1][j+1])/6;
            }
            
            
            else if(i==row-1){
                blurArr[i][j]=(node->arr[i][j]+node->arr[i][j-1]+node->arr[i][j+1]+node->arr[i-1][j]+node->arr[i-1][j-1]+node->arr[i-1][j+1])/6;

            }
            
            else{
                blurArr[i][j]=(node->arr[i][j]+node->arr[i-1][j]+node->arr[i-1][j-1]+node->arr[i-1][j+1]+node->arr[i][j+1]+node->arr[i][j-1]+node->arr[i+1][j]+node->arr[i+1][j-1]+node->arr[i+1][j+1])/9;

            }
        
            
        }
        
        
    }
    deallocateMem(node);
    node->arr=blurArr;


}

void rotateRight(struct Node* node){
    unsigned int width=node->width;
    unsigned int height=node->height;
    unsigned int newWidth=node->height;
    unsigned int newHeight=node->width;
    unsigned char** newArr= malloc(sizeof(unsigned char*)*newHeight);
    unsigned int i=0;
    unsigned int r=0;
    unsigned int c=0;
    for(i=0;i<newHeight;i++){
        newArr[i]=malloc(sizeof(unsigned char)*newWidth);
    }
    for(r=0;r<height;r++){
        for(c=0;c<width;c++){
            newArr[c][height-1-r]=(node->arr)[r][c];
        }
    }
    deallocateMem(node);
    node->arr=newArr;
    node->height=newHeight;
    node->width=newWidth;

}
void rotateLeft(struct Node* node){
    unsigned int width=node->width;
    unsigned int height=node->height;
    unsigned int newWidth=node->height;
    unsigned int newHeight=node->width;
    unsigned char** newArr= malloc(sizeof(unsigned char*)*newHeight);
    unsigned int i=0;
    unsigned int r=0;
    unsigned int c=0;
    for(i=0;i<newHeight;i++){
        newArr[i]=malloc(sizeof(unsigned char)*newWidth);
    }
    for(r=0;r<height;r++){
        for(c=0;c<width;c++){
            newArr[width-c-1][r]=(node->arr)[r][c];
        }
    }
    deallocateMem(node);
    node->arr=newArr;
    node->height=newHeight;
    node->width=newWidth;

}


int main(int argc,char** argv){
        struct Node* node= malloc(sizeof(struct Node));
        const char * inputFile=*(argv+1);
        const char* outputFile="output.pgm";
        int choice;
        (void)argc;
        processFile(node,inputFile);
        printf("Options menu:\n");
        printf(" 1. Rotate Left (counter-clockwise)\n");
        printf(" 2. Rotate Right (clockwise)\n");
        printf(" 3. Flip Vertical\n");
        printf(" 4. Flip Horizontal\n");
        printf(" 5. Convert to Monochrome (black and white)\n");
        printf(" 6. Negative (invert image)\n");
        printf(" 7. Blur\n");
        printf(" 8. Darken\n");
        printf(" 9. Sort Each Row (ascending)\n");
        printf("10. Save Changes and Quit\n\n");
        
        
        
    
        while(1){
            printf("Your choice? ");
            scanf("%d",&choice);
            if(choice==1){
                rotateLeft(node);

            }
            else if(choice==2){
                rotateRight(node);
                
            }
            else if(choice==3){
                flipVertical(node);

            }
            else if(choice==4){
                flipHorizontal(node);

            }
            else if(choice==5){
                monochrome(node);

            }
            else if(choice==6){
                negative(node);

            }
            else if(choice==7){
                blur(node);

            }
            else if(choice==8){
                darken(node);

            }
            else if(choice==9){
                sort(node);

            }
            else if(choice==10){
                writeFile(node,outputFile);
                break;

            }
        }
        deallocateMem(node);
        free(node);
         
         return 0;

}
