/************************************************/
/*File: spl.y								    */
/*The SPL Yacc/Bison specification file	 	    */
/*Author: Xia Siyi							    */
/************************************************/

%{
	#define YYPARSER
	
	#include "globals.h"
	#include "util.h"
	#include "scan.h"
	#include "parse.h"
	
	#define YYPARSER TreeNode *
	static char savedName[MAXIDLEN+1];	/*save the program name*/
	static TreeNode *child[MAXCHILDREN];
	static TreeNode *savedTree;/*store syntax tree for later return*/
	
%}

%token ID INTEGER REAL CHAR   /* token types */

%token LP RP LB RB DOTDOT COMMA COLON SEMI    /* Delimiters */

%token DOT PLUS MINUS MUL DIV_R LT LE EQ NE GT GE ASSIGN
%token AND OR NOT DIV MOD /* Operator */

%token ARRAY BEGIN CASE CONST DO DOWNTO ELSE END FOR FUNCTION GOTO
%token IF OF PACKED PROCEDURE PROGRAM RECORD REPEAT THEN
%token TO TYPE UNTIL VAR WHILE
%token SYS_CON SYS_FUNCT SYS_PROC SYS_TYPE READ/* Reserved */

%%

program			: program_head routine DOT			{  
													}
				;
program_head	: PROGRAM ID						{strcpy(savedName,TokenString);} 
				  SEMI								{ YYPARSER t = talloc();
				  									  if(t==NULL){
												      	fprintf(list,"Out of memory error at line %d\n",lineno);
													  } 
													  strcpy(t->attr.name,id);
													  t->nodeKind=Structure;
													  strcpy(t->SyntaxString,"program_head");
													  $$=t;
													}
				;
routine			: routine_head routine_body			{ YYPARSER t;
													  child[0]=$1;
													  child[1]=$2;
													  t = connect(2,child);
													  t->nodeKind=Structure;
													  strcpy(t->SyntaxString,"routine");
													  $$=t;
													}
				;
sub_routine		: routine_head routine_body			{ YYPARSER t;
													  child[0]=$1;
													  child[1]=$2;
													  t = connect(2,child);
													  t->nodeKind=Structure;
													  strcpy(t->SyntaxString,"sub_routine");
													  $$=t;
													}
				;

routine_head	: label_part const_part type_part var_part routine_part
													{ YYPARSER t;
													  child[0]=$1;
													  child[1]=$2;
													  t = connect(2,child);
													  t->nodeKind=Structure;
													  strcpy(t->SyntaxString,"sub_routine");
													  $$=t;
													}	// definitions are not in code tree
				;
label_part		: // empty
				;
// const part
const_part		: CONST const_expr_list				{ $$ = NULL; }
				| // empty
				;
const_expr_list	: const_expr_list ID EQ const_value SEMI 
													{ instConst($2, $4); }
				| ID EQ const_value SEMI			{ instConst($1, $3); }
				;
const_value		: INTEGER							{ $$ = $1; }
				| REAL						{ $$ = $1; }
				| CHAR						{ $$ = $1; }
				| SYS_CON							{ $$ = $1; }
				;
// type part
type_part		: TYPE type_decl_list				{ $$ = NULL; }		// no nodes
				| // empty
				;
type_decl_list	: type_decl_list type_definition	{ $$ = NULL; }
				| type_definition					{ $$ = NULL; }
				;
type_definition	: ID EQ type_decl SEMI				{ instType($1, $3); }
				;
type_decl		: simple_type_decl					{ $$ = $1; }
				| array_type_decl					{ $$ = $1; }
				| record_type_decl					{ $$ = $1; }
				;
simple_type_decl: SYS_TYPE							{ $$ = findType($1); }
				| ID								{ $$ = findType($1); }
				| LP id_list RP						{ $$ = instEnum($2); }
				| const_value DOTDOT const_value	{ $$ = instDotdot($1, $2, $3); }
				| ID DOTDOT ID						{ $$ = instDotdot($1, $2, $3); }
				;
