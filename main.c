// Example of a grammar for a tree parser.
// Adapted from Java equivalent example, by Terence Parr
// Author: Jim Idle - April 2007
// Permission is granted to use this example code in any way you want, so long as
// all the original authors are cited.
//

// set ts=4,sw=4
// Tab size is 4 chars, indent is 4 chars

// Notes: Although all the examples provided are configured to be built
//        by Visual Studio 2005, based on the custom build rules
//        provided in $(ANTLRSRC)/code/antlr/main/runtime/C/vs2005/rulefiles/antlr3.rules
//        there is no reason that this MUST be the case. Provided that you know how
//        to run the antlr tool, then just compile the resulting .c files and this
//	  file together, using say gcc or whatever: gcc *.c -I. -o XXX
//	  The C code is generic and will compile and run on all platforms (please
//        report any warnings or errors to the antlr-interest newsgroup (see www.antlr.org)
//        so that they may be corrected for any platform that I have not specifically tested.
//
//	  The project settings such as additional library paths and include paths have been set
//        relative to the place where this source code sits on the ANTLR perforce system. You
//        may well need to change the settings to locate the includes and the lib files. UNIX
//        people need -L path/to/antlr/libs -lantlr3c (release mode) or -lantlr3cd (debug)
//
//        Jim Idle (jimi cut-this at idle ws)
//

// You may adopt your own practices by all means, but in general it is best
// to create a single include for your project, that will include the ANTLR3 C
// runtime header files, the generated header files (all of which are safe to include
// multiple times) and your own project related header files. Use <> to include and
// -I on the compile line (which vs2005 now handles, where vs2003 did not).

#include "simplecTreeParser.h"
#include <search.h>
#include <gmodule.h>
// Main entry point for this example
//
int true = 1;
int false = 0;
int error = -1;

int *pTrue = &true;
int *pFalse = &false;
int *pError = &error;

