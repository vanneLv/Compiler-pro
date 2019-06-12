/************************************************/
/*File: spl.l								    */
/*Lex specification for SPL				 	    */
/*Author: Xia Siyi							    */
/************************************************/

%{
	#include "globals.h"
	#include "util.h"
	#include "scan.h"
	/*lexeme of identifier or reserved word*/
	char tokenString[MAXTOKENLEN+1];
	char id[MAXIDLEN+1];
	//extern tokenval val;
%}

/* regular definitions */

whitespace		[ \t]+
letter		[A-Za-z]
digit		[0-9]
id			{letter}(_|{letter}|{digit})*
integer		{digit}+
real		{digit}+(\.{digit}+)?(([E|e])[+\-]?{digit}+)?
char		(\'{letter}\')
comment1	("{")
comment2	(\/\/.*\n)
newline		\n
/* LEX: RULES  */
%%

{whitespace}		{ /* no action and no return */ }

/*delimiter*/
"("			{ return LP; }
")"			{ return RP; }
"["			{ return LB; }
"]"			{ return RB; }
".."		{ return DOTDOT; }
","			{ return COMMA; }
":"			{ return COLON; }
";"			{ return SEMI; }

"."			{ return DOT;}
"+"			{ return PLUS;}
"-"			{ return MINUS;}
"*"			{ return MUL;}
"/"			{ return DIV_R; }
"<"			{ return LT; }
"<="		{ return LE; }
"="			{ return EQ; }
"<>"		{ return NE; }
">"			{ return GT; }
">="		{ return GE; }
":="		{ return ASSIGN; }

/*reserved words*/
"and"		{ return AND;}
"array"		{ return ARRAY;}
"begin"		{ return BEGIN;} 
"case"		{ return CASE;}
"const"		{ return CONST;}
"div"		{ return DIV_R;}
"do"		{ return DO;}
"downto"	{ return DOWNTO;}
"else"		{ return ELSE;}
"end"		{ return END;}
"for"		{ return FOR;}
"function"	{ return FUNCTION;}
"goto"		{ return GOTO;}
"if"		{ return IF;}
"mod"		{ return MOD;}
"not"		{ return NOT;}
"of"		{ return OF;}
"or"		{ return OR;}
"packed"	{ return PACKED;}
"procedure"	{ return PROCEDURE;}
"program"	{ return PROGRAM;}
"record"	{ return RECORD;}
"repeat"	{ return REPEAT;}
"then"		{ return THEN;}
"to"		{ return TO;} 
"type"		{ return TYPE;}
"until"		{ return UNTIL;}
"var"		{ return VAR;}
"while"		{ return WHILE;}
"read"		{ return READ;}

 /*System functions, procedures, constants, types, etc. */
 "false"|"maxint"|"true"							{ /*strcpy(val.SYS_item,yytext);*/return SYS_CON;}
 "abs"|"chr"|"odd"|"ord"|"pred"|"sqr"|"sqrt"|"succ"	{/*strcpy(val.SYS_item,yytext);*/return SYS_FUNCT;}
 "write"|"writeln"									{/*strcpy(val.SYS_item,yytext);*/return SYS_PROC;}
 "boolean"|"char"|"integer"|"real"					{/*strcpy(val.SYS_item,yytext);*/return SYS_TYPE;}
 
{id}		{/*strncpy(val.id,yytext,15);*/return ID;}
{integer}	{/*sscanf(yytext,"%d",val.integer);*/return INTEGER;}
{real}		{/*sscanf(yytext,"%f",val.integer);*/return REAL;}
{char}		{/*val.ch=yytext[0];*/return CHAR;}

{comment1}	{ char c;
			  do
			  {c=input();
			   if(c==EOF)break;
			   if(c=='\n')lineno++;
			  }while(c!='}')
			} 
{comment2}	{lineno++;}

{newline}	{lineno++;}

.			{ fprintf(list,"lex scan error!\n");return ERROR;}

%%

TokenType getToken(void){
	static int firstTime=TRUE;
	TokenType currentToken;
	if(firstTime){
		firstTime=FALSE;
		lineno++;
		yyin=source;
		yyout=listing;
	}
	currentToken=yylex();
	strncpy(tokenString,yytext,MAXTOKENLEN);
	if(TraceScan){
		fprintf(listing,"\t%d: ",lineno);
		printToken(currentToken,tokenString);
	} 
	return currentToken;
}
 