array_type_decl	: ARRAY LB simple_type_decl RB OF type_decl
													{ $$ = instArray($3, $6); }		// instarray
				;
record_type_decl: RECORD field_decl_list END		{ $$ = instRec($1, $2); }		// intrec
				;
field_decl_list	: field_decl_list field_decl		{ $$ = cons($1, $2); }
				| field_decl						{ $$ = $1; }
				;
field_decl		: id_list COLON type_decl SEMI		{ $$ = instFields($1, $3);; }	// instfields
				;
id_list			: id_list COMMA ID					{ $$ = cons($1, $3); }
				| ID								{ $$ = $1; }
				;
// var part
var_part		: VAR var_decl_list					{ endVarPart(); }
				| /* empty */						{ endVarPart(); }
				;
var_decl_list	: var_decl_list var_decl			{ $$ = NULL; }
				| var_decl							{ $$ = NULL; }
				;
var_decl		: id_list COLON type_decl SEMI		{ instVars($1, $3); }	// install vars
				;
// routine part
routine_part	: routine_part function_decl		{ $$ = cons($1, $2); }
				| routine_part procedure_decl		{ $$ = cons($1, $2); }
				| function_decl						{ $$ = $1; }
				| procedure_decl					{ $$ = $1; }
				| /* empty */						{ $$ = NULL; }
				;
function_decl	: function_head SEMI sub_routine SEMI
													{ $$ = makeFunDcl($1, $3); }
				;
function_head	: FUNCTION ID parameters COLON simple_type_decl
													{ $$ = instFun(cons($1, cons($2, cons($5, $3)))); } // FUNCTION, ID, type, para
				;
procedure_decl	: procedure_head SEMI sub_routine SEMI
													{ $$ = makeFunDcl($1, $3); }
				;
procedure_head	: PROCEDURE ID parameters			{ $$ = instFun(cons($1, cons($2, $3))); }	// PROCEDURE, ID, para
				;
parameters		: LP para_decl_list RP				{ $$ = $2; }
				| /* empty */						{ $$ = NULL; }
				;
para_decl_list	: para_decl_list SEMI para_type_list
													{ $$ = cons($1, $3); }
				| para_type_list					{ $$ = $1; }
				;
para_type_list	: var_para_list COLON simple_type_decl
													{ $$ = $1, instVars($1, $3); }
				| val_para_list COLON simple_type_decl
													{ $$ = $1, instVars($1, $3); }
				;
var_para_list	: VAR id_list						{ $$ = $2; }
				;
val_para_list	: id_list							{ $$ = $1; }
				;

// routine body
routine_body	: compound_stmt						{ $$ = $1; }
				;
compound_stmt	: BEGIN_T stmt_list END				{ $$ = makePnb($1, $2); }
				;
stmt_list		: stmt_list stmt SEMI				{ $$ = cons($1, $2); }
				| /* empty*/						{ $$ = NULL; }
				;
stmt			: INTEGER COLON non_label_stmt	{ $$ = doLabel($1, $2, $3); }	// TODO: bug
				| non_label_stmt					{ $$ = $1; }
				;
non_label_stmt	: assign_stmt						{ $$ = $1; }
				| proc_stmt							{ $$ = $1; }
				| compound_stmt						{ $$ = $1; }
				| if_stmt							{ $$ = $1; }
				| repeat_stmt						{ $$ = $1; }
				| while_stmt						{ $$ = $1; }
				| for_stmt							{ $$ = $1; }
				| case_stmt							{ $$ = $1; }
				| goto_stmt							{ $$ = $1; }
				;
assign_stmt		: ID ASSIGN expression				{ $$ = binop($2, findId($1), $3); }
				| ID LB expression RB ASSIGN expression	// array[i], index from 1 defaut
													{ $$ = binop($5, arrayRef($1, NULL, $3, NULL), $6); }
				| ID DOT ID ASSIGN expression		{ $$ = binop($4, reduceDot($1, $2, $3), $5); }
				;
