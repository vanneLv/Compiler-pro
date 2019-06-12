#ifndef _UTIL_H_
#define _UTIL_H_
 
/*Procedure printToken prints a token
*and its lexeme to the listing file
*/

void printToken(TokenType,const char *);

TreeNode * newStmtNode(StmtKind kind);
TreeNode * newExpNode(ExpKind kind);
TreeNode * newFunctNode(void);
TreeNode * newProcNode(void);
TreeNode * newDefNode(DefKind kind);
TreeNode * newProgNode(void)

/*Function new*/
#endif
 