int ANTLR3_CDECL
main(int argc, char *argv[])
{

  void free_data(gpointer data)
  {
    //printf("freeing: %s %p\n", (char *)data, data);
    free(data);
  }
  //hashtable per i valori associati alle variabili
  GHashTable *valuesTable = g_hash_table_new_full(g_str_hash, g_str_equal, free_data, free_data);
  //hashtable per i tipi delle variabili
  GHashTable *typesTable = g_hash_table_new_full(g_str_hash, g_str_equal, free_data, free_data);

  //funzione per inserire chiave e valore nella hashtable scelta
  void hashInsert(char *keyChar, gpointer valueChar, GHashTable *hashTable)
  {
    gchar *key = g_strdup(keyChar);
    gchar *value = g_strdup(valueChar);

    if (!g_hash_table_contains(hashTable, (gconstpointer)key))
    {
      //printf("first insert of variable\n");
      g_hash_table_insert(hashTable, key, value);
    }
    else
    {
      //printf("value modified\n");
      g_hash_table_insert(hashTable, key, value);
    }
  }

  //funzione che ritorna il valore della chiave riportata nella hashtable
  char *hashGetVaue(char *key, GHashTable *hashTable)
  {
    if (!g_hash_table_contains(hashTable, (gconstpointer)key))
    {
      printf("variable not found in scope\n");
      return NULL;
    }
    else
    {
      gpointer value = g_hash_table_lookup(hashTable, (gconstpointer)key);
      if (value != NULL)
      {
        return (char *)value;
      }
      else
        return NULL;
    }
  }
  /*
  struct Scope
  {
    struct Scope *parent;
    struct Scope *block;
    char *latest;
    struct Scope *next;
    struct Scope *prev;
  };

  struct Scope *head = NULL;
  struct Scope *temp = NULL;
*/

  void inputString()
  {
    char userInput1[50];
    char *pUserInput1 = userInput1;
    char userInput2[25];
    char *pUserInput2 = userInput2;
    char userInput3[25];
    char *pUserInput3 = userInput3;
    while (strcmp(pUserInput2, "n") != 0)
    {
      printf("enter a command (h for commands)\n");
      fgets(userInput1, 49, stdin);
      //printf("fgets: %s\n", userInput1);
      sscanf(userInput1, "%s %s", pUserInput2, pUserInput3);
      //printf("input1: %s\n", userInput1);
      //printf("input2: %s\n", userInput2);
      //printf("input3: %s\n", userInput3);

      if (strcmp(pUserInput2, "n") == 0)
      {
        printf("next line:\n");
        //temp = temp->next;
        //printf("istruction %s\n", temp->latest);
      }
      else if (strcmp(pUserInput2, "h") == 0)
      {
        printf("'n' for next instruction\n");
        printf("'print'+ 'var name' to print variable name and type\n\n");
        //temp = temp->prev;
        //printf("istruction %s\n", temp->latest);
      }
      else if (strcmp(pUserInput2, "p") == 0)
      {
        printf("input p\n");
        //temp = temp->prev;
        //printf("istruction %s\n", temp->latest);
      }
      else if (pUserInput1[0] == 's' && userInput1[1] == ' ')
      {
        //pUserInput = pUserInput + 2;
        printf("userinput +2: %s\n", userInput1 + 2);
        // search in Scope for variable
      }
      else if (strcmp(pUserInput2, "print") == 0 )
      {
        char *var;
        char *varType;
        if (userInput3[0] != '\0')
        {
          var = hashGetVaue(userInput3, valuesTable);
          varType = hashGetVaue(userInput3, typesTable);

          printf("variable %s:\n type: %s\n value %s\n", userInput3, varType, var);
        }
      }
      else
      {
        printf("comando non riconosciuto\n");
      }
    }
  }

  //stampa il la riga da cui viene il token e il suo numero nel file
  void printLine(pANTLR3_COMMON_TOKEN token)
  {
    char *line = (char *)token->lineStart;
    int i = 0;
    int lineNumber = (int)token->line;
    printf("line %d: ", lineNumber);
    while (line[i] != ';' && line[i] != '{')
    {
      if (line[i] == '\t')
      {
        continue;
      }
      printf("%c", line[i]);
      i = i + 1;
    }
    printf("\n\n");
  }

  // funzione che attraversa l'albero ed esegue le funzioni associate in base al tipo di token
  // che incontra

  int numberEvaluator(pANTLR3_BASE_TREE tree){
    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    if (tok){
      switch (tok->type)
      {
      case INT:
      {
        //printf("token data int: %s\n", &tok->input->nextChar);
        const char *s = tree->getText(tree)->chars;
        int value = atoi(s);
        return value;
      }
      case ID:
      {
        char *var = tree->getText(tree)->chars;
        //check type
        int value = atoi(hashGetVaue(var, valuesTable));
        return value;
      }
      case PLUS:
      {
        return numberEvaluator(tree->getChild(tree, 0)) + numberEvaluator(tree->getChild(tree, 1));
      }
      case MINUS:
      {
        return numberEvaluator(tree->getChild(tree, 0)) - numberEvaluator(tree->getChild(tree, 1));
      }
      case MULT:
      {
        return numberEvaluator(tree->getChild(tree, 0)) * numberEvaluator(tree->getChild(tree, 1));
      }
      case DIVI:
      {
        return numberEvaluator(tree->getChild(tree, 0)) / numberEvaluator(tree->getChild(tree, 1));
      }
      case REST:
      {
        return numberEvaluator(tree->getChild(tree, 0)) % numberEvaluator(tree->getChild(tree, 1));
      }

    }

  }

  int evaluate(pANTLR3_BASE_TREE tree)
  {
    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    if (tok)
    {
      switch (tok->type)
      {
      case INT:
      {
        //printf("token data int: %s\n", &tok->input->nextChar);
        const char *s = tree->getText(tree)->chars;
        int value = atoi(s);
        return value;
      }
      case CHARACTER_LITERAL:
      {
        const char *s = tree->getText(tree)->chars;
        int value = atoi(s);
        return value;
      }
      case STRING_LITERAL:
      {
        const char *s = tree->getText(tree)->chars;
        int value = atoi(s);
        return value;
      }
      case ID:
      {
        char *var = tree->getText(tree)->chars;
        //check type
        int value = atoi(hashGetVaue(var, valuesTable));
        return value;
      }
      case EQEQ:
      {
        int firstValue = (int)evaluate(tree->getChild(tree, 0));
        int secondValue = (int)evaluate(tree->getChild(tree, 1));
        printLine(tok);
        inputString();
        if (firstValue == secondValue)
        {
          return true;
        }
        else
        {
          return false;
        }
      }
      case NEQ:
      {
        int firstValue = (int)evaluate(tree->getChild(tree, 0));
        int secondValue = (int)evaluate(tree->getChild(tree, 1));
        printLine(tok);
        inputString();
        if (firstValue != secondValue)
        {
          return true;
        }
        else
        {
          return false;
        }
      }
      case OPLT:
      {
        int firstValue =  evaluate(tree->getChild(tree, 0));
        int secondValue = evaluate(tree->getChild(tree, 1));
        printLine(tok);
        inputString();
        if (firstValue < secondValue)
        {
          return true;
        }
        else
        {
          return false;
        }
      }
      case PLUS:
      {
        return evaluate(tree->getChild(tree, 0)) + evaluate(tree->getChild(tree, 1));
      }
      case MINUS:
      {
        return evaluate(tree->getChild(tree, 0)) - evaluate(tree->getChild(tree, 1));
      }
      case MULT:
      {
        return evaluate(tree->getChild(tree, 0)) * evaluate(tree->getChild(tree, 1));
      }
      case DIVI:
      {
        return evaluate(tree->getChild(tree, 0)) / evaluate(tree->getChild(tree, 1));
      }
      case REST:
      {
        return evaluate(tree->getChild(tree, 0)) % evaluate(tree->getChild(tree, 1));
      }

      case VAR_DEF:
      {
        //metti la variabile nella tabella hash con valore null
        pANTLR3_BASE_TREE child0 = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        char *variableType = child0->getText(child0)->chars;
        pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);
        char *variableName = child1->getText(child1)->chars;
        hashInsert(variableName, variableType, typesTable);
        hashInsert(variableName, NULL, valuesTable);
        printLine(tok);
        inputString();
        return 1;
      }
      case EQ:
      {
        int i = 0;
        //caso in cui si faccia definizione e assegnamento insieme
        if (tree->getChildCount(tree) == 3)
        {
          pANTLR3_BASE_TREE child0 = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
          char *variableType = child0->getText(child0)->chars;
          pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);
          char *variableName = child1->getText(child1)->chars;
          hashInsert(variableName, variableType, typesTable);
          i = i + 1;
        }
        //caso di assegnamento
        pANTLR3_BASE_TREE child0 = (pANTLR3_BASE_TREE)tree->getChild(tree, i);
        char *variableName = child0->getText(child0)->chars;
        char *type = hashGetVaue(variableName, typesTable);
        //string var(getText(getChild(tree, 0)));

        if (strcmp(type, "int") == 0)
        {
          pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, i + 1);
          int value = evaluate(child1);
          char strVal[12];
          sprintf(strVal, "%d", value);
          hashInsert(variableName, strVal, valuesTable);
          printLine(tok);
          inputString();
          return value;
        }
        else if (strcmp(type, "char") == 0)
        {
          pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, i + 1);
          char *value = child1->getText(child1)->chars;
          //printf("value child1: %s\n",value);
          //char strVal[12];
          //sprintf(strVal, "%d", value);
          hashInsert(variableName, value, valuesTable);
          printLine(tok);
          inputString();
          return 1;
        }
        else
        {
          printf("type not found\n");
          return error;
        }
      }
      case IF_STAT:
      {
        //caso if senza else
        pANTLR3_BASE_TREE childIFELSE = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        pANTLR3_BASE_TREE childIF = (pANTLR3_BASE_TREE)childIFELSE->getChild(childIFELSE, 0);

        int ifClauseValue = evaluate(childIF);

        //condizione dell'if è vera quindi svolgo il codice nell'if
        if (ifClauseValue != -1 && ifClauseValue > 0)
        {
          //printf("ifclause %d\n", ifClauseValue);
          //printf("childif %s\n", childIF->toStringTree(childIF)->chars);
          return evaluate((pANTLR3_BASE_TREE)childIFELSE->getChild(childIFELSE, 1));
        }
        else if (tree->getChildCount(tree) > 1)
        {
          {
            pANTLR3_BASE_TREE childELSEIF = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);
            pANTLR3_BASE_TREE childELSE = (pANTLR3_BASE_TREE)childELSEIF->getChild(childELSEIF, 0);
            //printf("%s\n", childELSE->toStringTree(childELSE)->chars);
            return evaluate(childELSE);
          }
        }
      }
      case WHI_STAT:
      {
        pANTLR3_BASE_TREE conditionalExpr = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);

        int conditionalExprEval = evaluate(conditionalExpr);
        int ret;

        while (conditionalExprEval != -1 && conditionalExprEval > 0)
        {
          //printf("conditionalecpreval: %d\n", conditionalExprEval);
          ret = evaluate((pANTLR3_BASE_TREE)tree->getChild(tree, 1));
          conditionalExprEval = evaluate(conditionalExpr);
        }
        return ret;
      }
      case BLOCK:
      {
        int k = tree->getChildCount(tree);
        int r = 0;
        for (int i = 0; i < k; i++)
        {
          r = evaluate(tree->getChild(tree, i));
        }
        return r;
      }
      default:
        printf("Unhandled token: %d\n", tok->type);
        return error;
      }
    }
    else
    {
      int k = tree->getChildCount(tree);
      int r = 0;
      for (int i = 0; i < k; i++)
      {
        r = evaluate(tree->getChild(tree, i));
      }
      return r;
    }
  }

  // Now we declare the ANTLR related local variables we need.
  // Note that unless you are convinced you will never need thread safe
  // versions for your project, then you should always create such things
  // as instance variables for each invocation.
  // -------------------

  // Name of the input file. Note that we always use the abstract type pANTLR3_UINT8
  // for ASCII/8 bit strings - the runtime library guarantees that this will be
  // good on all platforms. This is a general rule - always use the ANTLR3 supplied
  // typedefs for pointers/types/etc.
  //
  pANTLR3_UINT8 fName;

  // The ANTLR3 character input stream, which abstracts the input source such that
  // it is easy to provide input from different sources such as files, or
  // memory strings.
  //
  // For an ASCII/latin-1 memory string use:
  //	    input = antlr3NewAsciiStringInPlaceStream (stringtouse, (ANTLR3_UINT64) length, NULL);
  //
  // For a UCS2 (16 bit) memory string use:
  //	    input = antlr3NewUCS2StringInPlaceStream (stringtouse, (ANTLR3_UINT64) length, NULL);
  //
  // For input from a file, see code below
  //
  // Note that this is essentially a pointer to a structure containing pointers to functions.
  // You can create your own input stream type (copy one of the existing ones) and override any
  // individual function by installing your own pointer after you have created the standard
  // version.
  //
  pANTLR3_INPUT_STREAM input;

  // The lexer is of course generated by ANTLR, and so the lexer type is not upper case.
  // The lexer is supplied with a pANTLR3_INPUT_STREAM from whence it consumes its
  // input and generates a token stream as output.
  //
  pSimpleCLexer lxr;

  // The token stream is produced by the ANTLR3 generated lexer. Again it is a structure based
  // API/Object, which you can customise and override methods of as you wish. a Token stream is
  // supplied to the generated parser, and you can write your own token stream and pass this in
  // if you wish.
  //
  pANTLR3_COMMON_TOKEN_STREAM tstream;

  // The Lang parser is also generated by ANTLR and accepts a token stream as explained
  // above. The token stream can be any source in fact, so long as it implements the
  // ANTLR3_TOKEN_SOURCE interface. In this case the parser does not return anything
  // but it can of course specify any kind of return type from the rule you invoke
  // when calling it.
  //
  pSimpleCParser psr;

  // The parser produces an AST, which is returned as a member of the return type of
  // the starting rule (any rule can start first of course). This is a generated type
  // based upon the rule we start with.
  //
  SimpleCParser_program_return SimpleCAST;

  // The tree nodes are managed by a tree adaptor, which doles
  // out the nodes upon request. You can make your own tree types and adaptors
  // and override the built in versions. See runtime source for details and
  // eventually the wiki entry for the C target.
  //
  pANTLR3_COMMON_TREE_NODE_STREAM nodes;

  // Finally, when the parser runs, it will produce an AST that can be traversed by the
  // the tree parser: c.f. SimpleCWalker.g3t
  //
  pSimpleCWalker treePsr;

  // Create the input stream based upon the argument supplied to us on the command line
  // for this example, the input will always default to ./input if there is no explicit
  // argument.
  //
  if (argc < 2 || argv[1] == NULL)
  {
    fName = (pANTLR3_UINT8) "./input"; // Note in VS2005 debug, working directory must be configured
  }
  else
  {
    fName = (pANTLR3_UINT8)argv[1];
  }

  // Create the input stream using the supplied file name
  // (Use antlr3AsciiFileStreamNew for UCS2/16bit input).
  //
  input = antlr3AsciiFileStreamNew(fName);

  // The input will be created successfully, providing that there is enough
  // memory and the file exists etc
  //
  if (input == NULL)
  {
    fprintf(stderr, "Unable to open file %s\n", (char *)fName);
    exit(1);
  }

  // Our input stream is now open and all set to go, so we can create a new instance of our
  // lexer and set the lexer input to our input stream:
  //  (file | memory | ?) --> inputstream -> lexer --> tokenstream --> parser ( --> treeparser )?
  //
  lxr = SimpleCLexerNew(input); // CLexerNew is generated by ANTLR

  // Need to check for errors
  //
  if (lxr == NULL)
  {
    fprintf(stderr, "Unable to create the lexer due to malloc() failure1\n");
    exit(ANTLR3_ERR_NOMEM);
  }

  // Our lexer is in place, so we can create the token stream from it
  // NB: Nothing happens yet other than the file has been read. We are just
  // connecting all these things together and they will be invoked when we
  // call the parser rule. ANTLR3_SIZE_HINT can be left at the default usually
  // unless you have a very large token stream/input. Each generated lexer
  // provides a token source interface, which is the second argument to the
  // token stream creator.
  // Note tha even if you implement your own token structure, it will always
  // contain a standard common token within it and this is the pointer that
  // you pass around to everything else. A common token as a pointer within
  // it that should point to your own outer token structure.
  //
  tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));

  if (tstream == NULL)
  {
    fprintf(stderr, "Out of memory trying to allocate token stream\n");
    exit(ANTLR3_ERR_NOMEM);
  }

  // Finally, now that we have our lexer constructed, we can create the parser
  //
  psr = SimpleCParserNew(tstream); // CParserNew is generated by ANTLR3

  if (tstream == NULL)
  {
    fprintf(stderr, "Out of memory trying to allocate parser\n");
    exit(ANTLR3_ERR_NOMEM);
  }

  // We are all ready to go. Though that looked complicated at first glance,
  // I am sure, you will see that in fact most of the code above is dealing
  // with errors and there isn;t really that much to do (isn;t this always the
  // case in C? ;-).
  //
  // So, we now invoke the parser. All elements of ANTLR3 generated C components
  // as well as the ANTLR C runtime library itself are pseudo objects. This means
  // that they are represented as pointers to structures, which contain any
  // instance data they need, and a set of pointers to other interfaces or
  // 'methods'. Note that in general, these few pointers we have created here are
  // the only things you will ever explicitly free() as everything else is created
  // via factories, that allocates memory efficiently and free() everything they use
  // automatically when you close the parser/lexer/etc.
  //
  // Note that this means only that the methods are always called via the object
  // pointer and the first argument to any method, is a pointer to the structure itself.
  // It also has the side advantage, if you are using an IDE such as VS2005 that can do it
  // that when you type ->, you will see a list of all the methods the object supports.
  //
  SimpleCAST = psr->program(psr);

  // If the parser ran correctly, we will have a tree to parse. In general I recommend
  // keeping your own flags as part of the error trapping, but here is how you can
  // work out if there were errors if you are using the generic error messages
  //
  if (psr->pParser->rec->state->errorCount > 0)
  {
    fprintf(stderr, "The parser returned %d errors, tree walking aborted.\n", psr->pParser->rec->state->errorCount);
  }
  else
  {

    pANTLR3_BASE_TREE rootTree = SimpleCAST.tree;

    printf("Tree : %s\n\n", SimpleCAST.tree->toStringTree(SimpleCAST.tree)->chars);
    nodes = antlr3CommonTreeNodeStreamNewTree(SimpleCAST.tree, ANTLR3_SIZE_HINT); // sIZE HINT WILL SOON BE DEPRECATED!!

    // Tree parsers are given a common tree node stream (or your override)
    evaluate(rootTree);
    //inputString();
    /*
    printf("valore di c: %s\n", hashGetVaue("c", valuesTable));
    printf("tipo di c: %s\n", hashGetVaue("c", typesTable));
    printf("valore di x: %s\n", hashGetVaue("x", valuesTable));
    printf("valore di z: %s\n", hashGetVaue("z", valuesTable));
  */
    treePsr = SimpleCWalkerNew(nodes);
    //treePsr->program(treePsr);

    nodes->free(nodes);
    nodes = NULL;
    treePsr->free(treePsr);
    treePsr = NULL;
  }

  // We did not return anything from this parser rule, so we can finish. It only remains
  // to close down our open objects, in the reverse order we created them
  //

  psr->free(psr);
  psr = NULL;
  tstream->free(tstream);
  tstream = NULL;
  lxr->free(lxr);
  lxr = NULL;
  input->close(input);
  input = NULL;

  return 0;
}
