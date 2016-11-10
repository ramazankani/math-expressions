//Ramazan Kani Türkmen

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//for variables and and postfix linked lists
struct list{
  char *str;
  struct list *next,*prev;
};
typedef struct list node;
typedef enum{true=1,false=0} bool;
//to control two string equal or not
bool isEqual(char *x,char *y){
  int test=0,i;
  if(!strlen(x)==strlen(y)) return false;
  else{
    for (i=0;i<strlen(x);i++){
      if(!x[i]==y[i]) test++;
    }
    if(test==0) return true;
    else return false;
  }
}
//to test integer or not
bool isInteger(char *exp){
  int i,num=strlen(exp);
  for(i=0;exp[i];i++){
    if(isdigit(exp[i])) num--;
  }
  if(num==0){
    return true;
  }else return false;
}
//to test variable or not
bool isVariable(char *exp){
  if(strlen(exp)==0) return false;
  else if(!isalpha(exp[0])) return false;
  else true;
}
//to add element for postfix linked list
void add_element_stack(node *root_stack,char *factor){
  while(root_stack->next!=NULL){
    root_stack=root_stack->next;
  }
  root_stack->next=(node *)malloc(sizeof(node));
  root_stack->next->str=factor;
  root_stack->next->next=NULL;
  root_stack->next->prev=root_stack;
}
//to parse expression for postfix stack
void parser(node *root_stack,node *root_variable,char *str){
  char *exp=malloc(100);
  strcpy(exp,str);
  int i,len=0,numOfParser=0,numOfBracket=0;
  for(i=0;exp[i];i++){
    len++;
  }
  for(i=len-1;i>=0;i--){
    if(exp[i]==')') numOfBracket++;
    if(exp[i]=='(') numOfBracket--;
    if(numOfBracket==0){
      if(exp[i]=='+'){
        numOfParser++;
        exp[i]='\0';
        add_element_stack(root_stack,"+");
        if(strlen(exp+i+1)==0){
          printf("ERROR..Wrong input");
          exit(1);
        }
        parser(root_stack,root_variable,exp);
        parser(root_stack,root_variable,exp+i+1);
        return;
      }
      if(exp[i]=='-'){
        numOfParser++;
        exp[i]='\0';
        add_element_stack(root_stack,"-");
        if(strlen(exp+i+1)==0){
          printf("ERROR..Wrong input");
          exit(1);
        }
        parser(root_stack,root_variable,exp);
        parser(root_stack,root_variable,exp+i+1);
        return;
      }
    }
  }
  if(numOfParser==0){
    for(i=len-1;i>=0;i--){
      if(exp[i]==')') numOfBracket++;
      if(exp[i]=='(') numOfBracket--;
      if(numOfBracket==0){
        if(exp[i]=='*'){
          numOfParser++;
          exp[i]='\0';
          add_element_stack(root_stack,"*");
          if((strlen(exp+i+1)==0)||(strlen(exp))==0){
            printf("ERROR..Wrong input");
            exit(1);
          }
          parser(root_stack,root_variable,exp);
          parser(root_stack,root_variable,exp+i+1);
          return;
        }
        if(exp[i]=='/'){
          numOfParser++;
          exp[i]='\0';
          add_element_stack(root_stack,"/");
          if((strlen(exp+i+1)==0)||(strlen(exp))==0){
            printf("ERROR..Wrong input");
            exit(1);
          }
          parser(root_stack,root_variable,exp);
          parser(root_stack,root_variable,exp+i+1);
          return;
        }
      }
    }
  }
  if(numOfParser==0){
    if(exp[len-1]==')'&&exp[0]=='('){
      exp[len-1]='\0';
      if(strlen(exp+1)==0){
        printf("ERROR..Wrong input");
        exit(1);
      }
      parser(root_stack,root_variable,exp+1);
      return;
    }else{
      if(strlen(exp)==0){
        add_element_stack(root_stack,"0");
        return;
      }else if(isInteger(exp)){
        add_element_stack(root_stack,exp);
        return;
      }else if(isVariable(exp)){
        add_element_stack(root_stack,exp);
        return;
      }else{
        printf("ERROR...Wrong type variable or integer");
        exit(1);
      }
    }
  }
}
//to change postfix linked list to assembly instructions
void write_inst(FILE *filw,node *root_stack,node *root_variable){
  node *iter=root_variable;
  while(root_stack->next!=NULL){
    root_stack=root_stack->next;
  }
  while(root_stack->str!=NULL){
    if(isInteger(root_stack->str)){
      fprintf(filw," push %sd\n",root_stack->str);
    }else if(root_stack->str=="+"){
      fputs(" pop ax\n pop bx\n add ax,bx\n push ax\n",filw);
    }else if(root_stack->str=="-"){
      fputs(" pop ax\n pop bx\n sub ax,bx\n push ax\n",filw);
    }else if(root_stack->str=="*"){
      fputs(" pop cx\n pop ax\n mul cx\n push ax\n",filw);
    }else if(root_stack->str=="/"){
      fputs(" mov dx,0\n pop ax\n pop cx\n div cx\n push ax\n",filw);
    }else if(isVariable(root_stack->str)){
      int a=0;root_variable=iter;
      while(root_variable->next!=NULL){
        if(root_variable->next->str[0]==root_stack->str[0]){
          a++;
          fprintf(filw," mov bx,%s\n",root_variable->next->str);
          fputs(" push [bx]\n",filw);
          break;
        }
        root_variable=root_variable->next;
      }
      if(a==0){puts("ERROR..Undefined Variable");exit(1);}
    }else { puts("ERROR"); exit(1); }
    root_stack=root_stack->prev;
    root_stack->next=NULL;
  }
}
//to add variable's name to linked list
void add_element_variable_compiler(node *root_variable,char *factor){
  while(root_variable->next!=NULL){
    if(isEqual(root_variable->next->str,factor)){
      return;
    }
    root_variable=root_variable->next;
  }
  root_variable->next=(node *)malloc(sizeof(node));
  root_variable->next->str=factor;
  root_variable->next->next=NULL;
  root_variable->next->prev=root_variable;
}
//to parse compiler first for '=' expression
void parser_statement_compiler(FILE *filw,node *root_stack,node *root_variable,char *str){
  char *exp=malloc(200);
  int i,j,num=0,len=strlen(str);

  for(j=0,i=0;i<strlen(str);i++)
    if((!isspace(str[i]))&&str[i]!='\n'){
      exp[j]=str[i];j++;
    }
    exp[j]='\0';

  for(i=0;exp[i];i++){
    if(exp[i]=='='){
      num++;
      exp[i]='\0';
      if(isVariable(exp)){
        add_element_variable_compiler(root_variable,exp);
        parser(root_stack,root_variable,exp+i+1);
        write_inst(filw,root_stack,root_variable);
        fprintf(filw," pop ax\n mov bx,%s\n mov [bx],ax\n",exp);
      }else{
        printf("ERROR...Wrong type variable or integer");
        exit(1);
      }
    }
  }
  if(num==0){
    parser(root_stack,root_variable,exp);
    write_inst(filw,root_stack,root_variable);
    fputs(" pop ax\n call myprint\n",filw);
  }
}
//to write print function in assembly
void write_var(node *root_variable,FILE *filw){
  fputs(" int 20h \nmyprint:\n mov si,10d\n xor dx,dx\n push ' '\n",filw);
  fputs(" mov cx,1d\n mov bx,8000h\n and bx,ax\n shr bx,15\n jz positive\n",filw);
  fputs("negative:\n not ax\n inc ax\npositive:\n div si\n add dx,48d\n",filw);
  fputs(" push dx\n inc cx\n xor dx,dx\n cmp ax,0h\n jne positive\n",filw);
  fputs(" dec bx\n jnz writeloop\n push '-'\n inc cx\nwriteloop:\n",filw);
  fputs(" pop dx\n mov ah,02h\n int 21h\n dec cx\n jnz writeloop\n ret\n",filw);
  while(root_variable->next!=NULL){
    fprintf(filw,"%s: dw ?\n",root_variable->next->str);
    root_variable=root_variable->next;
  }
  fputs("code ends",filw);
}
//the main function for interpreter and compiler
int main(int argc, char const *argv[]) {
  FILE *filr,*filw;
  char str[200],*k;
  if((filr=fopen(argv[1],"r"))==NULL){
    puts("input file can not be created.");
    exit(1);
  }

  if((filw=fopen("calc.asm","w+"))==NULL){
    puts("output file can not be created.");
    exit(1);
  }
  fputs("code segment\n",filw);

  node *root_stack;
  root_stack=(node *)malloc(sizeof(node));
  root_stack->str=NULL;
  root_stack->next=NULL;
  root_stack->prev=NULL;

  node *root_variable;
  root_variable=(node *)malloc(sizeof(node));
  root_variable->str=NULL;
  root_variable->next=NULL;
  root_variable->prev=NULL;

  do {
    k=fgets(str,200,filr);
    if (k!=NULL)
    parser_statement_compiler(filw,root_stack,root_variable,str);
  } while (k != NULL);
  write_var(root_variable,filw);
  fclose(filw);
  fclose(filr);
}
