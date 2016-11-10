#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct list{
  char *str;
  int value;
  struct list *next;
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
//to control it is integer or not
bool isInteger(char *exp){
  int i,num=strlen(exp);
  for(i=0;exp[i];i++){
    if(isdigit(exp[i])) num--;
  }
  if(num==0){
    return true;
  }else return false;
}
//to control it is variable or not
bool isVariable(char *exp){
  if(strlen(exp)==0) return false;
  if(isInteger(exp)||!isalpha(exp[0])) return false;
  else return true;
}
//to get variable's value
int variableValue(node *root_variable,char *exp){
  while(root_variable->next!=NULL){
    if(isEqual(root_variable->next->str,exp)){
      return root_variable->next->value;
    }else root_variable=root_variable->next;
  }
  puts("Error..Indefined Variable.");
  exit(1);
}
//to add variables name and its value to linked list
void add_element_variable(node *root_variable,char *factor,int num){
  while(root_variable->next!=NULL){
    if(isEqual(root_variable->next->str,factor)){
      root_variable->next->value=num;
      return;
    }
    root_variable=root_variable->next;
  }
  root_variable->next=(node *)malloc(sizeof(node));
  root_variable->next->str=factor;
  root_variable->next->value=num;
  root_variable->next->next=NULL;
}
//to calculate expression
int calculator(node *root_variable,char *str){
  char *exp=malloc(200);
  strcpy(exp,str);
  int i,len=strlen(exp),numOfParser=0,numOfBracket=0;
  for(i=len-1;i>=0;i--){
    if(exp[i]==')') numOfBracket++;
    if(exp[i]=='(') numOfBracket--;
    if(numOfBracket==0){
      if(exp[i]=='+'){
        numOfParser++;
        exp[i]='\0';
        if(strlen(exp+i+1)==0){
          puts("ERROR..Wrong input");
          exit(1);
        }
        return calculator(root_variable,exp)+calculator(root_variable,exp+i+1);
      }
      if(exp[i]=='-'){
        numOfParser++;
        exp[i]='\0';
        if(strlen(exp+i+1)==0){
          puts("ERROR..Wrong input");
          exit(1);
        }
        return calculator(root_variable,exp)-calculator(root_variable,exp+i+1);
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
          if((strlen(exp+i+1)==0)||(strlen(exp))==0){
            puts("ERROR..Wrong input");
            exit(1);
          }
          return calculator(root_variable,exp)*calculator(root_variable,exp+i+1);
        }
        if(exp[i]=='/'){
          numOfParser++;
          exp[i]='\0';
          if((strlen(exp+i+1)==0)||(strlen(exp))==0){
            puts("ERROR..Wrong input");
            exit(1);
          }
          return calculator(root_variable,exp)/calculator(root_variable,exp+i+1);
        }
      }
    }
  }
  if(numOfParser==0){
    if(exp[len-1]==')'&&exp[0]=='('){
      exp[len-1]='\0';
      if(strlen(exp+1)==0){puts("ERROR..Wrong input");exit(1);}
      calculator(root_variable,exp+1);
    }else{
      if(strlen(exp)==0){
        return 0;
      }else if(isInteger(exp)){
        return atoi(exp);
      }else if(isVariable(exp)){
        return variableValue(root_variable,exp);
      }else{
        puts("ERROR...Wrong type variable or integer");
        exit(1);
      }
    }
  }
}
//to parse right and left of the '=' sign
void parser_statement(node *root_variable,char *str){
  char *exp=malloc(200);
  int i,j,num=0,len=strlen(str);
  //to clean spaces
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
        add_element_variable(root_variable,exp,calculator(root_variable,exp+i+1));
      }else{
        puts("ERROR...Wrong type variable");
        exit(1);
      }
    }
  }
  if(num==0){
    printf("%d\n",calculator(root_variable,exp));
  }
}
//main function
int main(int argc, char const *argv[]) {
  FILE *fil,*ter;
  char str[200],*k;
  if((fil=fopen(argv[1],"r"))==NULL){
    ter=stdin;
    node *root_variable;
    root_variable=(node *)malloc(sizeof(node));
    root_variable->str=NULL;
    root_variable->next=NULL;
    do {
      k=fgets(str,200,ter);
      if (k!=NULL)
      parser_statement(root_variable,str);
    } while (k != NULL);
    fclose(ter);
  }else{
    node *root_variable;
    root_variable=(node *)malloc(sizeof(node));
    root_variable->str=NULL;
    root_variable->next=NULL;
    do {
      k=fgets(str,200,fil);
      if (k!=NULL)
      parser_statement(root_variable,str);
    } while (k!= NULL);
  }
  fclose(fil);
}
