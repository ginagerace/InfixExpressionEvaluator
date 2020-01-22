#include "ggerac3Storage.cpp"

bool DebugMode;

void printCommands()
{
 printf ("The commands for this program are:\n\n");
 printf ("q - to quit the program\n");
 printf ("? - to list the accepted commands\n");
 printf ("or any infix mathematical expression using operators of (), *, /, +, -\n");
}

int popAndEval(MyStack *OperatorStack, MyStack *ValueStack){
	
	char op = OperatorStack->top().getOperator();
	OperatorStack->pop();
	
	int v1, v2, v3;
	v1 = v2 = v3 = -999;
	
	if(!ValueStack->isEmpty()) {
		v2 = ValueStack->top().getValue();
		ValueStack->pop();
	}
	else{
		printf("\nERROR Value Stack is empty!");
		return -999;
	}
	if(!ValueStack->isEmpty()) {
		v1 = ValueStack->top().getValue();
		ValueStack->pop();
	}
	else{
		printf("\nERROR Value Stack is empty!");
		return -999;
	}
	if(op == '*')
		v3 = v1 * v2;
	else if(op == '/')
		v3 = v1 / v2;
	else if(op == '+')
		v3 = v1 + v2;
	else if(op == '-')
		v3 = v1 - v2;
	else{
		printf("\nERROR Operator is invalid!");
		return -999;	
	}

	Token *result = new Token();
	result->setToValue(v3);
	ValueStack->push(*result);
}
    
void processExpression (Token inputToken, TokenReader *tr);

int main(int argc, char *argv[])
{
    /***************************************************************/
    /* Add code for checking command line arguments for debug mode */
	
	DebugMode = false;
	//check if debugMode was selected
	 for(int i=0; i<argc; i++) {
       if(strcmp(argv[i], "-d") == 0) {
           DebugMode = true;
           printf("Debugging Information\n");
       }
   }

    Token inputToken;
    TokenReader tr;

    printf ("Starting Expression Evaluation Program\n\n");
    printf ("Enter Expression: ");


    inputToken = tr.getNextToken ();

    while (inputToken.equalsType(QUIT) == false)
    {
      /* check first Token on Line of input */
      if(inputToken.equalsType(HELP))
      {
       printCommands();
       tr.clearToEoln();
      }
      else if(inputToken.equalsType(ERROR))
      {
       printf ("Invalid Input - For a list of valid commands, type ?\n");
       tr.clearToEoln();
      }
      else if(inputToken.equalsType(EOLN))
      {
       printf ("Blank Line - Do Nothing\n");
       /* blank line - do nothing */
      }
      else
      {
       processExpression(inputToken, &tr);
      }

      printf ("\nEnter Expression: ");
      inputToken = tr.getNextToken ();
    }

  printf ("Quitting Program\n");
  return 1;
}

void processExpression (Token inputToken, TokenReader *tr)
{
 /* Declare both stack head pointers here */
 MyStack *OperatorStack = new MyStack();
 MyStack *ValueStack = new MyStack();

 /* Loop until the expression reaches its End */
 while (inputToken.equalsType(EOLN) == false)
   {
    /* The expression contain a VALUE */
    if (inputToken.equalsType(VALUE))
      {
       /* make this a debugMode statement */
       if(DebugMode == true)
	   	 printf ("Val: %d, ", inputToken.getValue() );

       // add code to perform this operation here
	   ValueStack->push(inputToken);
      }

    /* The expression contains an OPERATOR */
    else if (inputToken.equalsType(OPERATOR))
      {
    	char op = inputToken.getOperator();
       /* make this a debugMode statement */
       if(DebugMode == true)
	   	 printf ("OP: %c, ", inputToken.getOperator() );

        // add code to perform this operation here
	    if(op == '(')
		  OperatorStack->push(inputToken);
		if(op=='+' || op=='-') {
			char top = OperatorStack->top().getOperator();
			while(!OperatorStack->isEmpty() && (top=='+' || top=='-' || top=='*' || top=='/')) {
				 popAndEval(OperatorStack, ValueStack);  	
			}
			OperatorStack->push(inputToken);	
		}	
		if(op=='*' || op=='/') {
			char top = OperatorStack->top().getOperator();
			while(!OperatorStack->isEmpty() && (top=='*' || top=='/')) {
				popAndEval(OperatorStack, ValueStack);  	
			}
			OperatorStack->push(inputToken);	
		}
		if(op == ')') {
			while(!OperatorStack->isEmpty() && OperatorStack->top().getOperator()!='(')
				popAndEval(OperatorStack, ValueStack);
			if(OperatorStack->isEmpty())
				printf("ERROR Operator Stack is empty!");
			else
				OperatorStack->pop();
		}
      }

    /* get next token from input */
    inputToken = tr->getNextToken ();
   }

 /* The expression has reached its end */

 // add code to perform this operation here
 while(!OperatorStack->isEmpty())
 	popAndEval(OperatorStack, ValueStack);
 printf("\nTop of Value Stack: %d", ValueStack->top().getValue());
 ValueStack->pop();
 if(ValueStack->isEmpty() != true)
 	printf("\nERROR Value Stack should be empty!");

 printf ("\n\n");
}
