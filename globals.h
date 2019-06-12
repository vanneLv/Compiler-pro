/************************************************/
/*File: globals.h						 	    */
/*Global types and vars for SPL compiler 	    */
/*Define the token type and struct of treenode	*/
/*must come before other include files			*/ 
/*Author: Xia Siyi						 		*/
/************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef YYPARSER
#include "y.tab.h"
#define ENDFILE 0
#endif

ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/*MAXRESERVED = the number of reserved words*/
#define MAXRESERVED 29
#define MAXIDLEN 15
#define MAXSYSLEN 8
/*typedef enum{
	/*Operator, delimiter*/
/*	LP,RP,LB,RB,DOT,COMMA,COLON,MUL,DIV,UNEQUAL,
	PLUS,MINUS,GE,GT,LE,LT,EQUAL,ASSIGN,DOTDOT,SEMI,
	 /*System functions, procedures, constants, types, etc. */
/*	SYS_CON,SYS_FUNCTION,SYS_PROC,SYS_TYPE,
	READ,INTEGER,REAL,CHAR,
	/*Reserved words*/
/*	AND,ARRAY,BEGIN,CASE,CONST,DIV_R,DO,DOWNTO,ELSE,END,
	FOR,FUNCTION,GOTO,IF,MOD,NOT,OF,OR,PACKED,PROCEDURE,
	PROGRAM,RECORD,REPEAT,THEN,TO,TYPE,UNTIL,VAR,WHILE
	
}TokenType;*/

/*Yacc will generate its own integer values for token*/

typedef int TokenType;

extern FILE *source;
extern FILE *listing;

extern int lineno;

/*typedef enum{
	FALSE,MAXINT,TRUE
} SYS_CON_item;

typedef enum{
	ABS,CHR,ODD,ORD,PRED,SQR,SQRT,SUCC
} SYS_FUNCT_item;

typedef enum{
	WRITE,WRITELN
} SYS_PROC_item;

typedef enum{
	BOOLEAN,CHAR,INTEGER,REAL
} SYS_TYPE_item;
*/
/*typedef union {
	char SYS_item[MAXSYSLEN];
	char id[MAXIDLEN];
	int integer;
	double real;
	char ch; 
}tokenval;

tokenval val;
*/
/**********************************************/
/*************Syntax tree for parsing**********/
/**********************************************/
//for type checking
typedef enum {StmtK,ExpK,FunctK,ProcK,DefK,ProgramK} NodeKind;
typedef enum {AssignK,ProcK,CompoundK,IfK,RepeatK,WhileK,ForK,CaseK,GotoK} StmtKind;
//TYPE means custom type
typedef enum {VOID,TYPE, INT,REAL,CHAR,BOOLEAN} ExpType;
typedef enum {LABEL,CONST,TYPE,VAR} DefType;

#define MAXCHILDREN 6
typedef struct treeNode{
	struct treeNode *child[MAXCHILDREN];
	struct treeNode *sibling;	//Link the syntax tree node of the same layer
	int lineno;
	//for type checking
	NodeKind nodeKind;
	union{
		StmtKind stmt;
		ExpType exp;
		DefType def;
	}kind;
	//Unterminal or terminal symbol
	union {
		char name[MAXIDLEN+1];
		TokenType op; 
		bool boolean;
		int intNum;
		double realNum;
		char ch;
	} attr;			//the attributes of the syntax tree
	
}TreeNode;


/**********************************************/
/****************Flag for tracing**************/
/**********************************************/
extern int EchoSource;	//is source file printed in listing file
extern int TraceScan;	//is token information printed in listing file
extern int TraceParse;	//is syntax tree printed in listing file
extern int TraceAnalyze;	//is symbol table printed in listing file
extern int TraceCode;	//is code printed in listing file
extern int Error;	//prevent further passes if an error occurs

#endif

