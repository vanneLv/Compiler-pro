/************************************************/
/*File: scan.h						 	    	*/
/*head file for lex file spl.l			 	    */
/*Author: Xia Siyi						 		*/
/************************************************/
#ifndef _SCAN_H_
#define _SCAN_H_

/*Maxsize of a token*/
#define MAXTOKENLEN 40
/*Store the actual context of each token*/
extern char tokenString[MAXTOKENLEN+1];
//extern  tokenval val;
TokenType getToken(void);

#endif