proc_stmt		: ID								{ $$ = makeFuncall($1, NULL, NULL); }
				| ID LP args_list RP				{ $$ = makeFuncall($2, $1, $3); }
				//| SYS_PROC						{ $$ = NULL; }	// what is it ???
				| SYS_PROC LP expression_list RP	{ $$ = makeFuncall($2, $1, $3); }
				| READ LP factor RP					{ $$ = makeFuncall($2, $1, $3); }
				;
if_stmt			: IF expression THEN stmt else_clause		
													{ $$ = makeIf($1, $2, $4, $5); }
				;
else_clause		: ELSE stmt							{ $$ = $2; }
				| // empty
				;
repeat_stmt		: REPEAT stmt_list UNTIL expression	{ $$ = makeRepeat($1, $2, $3, $4); }
				;
while_stmt		: WHILE expression DO stmt			{ $$ = makeWhile($1, $2, $3, $4); }
				;
for_stmt		: FOR ID ASSIGN expression direction expression DO stmt
													{ $$ = makeFor($1, binop($3, $2, $4), $5, $6, $7, $8); }
				;
direction		: TO								{ $$ = $1; }
				| DOWNTO							{ $$ = $1; }
				;
case_stmt		: CASE expression OF case_expr_list END
													{ $$ = NULL; }	// TODO
				;
case_expr_list	: case_expr_list case_expr			{ $$ = NULL; }
				| case_expr							{ $$ = NULL; }
				;
case_expr		: const_value COLON stmt SEMI		{ $$ = NULL; }
				| ID COLON stmt SEMI				{ $$ = NULL; }
				;
goto_stmt		: GOTO INTEGER					{ $$ = doGoto($1, $2); }	// useless
				;
expression_list	: expression_list COMMA expression	{ $$ = cons($1, $2); }
				| expression						{ $$ = $1; }
				;
expression		: expression GE expr				{ $$ = binop($2, $1, $3); }
				| expression GT expr				{ $$ = binop($2, $1, $3); }
				| expression LE expr				{ $$ = binop($2, $1, $3); }
				| expression LT expr				{ $$ = binop($2, $1, $3); }
				| expression EQ expr				{ $$ = binop($2, $1, $3); }
				| expression NE expr				{ $$ = binop($2, $1, $3); }
				| expr								{ $$ = $1; }
				;
expr			: expr PLUS term					{ $$ = binop($2, $1, $3); }
				| expr MINUS term					{ $$ = binop($2, $1, $3); }
				| expr OR term						{ $$ = binop($2, $1, $3); }
				| term								{ $$ = $1; }
				;
term			: term MUL factor					{ $$ = binop($2, $1, $3); }
				| term DIV factor					{ $$ = binop($2, $1, $3); }
				| term MOD factor					{ $$ = binop($2, $1, $3); }
				| term AND factor					{ $$ = binop($2, $1, $3); }
				| factor							{ $$ = $1; }
				;
factor			: ID								{ $$ = findId($1); }
				| ID LP args_list RP				{ $$ = makeFuncall($2, $1, $3); }
				//| SYS_FUNCT						{ $$ = NULL; }	// what is it ???
				| SYS_FUNCT LP args_list RP			{ $$ = makeFuncall($2, $1, $3); }
				| const_value						{ $$ = $1; }
				| LP expression RP					{ $$ = $2; }
				| NOT factor						{ $$ = unaryop($1, $2); }
				| MINUS factor						{ $$ = unaryop($1, $2); }
				| ID LB expression RB				{ $$ = arrayRef($1, NULL, $3, NULL); }
				| ID DOT ID							{ $$ = reduceDot($1, $2, $3); }
				;
args_list		: args_list COMMA expression		{ $$ = cons($1, $3); }
				| expression						{ $$ = $1; }

%%


