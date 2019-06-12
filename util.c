#include "globals.h"
#include "util.h"

/*Procedure printToken prints a token
*and its lexeme to the listing file
*/
void printToken(TokenType token:const char *tokenString){
	printf("Token\tTokenType\tyylex()\n");
	switch(token){
		/*Reserved words*/
		case AND:
		case ARRAY:
		case BEGIN:
		case CASE:
		case CONST:
		case DIV_R:
		case DO:
		case DOWNTO:
		case ELSE:
		case END:
		case FOR:
		case FUNCTION:
		case GOTO:
		case IF:
		case MOD:
		case NOT:
		case OF:
		case OR:
		case PACKED:
		case PROCEDURE:
		case PROGRAM:
		case RECORD:
		case REPEAT:
		case THEN:
		case TO:
		case TYPE:
		case UNTIL:
		case VAR:
		case WHILE:	fprintf(listing,"%s\tReserved Word\t%d\n",tokenString,token);break;
		
		/*Operator, delimiter*/
		case LP:	fprintf(listing,"(\tDelimiter\t%d\n",token);break;
		case RP:	fprintf(listing,")\tDelimiter\t%d\n",token);break;
		case LB:	fprintf(listing,"[\tDelimiter\t%d\n",token);break;
		case RB:	fprintf(listing,"]\tDelimiter\t%d\n",token);break;
		case DOT:	fprintf(listing,".\tDelimiter\t%d\n",token);break;
		case COMMA:	fprintf(listing,",\tDelimiter\t%d\n",token);break;
		case COLON:	fprintf(listing,":\tDelimiter\t%d\n",token);break;
		case MUL:	fprintf(listing,"*\tOperator\t%d\n",token);break;
		case DIV:	fprintf(listing,"/\tOperator\t%d\n",token);break;
		case UNEQUAL:	fprintf(listing,"<>\tOperator\t%d\n",token);break;
		case PLUS:	fprintf(listing,"+\tOperator\t%d\n",token);break;
		case MINUS:	fprintf(listing,"-\tOperator\t%d\n",token);break;
		case GE:	fprintf(listing,">=\tOperator\t%d\n",token);break;
		case GT:	fprintf(listing,">\tOperator\t%d\n",token);break;
		case LE:	fprintf(listing,"<=\tOperator\t%d\n",token);break;
		case LT:	fprintf(listing,"<\tOperator\t%d\n",token);break;
		case EQUAL:	fprintf(listing,"=\tOperator\t%d\n",token);break;
		case ASSIGN:	fprintf(listing,":=\tOperator\t%d\n",token);break;
		case DOTDOT:	fprintf(listing,"..\tDelimiter\t%d\n",token);break;
		case SEMI:	fprintf(listing,";\tDelimiter\t%d\n",token);break;
		
		case SYS_CON:	fprintf(listing,"%s\tSYS_CON\t%d\n",tokenString,token);break;
		case SYS_FUNCTION:	fprintf(listing,"%s\tSYS_FUNCT\t%d\n",tokenString,token);break;
		case SYS_PROC:	fprintf(listing,"%s\tSYS_PROC\t%d\n",tokenString,token);break;
		case SYS_TYPE:	fprintf(listing,"%s\tSYS_TYPE\t%d\n",tokenString,token);break;
		case READ:	fprintf(listing,"%s\tREAD\t%d\n",tokenString,token);break;
		case INTEGER:	fprintf(listing,"%s\tINTEGER\t%d\n",tokenString,token);break;
		case REAL:	fprintf(listing,"%s\tREAL\t%d\n",tokenString,token);break;
		case CHAR:	fprintf(listing,"%s\tCHAR\t%d\n",tokenString,token);break;
		case ID:	if(strlen(tokenString)>MAXIDLEN)tokenString[MAXIDLEN]='\0';
					fprintf(listing,"%s\tID&NAME\t%d\n",tokenString,token);break;
		default:	fprintf(listing,"Unknown token\n",tokenString);break;
		
	}
}


/*allocate space for syntax tree node
*n:the number of the children */
TreeNode *conect(int n,TreeNode *child[]){
	TreeNode *parent=talloc();
	if(parent==NULL){
		fprintf(list,"Out of memory error at line ") 
	} 
	TreeNode *t=NULL;
	for(int i=0;i<n;i++){
		if(i!=0)t->sibling=child[i];
		parent.child[i]=child[i];
		t=child[i];
		while(t->sibling!=NULL)t=t->sibling;	
	}
	return parent;
}

