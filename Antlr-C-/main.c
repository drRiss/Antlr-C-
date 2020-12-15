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

int ANTLR3_CDECL main(int argc, char *argv[])
{

  void free_data(gpointer data)
  {
    g_free(data);
  }

  typedef struct Scope
  {
    GHashTable *valuesTable;
    GHashTable *typesTable;
    struct Scope *next;
    struct Scope *prev;

  } scope_list;

  typedef struct ParameterList
  {
    char *type;
    char *name;
    struct ParameterList *next;
  } par_list;

  typedef struct FunctionDefinition
  {
    pANTLR3_BASE_TREE treeBody;
    par_list *parameters;
    char *funName;
    char *func_ret_type;
    GHashTable *scopeValues;
    GHashTable *scopeTypes;
  } fun_def;

  void freeList(par_list * head)
  {
    par_list *tmp;

    while (head)
    {
      tmp = head;
      head = head->next;
      free(tmp);
    }
    head = NULL;
  }

  void free_function(gpointer function)
  {
    fun_def *func = (fun_def *)function;
    freeList(func->parameters);
    if (func->scopeValues)
      g_hash_table_unref(func->scopeValues);
    if (func->scopeTypes)
      g_hash_table_unref(func->scopeTypes);
    g_free(func);
  }

  //hashtable per i valori associati alle variabili
  GHashTable *valuesTable = g_hash_table_new_full(g_str_hash, g_str_equal, free_data, free_data);
  //hashtable per i tipi delle variabili
  GHashTable *typesTable = g_hash_table_new_full(g_str_hash, g_str_equal, free_data, free_data);
  //hashtable per le funzioni
  GHashTable *funcTable = g_hash_table_new_full(g_str_hash, g_str_equal, free_data, free_function);

  //funzione per inserire chiave e valore nella hashtable scelta, se la chiave e il valore
  //sono entrambi char* utilizzare hashInsertChar
  void hashInsert(char *keyChar, gpointer value, GHashTable *hashTable)
  {
    gchar *key = g_strdup(keyChar);
    if (!hashTable)
    {
      printf("hashtable is null");
      return;
    }
    if (!g_hash_table_contains(hashTable, (gconstpointer)key))
    {
      g_hash_table_insert(hashTable, key, value);
    }
    else
    {
      g_hash_table_insert(hashTable, key, value);
    }
  }

  void hashInsertChar(char *keyChar, char *valueChar, GHashTable *hashTable)
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
  gpointer hashGetVaue(char *key, GHashTable *hashTable, GHashTable *globalHashTable)
  {
    GHashTable *scope = hashTable;
    if (!hashTable)
    {
      printf("hashGetVaue: hashtable is null\n");
      return hashTable;
    }
    if (!g_hash_table_contains(hashTable, (gconstpointer)key))
    {
      if (globalHashTable)
      {
        if (g_hash_table_contains(globalHashTable, (gconstpointer)key))
        {
          scope = globalHashTable;
        }
      }
      else
      {
        printf("value %s not found\n", key);
        return NULL;
      }
    }
    gpointer value = g_hash_table_lookup(scope, (gconstpointer)key);
    if (value)
    {
      return value;
    }
    else
    {
      printf("%s 's value is NULL\n", key);
      return NULL;
    }
  }
  //funzione che richiede l'input dell'utente per stampare variabili, andare avanti nelle istruzioni passo passo
  void inputString(GHashTable * valTable, GHashTable * tyTable)
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
      sscanf(userInput1, "%s %s", pUserInput2, pUserInput3);

      if (strcmp(pUserInput2, "n") == 0)
      {
        printf("next line:\n");
      }
      else if (strcmp(pUserInput2, "h") == 0)
      {
        printf("'n' for next instruction\n");
        printf("'print'+ 'var name' to print variable name and type\n\n");
      }
      else if (strcmp(pUserInput2, "p") == 0)
      {
        printf("input p\n");
      }
      else if (pUserInput1[0] == 's' && userInput1[1] == ' ')
      {
        printf("userinput +2: %s\n", userInput1 + 2);
      }
      else if (strcmp(pUserInput2, "print") == 0)
      {
        char *varType;
        if (userInput3[0] != '\0')
        {
          varType = (char *)hashGetVaue(userInput3, tyTable, typesTable);
          if (!varType)
          {
            varType = (char *)hashGetVaue(userInput3, tyTable, typesTable);
          }
          if (varType)
          {
            if (strcmp(varType, "char") == 0 || strcmp(varType, "char*") == 0)
            {
              char *var = (char *)hashGetVaue(userInput3, valTable, valuesTable);
              if (!var)
                printf("value not found\n");

              printf("variabled %s:\n type: %s\n value %s\n", userInput3, varType, var);
            }
            else if (strcmp(varType, "int") || strcmp(varType, "int*"))
            {
              int *var = (int *)hashGetVaue(userInput3, valTable, valuesTable);
              if (!var)
                printf("value not found\n");
              if (varType && var)
                printf("variables %s:\n type: %s\n value %d\n", userInput3, varType, *var);
            }
          }
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
    if (token != NULL)
    {
      char *line = (char *)token->lineStart;
      int i = 0;
      int lineNumber = (int)token->line;
      printf("line %d: ", lineNumber);
      if (line)
      {
        while (line[i] != ';' && line[i] != '{')
        {
          if (line[i] == '\t')
          {
            continue;
          }
          printf("%c", line[i]);
          i = i + 1;
        }
      }
      printf("\n\n");
    }
  }

  // funzione che attraversa l'albero ed esegue le funzioni associate al tipo char
  char *charEvaluator(pANTLR3_BASE_TREE tree, GHashTable * valTable, GHashTable * tyTable)
  {
    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    if (tok)
    {
      switch (tok->type)
      {
      case ID:
      {
        char *var = tree->getText(tree)->chars;
        //check type
        //int value = atoi(hashGetVaue(var, valuesTable));
        //char *type = hashGetVaue(var, typesTable);
        char *value = (char *)hashGetVaue(var, valTable, NULL);

        return value;
      }
      case CHARACTER_LITERAL:
      {
        return tree->getText(tree)->chars;
      }
      case STRING_LITERAL:
      {
        return tree->getText(tree)->chars;
      }
      }
    }
  }

  // funzione che attraversa l'albero ed esegue le funzioni associate al tipo int
  int numberEvaluator(pANTLR3_BASE_TREE tree, GHashTable * valTable, GHashTable * tyTable)
  {
    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    if(!valTable){
      printf("valtable in number evaluator is null");
    }
    if (tok)
    {
      switch (tok->type)
      {
      case INT:
      {
        char *s = tree->getText(tree)->chars;
        int value = atoi(s);
        return value;
      }
      case ID:
      {
        char *var = tree->getText(tree)->chars;
        //check type
        //int value = atoi(hashGetVaue(var, valuesTable));
        int *value = (int *)hashGetVaue(var, valTable, valuesTable);
        if (!value)
          printf("qui null\n");
        return *value;
      }
      case PLUS:
      {
        return numberEvaluator(tree->getChild(tree, 0), valTable, tyTable) + numberEvaluator(tree->getChild(tree, 1), valTable, tyTable);
      }
      case MINUS:
      {
        return numberEvaluator(tree->getChild(tree, 0), valTable, tyTable) - numberEvaluator(tree->getChild(tree, 1), valTable, tyTable);
      }
      case MULT:
      {
        return numberEvaluator(tree->getChild(tree, 0), valTable, tyTable) * numberEvaluator(tree->getChild(tree, 1), valTable, tyTable);
      }
      case DIVI:
      {
        return numberEvaluator(tree->getChild(tree, 0), valTable, tyTable) / numberEvaluator(tree->getChild(tree, 1), valTable, tyTable);
      }
      case REST:
      {
        return numberEvaluator(tree->getChild(tree, 0), valTable, tyTable) % numberEvaluator(tree->getChild(tree, 1), valTable, tyTable);
      }
      }
    }
  }

  //funzione ausiliaria che aggiunge un oggetto par_list alla fine di una lista "head"
  void addEnd(par_list * *head, char *type, char *name)
  {

    par_list *newPar = (par_list *)malloc(sizeof(par_list));
    newPar->type = type;
    newPar->name = name;

    if (!*head)
    {
      *head = newPar;
      return;
    }
    par_list *temp = *head;
    while (temp->next)
    {
      temp = temp->next;
    }
    temp->next = newPar;

    return;
  }


  //funzione principale che attraversa l'albero e gestisce ogni azione
  int returnValue;

  void *evaluate(pANTLR3_BASE_TREE tree, GHashTable * valTable, GHashTable * tyTable)
  {
    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    if (tok)
    {
      switch (tok->type)
      {
      case INT:
      {
        //printf("token data int: %s\n", &tok->input->nextChar);
        //const char *s = tree->getText(tree)->chars;
        int value = numberEvaluator(tree, valTable, tyTable);
        return pTrue;
      }
    
      case CHARACTER_LITERAL:
      {
        return tree->getText(tree)->chars;
      }
      case STRING_LITERAL:
      {
        return tree->getText(tree)->chars;
      }
    case ID:
      {
        char *var = tree->getText(tree)->chars;
        //check type
        return hashGetVaue(var, valTable, valuesTable);

      }
      case EQEQ:
      {
        int firstValue = (int)numberEvaluator(tree->getChild(tree, 0), valTable, tyTable);
        int secondValue = (int)numberEvaluator(tree->getChild(tree, 1), valTable, tyTable);
        printLine(tok);
        inputString(valTable, tyTable);
        if (firstValue == secondValue)
        {
          return pTrue;
        }
        else
        {
          return NULL;
        }
      }
      case NEQ:
      {
        int firstValue = (int)numberEvaluator(tree->getChild(tree, 0), valTable, tyTable);
        int secondValue = (int)numberEvaluator(tree->getChild(tree, 1), valTable, tyTable);
        printLine(tok);
        inputString(valTable, tyTable);
        if (firstValue != secondValue)
        {
          return pTrue;
        }
        else
        {
          return NULL;
        }
      }
      case OPLT:
      {
        int firstValue = numberEvaluator(tree->getChild(tree, 0), valTable, tyTable);
        int secondValue = numberEvaluator(tree->getChild(tree, 1), valTable, tyTable);
        printLine(tok);
        inputString(valTable, tyTable);
        if (firstValue < secondValue)
        {
          return pTrue;
        }
        else
        {
          return pFalse;
        }
      }
      case PLUS:
      {
        returnValue = numberEvaluator(tree, valTable, tyTable);
        return &returnValue;
      }
      case MINUS:
      {
        returnValue = numberEvaluator(tree, valTable, tyTable);
        return &returnValue;
      }
      case MULT:
      {
        returnValue = numberEvaluator(tree, valTable, tyTable);
        return &returnValue;
      }
      case DIVI:
      {
        returnValue = numberEvaluator(tree, valTable, tyTable);
        return &returnValue;
      }
      case REST:
      {
        returnValue = numberEvaluator(tree, valTable, tyTable);
        return &returnValue;
      }
      case VAR_DEF:
      {
        //metti la variabile nella tabella hash con valore null
        pANTLR3_BASE_TREE child0 = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        char *variableType = child0->getText(child0)->chars;
        pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);
        char *variableName = child1->getText(child1)->chars;
        hashInsertChar(variableName, variableType, tyTable);
        hashInsert(variableName, NULL, valTable);
        printLine(child0->getToken(child0));
        inputString(valTable, tyTable);
        return pTrue;
      }
      case POI_DEF:
      {
        //puntatore e tipo nelle rispettive tabelle hash
        pANTLR3_BASE_TREE child0 = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        char *variableType = child0->getText(child0)->chars;
        pANTLR3_BASE_TREE child2 = (pANTLR3_BASE_TREE)tree->getChild(tree, 2);
        char *variableName = child2->getText(child2)->chars;
        char asterisk = '*';
        strncat(variableType, &asterisk, 1);
        hashInsertChar(variableName, variableType, typesTable);
        hashInsert(variableName, NULL, valuesTable);
        printLine(child0->getToken(child0));
        inputString(valTable, tyTable);
        return pTrue;
      }
      case EQ:
      {
        int i = 0;
        //caso in cui si faccia definizione e assegnamento insieme
        if (tree->getChildCount(tree) == 3)
        {
          pANTLR3_BASE_TREE child0 = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
           char *variableType;
          //caso di una variabile normale
          if(child0->getChildCount(child0) == 0){
            variableType = child0->getText(child0)->chars;
            }
          //caso di un puntatore
          else if(child0->getChildCount(child0) == 1){
            child0 = child0->getChild(child0, 0);
            variableType = child0->getText(child0)->chars;
            char asterisk = '*';
            strncat(variableType, &asterisk, 1);
          }
          pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);
          char *variableName = child1->getText(child1)->chars;
          hashInsertChar(variableName, variableType, tyTable);
          i = i + 1;
        }

        //caso di assegnamento
        pANTLR3_BASE_TREE child00 = (pANTLR3_BASE_TREE)tree->getChild(tree, i);
        char *variableName = child00->getText(child00)->chars;
        char *type = (char *)hashGetVaue(variableName, tyTable, NULL);
        //string var(getText(getChild(tree, 0)));
        if (!type){
          printf("error, no type found for variable\n");
          return pError;
        }
          if (strcmp(type, "int") == 0)
          {

            pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, i + 1);
            //caso di assegnamento tramite una funzione
            int value;
            if (child1->getToken(child1)->type == FUNC_CALL)
            {
              pANTLR3_BASE_TREE funNameTree = (pANTLR3_BASE_TREE)child1->getChild(child1, 0);
              char *funName = funNameTree->getText(funNameTree)->chars;

              fun_def *function = hashGetVaue(funName, funcTable, NULL);
              if (!function)
              {
                printf("function %s not found\n", funName);
                return pError;
              }
              char* fReturnType = function->func_ret_type;
              if(strcmp(fReturnType, "int") == 0){
                value = *(int *)evaluate(child1, valTable, tyTable);
              printf("func eq value: %d\n", value);
              }
              else if(strcmp(fReturnType, "int*") == 0){
                int* pValue = (int *)evaluate(child1, valTable, tyTable);
                }
              else if(strcmp(fReturnType, "char*") || strcmp(fReturnType, "char")){
                char* pValue = (char *)evaluate(child1, valTable, tyTable);
                }
            }
            else
            {
              value = numberEvaluator(child1, valTable, tyTable);
            }
            char strVal[12];
            int *pValue = malloc(sizeof(int));
            *pValue = value;
            printf("equation value : %d\n", *pValue);
            //sprintf(strVal, "%d", value);
            hashInsert(variableName, pValue, valuesTable);
            printLine(tok);
            inputString(valTable, tyTable);
            return pValue;
          }
          else if (strcmp(type, "char") == 0)
          {
            pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, i + 1);
            char *value = charEvaluator(child1, valTable, tyTable);
            char *pValue;
            pValue = malloc(sizeof(char));
            printf("value[1]:%c", value[1]);
            *pValue = value[1];
            hashInsert(variableName, pValue, valuesTable);
            printLine(tok);
            inputString(valTable, tyTable);
            return pTrue;
          }
          else if (strcmp(type, "char*") == 0)
          {
            pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, i + 1);
            char *value = charEvaluator(child1, valTable, tyTable);
            int valueSize = strlen(value);
            value[valueSize - 1] = '\0';
            value = value + 1;
            printf("value:%s\n", value);

            gpointer pValue = g_strdup(value);
            hashInsert(variableName, pValue, valuesTable);
            printLine(tok);
            inputString(valTable, tyTable);
            return pTrue;
          }
          else if (strcmp(type, "int*") == 0)
          {
            pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, i + 1);
            int value = numberEvaluator(child1, valTable, tyTable);
            char strVal[12];
            int *pValue;
            pValue = malloc(sizeof(int));
            *pValue = value;
            //sprintf(strVal, "%d", value);
            hashInsert(variableName, pValue, valuesTable);
            printLine(tok);
            inputString(valTable, tyTable);
            return pTrue;
          }
          else
          {
            printf("type not found\n");
            return pError;
          }
      }
      case IF_STAT:
      {
        //caso if senza else
        pANTLR3_BASE_TREE childIFELSE = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        pANTLR3_BASE_TREE childIF = (pANTLR3_BASE_TREE)childIFELSE->getChild(childIFELSE, 0);

        int *ifClauseValue = (int *)evaluate(childIF, valTable, tyTable);

        //condizione dell'if è vera quindi svolgo il codice nell'if
        if (ifClauseValue)
        {
          return evaluate((pANTLR3_BASE_TREE)childIFELSE->getChild(childIFELSE, 1), valTable, tyTable);
        }
        else if (tree->getChildCount(tree) > 1)
        {
          {
            pANTLR3_BASE_TREE childELSEIF = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);
            pANTLR3_BASE_TREE childELSE = (pANTLR3_BASE_TREE)childELSEIF->getChild(childELSEIF, 0);
            return evaluate(childELSE, valTable, tyTable);
          }
        }
      }
      case WHI_STAT:
      {
        pANTLR3_BASE_TREE conditionalExpr = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);

        int *conditionalExprEval = (int *)evaluate(conditionalExpr, valTable, tyTable);
        //int conditionalExprEval = evaluate(conditionalExpr);

        while (conditionalExprEval)
        {
          evaluate((pANTLR3_BASE_TREE)tree->getChild(tree, 1), valTable, tyTable);
          conditionalExprEval = evaluate(conditionalExpr, valTable, tyTable);
        }
        return conditionalExprEval;
      }
      case FUNC_DEF:
      {

        fun_def *pFun = g_malloc0(sizeof(fun_def));

        pANTLR3_BASE_TREE header = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        pANTLR3_BASE_TREE body = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);

        pFun->treeBody = body;
        //tipo della funzione
        pANTLR3_BASE_TREE funType = (pANTLR3_BASE_TREE)header->getChild(header, 0);
        char *funTypeChar = funType->getText(funType)->chars;
        pFun->func_ret_type = funTypeChar;

        //nome della funzione
        pANTLR3_BASE_TREE funName = (pANTLR3_BASE_TREE)header->getChild(header, 1);
        char *funNameChar = funName->getText(funName)->chars;
        pFun->funName = funNameChar;

        int k = header->getChildCount(header);
        printf("number of parameters: %d \n", k - 2);

        par_list *parameter_list_head = NULL;
        for (int i = 2; i < k; i++)
        {
          pANTLR3_BASE_TREE paramTree = (pANTLR3_BASE_TREE)header->getChild(header, i);

          pANTLR3_BASE_TREE parType = (pANTLR3_BASE_TREE)paramTree->getChild(paramTree, 0);
          pANTLR3_BASE_TREE parName = (pANTLR3_BASE_TREE)paramTree->getChild(paramTree, 1);
          //parameter->type = partype->getText(parType)->chars;
          //parameter->name = parname->getText(parname)->chars;

          char *parTypeChar = parType->getText(parType)->chars;
          char *parNameChar = parName->getText(parName)->chars;

          addEnd(&parameter_list_head, parTypeChar, parNameChar);
        }
        pFun->parameters = parameter_list_head;

        par_list *temp = parameter_list_head;

        while (temp && temp->next)
        {
          printf("param: %s %s ", pFun->parameters->type, temp->name);
          temp = pFun->parameters->next;
        }
        printf("param: %s %s\n", temp->type, temp->name);

        hashInsert(funNameChar, pFun, funcTable);
        return pTrue;
      }
      case FUNC_CALL:
      {
        pANTLR3_BASE_TREE fName = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        char *fNameChar = fName->getText(fName)->chars;
        int nParams = tree->getChildCount(tree);
        fun_def *function = (fun_def *)hashGetVaue("fun", funcTable, NULL);
        char* fReturnType = function->func_ret_type;
        if (!function)
        {
          printf("function %s not found\n", fNameChar);
          return 0;
        }

        function->scopeValues = g_hash_table_new_full(g_str_hash, g_str_equal, free_data, free_data);
        function->scopeTypes = g_hash_table_new_full(g_str_hash, g_str_equal, free_data, free_data);
        par_list *temp = function->parameters;
        char *paramType;
        char *paramName;
        for (int i = 1; i < nParams; i++)
        {
          /* metti i parametri nello scope */
          paramType = temp->type;
          paramName = temp->name;
          if (strcmp(paramType, "int") == 0 || strcmp(paramType, "int*") == 0)
          {
            int *paramValue = malloc(sizeof(int));
            pANTLR3_BASE_TREE paramValueTree = (pANTLR3_BASE_TREE)tree->getChild(tree, i);
            *paramValue = numberEvaluator(paramValueTree, valTable, tyTable);
            printf("paramtype: %s ", paramType);
            printf("paramValue: %d\n", *paramValue);
            hashInsert(paramName, paramValue, function->scopeValues);
          }
          else if (strcmp(paramType, "char") == 0 || strcmp(paramType, "char*") == 0)
          {
            char *paramValue = charEvaluator((pANTLR3_BASE_TREE)tree->getChild(tree, i), valTable, tyTable);
            hashInsert(paramName, paramValue, function->scopeValues);
          }

            hashInsertChar(paramName, paramType, function->scopeTypes);
          temp = temp->next;
        }
        printLine(fName->getToken(fName));
        inputString(valTable, tyTable);
        if(strcmp(fReturnType, "int") == 0 || strcmp(fReturnType, "int*") == 0)
          return (int*)evaluate(function->treeBody, function->scopeValues, function->scopeTypes);
        if(strcmp(fReturnType, "char")== 0 || strcmp(fReturnType, "char*") == 0)
          return (char*)evaluate(function->treeBody, function->scopeValues, function->scopeTypes);

      }
      case RET:
      {
        pANTLR3_BASE_TREE expr = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        return evaluate(expr, valTable, tyTable);
      }
      case BLOCK:
      {
        int k = tree->getChildCount(tree);
        int *r = NULL;
        for (int i = 0; i < k; i++)
        {
          r = evaluate(tree->getChild(tree, i), valTable, tyTable);
        }
        return r;
      }
      default:
        printf("Unhandled token: %d\n", tok->type);
        return pError;
      }
    }
    else
    {
      int k = tree->getChildCount(tree);
      int *r = NULL;
      for (int i = 0; i < k; i++)
      {

        pANTLR3_BASE_TREE child = tree->getChild(tree, i);
        r = evaluate(child, valTable, tyTable);

        if (child->getToken(child)->type == RET)
        {
          break;
        }
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
    evaluate(rootTree, valuesTable, typesTable);

    //libero la memoria allocata dinamicamente
    g_hash_table_remove_all(typesTable);
    g_hash_table_remove_all(valuesTable);
    g_hash_table_remove_all(funcTable);

    g_hash_table_unref(typesTable);
    g_hash_table_unref(valuesTable);
    g_hash_table_unref(funcTable);

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
