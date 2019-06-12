#include "globals.h"
#include "code.c"

/* location number for current instruction emission */
static int emitLoc = 0;

/* highest location emitted so far for use in conjunction with 
emitSkip, emitBackup, and emitRestore */
static int highEmitloc = 0;


/* procedure emitRO emits a register-only instruction
	op = the opcode
	r = target register
	s = 1st source register
	t = 2nd source register
	c = a comment to be printed if TraceCode is TRUE
*/
void emitRO( char *op, int r, int s, int t, char *c)
{
	fprintf(code, "%3d:	%5s %d,%d,%d ", emitLoc++, op, r, s, t);
	if(TraceCode)
		fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitloc < emitLoc)
		highEmitloc = emitLoc;
}


/* prcedure emitRM emits a register-to-memory instruction
	op = the opcode
	r = target register
	d = the offset
	s = the base register
	c = a comment to be printed if TraceCode is TRUE
*/
void emitRM(char *op, int r, int d, int s, char *c)
{
	fprintf(code, "%3d: %5s %d,%d(%d) ", emitLoc++, op, r, d, s);
	if(TraceCode)
		fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitloc < emitLoc)
		highEmitloc = emitLoc;
}


/* function emitSkip skips "howMany" code locations for later backpatch.
	It also returns the current code position.
*/
int emitSkip(int howMany)
{
	int i = emitLoc;
	emitLoc += howMany;
	if(highEmitloc < emitLoc)
		highEmitloc = emitLoc;
	return i;
}


/* procedure emitBackup backs up to 
	loc = a previously skipped location 
*/
void emitBackup(int loc)
{
	if(loc > highEmitloc)
		emitComment("BUG in emitBackup");
	emitLoc = loc;
}


/* procedure emitRestore restores the current code position 
to the highest previously unemitted position
*/
void emitRestore(void)
{
	emitLoc = highEmitloc;
}


/* procedure emitComment prints a comment line with comment c in the code file*/
void emitComment(char *c)
{
	if(TraceCode)
		fprintf(code, "* %s\n", c);
}


/* procedure emitRM_Abs converts an absolute reference to
a pc-relative reference when emitting a register-to-memory instruction
	op = the opcode
	r = target register
	a = the absulote location in memory
	c = a comment to be printed if TraceCode is TRUE
*/
void emitRM_Abs(char *op, int r, int a, char *c)
{
	fprintf(code, "%3d: %5s %d,%d(%d) ", emitLoc, op, r, a-(emitLoc+1), pc);
	++emitLoc;
	if(TraceCode)
		fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if(highEmitloc < emitLoc)
		highEmitloc = emitLoc;
}