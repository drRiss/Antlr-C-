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
void *pNull = NULL;

int *pTrue = &true;
int *pFalse = &false;
int *pError = &error;
void *returnVal = NULL;

int ANTLR3_CDECL main(int argc, char *argv[])
{

  void free_data(gpointer data)
  {
    g_free(data);
  }

  typedef struct ScopesTree
  {
    //fare un puntatore per i figli non è necessario in quanto dal parent non si passa mai ai figli
    struct ScopesTree *parent;

    GHashTable *scope;

    int depth;
  } scope_tree;

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
  } fun_def;

  typedef struct Variable
  {
    char *name;
    char *type;
    gpointer value;
    //posizione nella lista
    int position;

    int isPointer;
    char *pointsTo;  // nome della variabile a cui punta
    int pointsToPos; //posizione della variabile
    struct Variable *prev;
    struct Variable *next;

  } var_list;

  void freeVarList(gpointer listHead)
  {
    var_list *head = (var_list *)listHead;
    var_list *tmp;
    //tolgo la ciclicità della lista
    if (head->prev)
    {
      head->prev->next = NULL;
    }
    while (head)
    {

      tmp = head;
      head = head->next;
      //free(tmp->name);
      //free(tmp->type);
      //free(tmp->value);
      tmp->next = NULL;
      tmp->prev = NULL;

      free(tmp);
    }
    head = NULL;
  }

  typedef struct History
  {
    //punto attuale dell'albero sintattico
    pANTLR3_BASE_TREE subTree;
    //lista variabili scritte in quel punto dell'albero sintattico
    struct Variable *written;
    //lista variabili lette in quel punto dell'albero sintattico
    struct Variable *read;
    void *token; //token per stampare la linea cui si riferisce

    struct ScopesTree *scopeTree;

    struct History *prev;
    struct History *next;

  } History;

  void freeHistory(History * *pHistory)
  {
    History *freeHistory = *pHistory;
    History *temp;

    while (freeHistory)
    {
      temp = freeHistory;
      temp->subTree = NULL;
      temp->token = NULL;
      if (temp->written)
      {
        freeVarList(temp->written);
        //free(temp->written);
      }
      if (temp->read)
      {
        freeVarList(temp->read);
        //free(temp->read);
      }

      freeHistory = freeHistory->next;

      free(temp);
    }
  }

  void freeListPar(par_list * head)
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
    freeListPar(func->parameters);
    if (func->scopeValues)
      g_hash_table_unref(func->scopeValues);
    g_free(func);
  }

  //hashtable per i valori associati alle variabili
  GHashTable *valuesTable = g_hash_table_new_full(g_str_hash, g_str_equal, free_data, free_data);
  //hashtable per le funzioni
  GHashTable *funcTable = g_hash_table_new_full(g_str_hash, g_str_equal, free_data, free_function);

  History *isHistory = malloc(sizeof(History));
  isHistory->subTree = NULL;
  isHistory->written = NULL;
  isHistory->read = NULL;
  isHistory->token = NULL;
  isHistory->prev = NULL;
  isHistory->next = NULL;

  scope_tree *scopeTree = malloc(sizeof(scope_tree));
  scopeTree->parent = NULL;
  scopeTree->scope = valuesTable;

  void insertWrittenHistory(History * *pHistory, struct Variable * varHistory)
  {
    History *history = *pHistory;
    if (!history->written)
    {
      history->written = malloc(sizeof(var_list));
      history->written->name = varHistory->name;
      history->written->type = varHistory->type;
      history->written->position = varHistory->position;
      history->written->next = history->written;
      history->written->prev = history->written;
    }
    else
    { //inserisco in coda
      var_list *tempV = malloc(sizeof(var_list));
      tempV->name = varHistory->name;
      tempV->type = varHistory->type;
      tempV->position = varHistory->position;
      tempV->next = history->written;
      tempV->prev = history->written->prev;
      history->written->prev = tempV;
      tempV->prev->next = tempV;
    }
  }

  //funzione per inserire chiave e valore nella hashtable scelta, se la chiave e il valore
  //sono entrambi char* utilizzare hashInsertChar
  void hashInsert(char *keyChar, gpointer value, GHashTable *hashTable)
  {
    gchar *key = g_strdup(keyChar);
    if (!hashTable)
    {
      printf("hashtable is null\n");
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
  //globalh non utilizzato da rivedere
  void hashInsertVar(char *keyChar, char *type, gpointer value, scope_tree *scopeTree, History **pHistory, int save)
  {
    History *history = *pHistory;
    gchar *key = g_strdup(keyChar);
    if (!scopeTree || !scopeTree->scope)
    {
      printf("hashtable is null\n");
      return;
    }
    struct Variable *pVar = NULL;
    if (!g_hash_table_contains(scopeTree->scope, (gconstpointer)key))
    { //primo inserimento della variabile
      pVar = malloc(sizeof(struct Variable));
      pVar->name = key;
      pVar->type = type;
      pVar->position = 1;
      pVar->next = pVar;
      pVar->prev = pVar;
      pVar->isPointer = 0;
      //caso in cui la variabile sia di tipo char, bisogna duplicarla perchè non venga persa
      if (strcmp(type, "char*") == 0 || strcmp(type, "char") == 0)
      {
        char *valueChar = g_strdup(value);
        pVar->value = (gpointer)valueChar;
        g_hash_table_insert(scopeTree->scope, key, pVar);
      }
      else if (strcmp(type, "int") == 0)
      {
        pVar->value = (gpointer)value;
        g_hash_table_insert(scopeTree->scope, key, pVar);
      }
      else if (strcmp(type, "int*") == 0)
      {
        pVar->isPointer = 1; //true
        if (value)
        {
          var_list *varToPointTo = (var_list *)value;
          char *vtptName = varToPointTo->name;
          pVar->pointsTo = vtptName;
          pVar->pointsToPos = varToPointTo->prev->position;
        }
        else
        {
          pVar->pointsTo = NULL;
        }
        g_hash_table_insert(scopeTree->scope, key, pVar);
      }
    }
    else
    {
      //variabile già presente valore da aggiungere
      var_list *list_head = (var_list *)g_hash_table_lookup(scopeTree->scope, (gconstpointer)key);
      if (!list_head)
      {
        printf("value is not found in this scope\n");
        return;
      }
      var_list *temp = list_head;
      while (list_head != temp->next)
      {
        temp = temp->next;
      }
      pVar = malloc(sizeof(var_list));
      pVar->name = key;
      pVar->type = type;
      if (strcmp(type, "char*") == 0 || strcmp(type, "char") == 0)
      {
        char *valueChar = g_strdup(value);
        pVar->value = (gpointer)valueChar;
      }
      else if (strcmp(type, "int") == 0)
      {
        pVar->value = (gpointer)value;
      }
      else if (strcmp(type, "int*") == 0)
      {
        pVar->isPointer = 1; //true

        if (value)
        {
          var_list *varToPointTo = (var_list *)value;
          char *vtptName = varToPointTo->name;
          pVar->pointsTo = vtptName;
          pVar->pointsToPos = varToPointTo->prev->position;
        }
        else
        {
          pVar->pointsTo = NULL;
        }
      }

      temp->next = pVar;
      pVar->prev = temp;
      pVar->next = list_head;
      list_head->prev = pVar;
      pVar->position = temp->position + 1;
    }
    if (save)
      insertWrittenHistory(pHistory, pVar);
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
        printf("function %s not found\n", key);
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

  gpointer hashGetVaueVar(char *key, scope_tree *scopeTree, int position, History **pHistory, int save)
  {
    History *history = *pHistory;
    GHashTable *scope = scopeTree->scope;
    if (!scopeTree || !scopeTree->scope)
    {
      printf("hashGetVaue: hashtable is null\n");
      return NULL;
    }
    //cerco nell'albero se esiste il valore della chiave
    scope_tree *temp = scopeTree;

    while (!g_hash_table_contains(temp->scope, (gconstpointer)key))
    {
      if (temp->parent)
      {
        temp = temp->parent;

        if (g_hash_table_contains(temp->scope, (gconstpointer)key))
        {
          scope = temp->scope;
        }
      }
      else
      {
        printf("value %s not found\n", key);
        return NULL;
      }
    }

    var_list *valueList = (var_list *)g_hash_table_lookup(scope, (gconstpointer)key);
    if (save)
    {
      if (!history->read)
      {
        var_list *tempV = malloc(sizeof(var_list));
        tempV->name = key;
        tempV->type = valueList->type;
        tempV->position = valueList->prev->position;
        tempV->next = tempV;
        tempV->prev = tempV;
        history->read = tempV;
      }
      else
      { //inserisco il valore in coda
        var_list *tempV = malloc(sizeof(var_list));
        tempV->name = key;
        tempV->type = valueList->type;
        tempV->position = valueList->prev->position;
        tempV->next = history->read;
        tempV->prev = history->read->prev;
        history->read->prev = tempV;
        tempV->prev->next = tempV;
      }
    }
    return valueList;
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
          if (line[i])
          {
            printf("%c", line[i]);
          }
          i = i + 1;
        }
      }
      printf("\n\n");
    }
  }

  //funzione che richiede l'input dell'utente per stampare variabili, andare avanti nelle istruzioni passo passo
  void inputString(scope_tree * scope, History * *pHistory)
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
        //do nothing, continue with the istructions
        printf("next line:\n");
      }
      else if (strcmp(pUserInput2, "prev") == 0)
      {
      }
      else if (strcmp(pUserInput2, "h") == 0)
      {
        printf("'n' for next instruction\n");
        printf("'print'+ 'var name' to print variable name and type\n\n");
      }
      else if (strcmp(pUserInput2, "p") == 0)
      {
        History *history = *pHistory;
        if (!history)
        {
          printf("no instructions to rewind\n");
          return;
        }
        var_list *temp = history->written;
        if (temp)
        {
          do
          {
            char *varWritten = temp->name;
            printf("varWritten: %s\n", varWritten);

            var_list *varList = (var_list *)hashGetVaueVar(varWritten, history->scopeTree, -1, pHistory, false);
            if (varList)
            {
              //rimuovo l'ultima occorrenza della variabile
              var_list *toFree = varList->prev;
              toFree->prev->next = varList;
              toFree->next->prev = toFree->prev;
              free(toFree);
            }
            temp = temp->next;

          } while (temp != history->written);
        }
        if (history->prev)
        {
          *pHistory = history->prev;
          history = *pHistory;
        }

        printLine(history->subTree->getToken(history->subTree));
        printf("input p\n");
      }
      //da togliere
      else if (pUserInput1[0] == 's' && userInput1[1] == ' ')
      {
        printf("userinput +2: %s\n", userInput1 + 2);
      }
      else if (strcmp(pUserInput2, "print") == 0)
      {
        char *varType;
        if (userInput3[0] != '\0')
        {
          var_list *varList = (var_list *)hashGetVaueVar(userInput3, scope, -1, pHistory, false);
          if (varList)
          {
            varType = (char *)varList->type;
            printf("variabel type in inputstring: %s\n", varType);
            printf("variabel value in inputstring: %s\n", (char *)varList->prev->value);
            if (strcmp(varType, "char") == 0 || strcmp(varType, "char*") == 0)
            {
              char *var = (char *)varList->prev->value;
              if (!var)
              {
                printf("value not found\n");
              }
              printf("variabled %s:\n type: %s\n value %s\n", userInput3, varType, var);
            }
            else if (strcmp(varType, "int") == 0)
            {
              int *var = (int *)varList->prev->value;
              if (!var)
                printf("value not found\n");
              if (varType && var)
                printf("variables %s:\n type: %s\n value %d\n", userInput3, varType, *var);
            }
            else if (strcmp(varType, "int*") == 0)
            {
              char *varPointingTo = varList->prev->pointsTo;
              int varPToPos;
              int *valuePointed;
              if (varPointingTo)
              {
                varPToPos = varList->prev->pointsToPos;
                var_list *varPointed = (var_list *)hashGetVaueVar(varPointingTo, scope, -1, pHistory, false);
                valuePointed = (int *)varPointed->prev->value;
                printf("int pointer pointing to variable: %s\n  %s = %d\n", varPointingTo, varPointingTo, *valuePointed);
              }
              else
              {
                valuePointed = pNull;
                printf("int pointer pointing to nothing \n");
              }
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

  // funzione che attraversa l'albero ed esegue le funzioni associate al tipo char
  char *charEvaluator(pANTLR3_BASE_TREE tree, scope_tree * scope, History * *pHistory)
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
        var_list *varL = (var_list *)hashGetVaueVar(var, scope, -1, pHistory, true);
        char *value = (char *)varL->type;

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
  int numberEvaluator(pANTLR3_BASE_TREE tree, scope_tree * scope, History * *pHistory)
  {

    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    if (!scope || !scope->scope)
    {
      printf("scope in number evaluator is null");
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
        var_list *varL = (var_list *)hashGetVaueVar(var, scope, -1, pHistory, true);
        if (!varL)
        {
          printf("%s not found in scope\n", var);
        }
        int *value = (int *)varL->prev->value;
        if (!value)
        {
          printf("%s value is null or not initializedd\n", var);
          //return 0;
        }
        return *value;
      }
      case UN_OP:
      {
        //da fare caso con '*' dereferenziazione
        pANTLR3_BASE_TREE unop = (pANTLR3_BASE_TREE)tree->getChild(tree, 0); //controllo se si tratta di & o *
        if (strcmp(unop->getText(unop)->chars, "*") != 0)
        {
          printf("wrong type assigned to int at line %d/n", *(int *)unop->getToken(unop)->getLine);
        }
        pANTLR3_BASE_TREE rhs_id_tree = (pANTLR3_BASE_TREE)tree->getChild(tree, 1); //controllo se si tratta di & o *
        char *rhs_id = rhs_id_tree->getText(rhs_id_tree)->chars;
        var_list *varL = (var_list *)hashGetVaueVar(rhs_id, scope, -1, pHistory, true);
        if (!varL->isPointer)
        {
          printf("dereferencing a non pointer variable: %s\n", rhs_id); //da rivedere con char*
          return 0;
        }
        char *rhs_points_to = varL->prev->pointsTo;
        var_list *varPointedTo = (var_list *)hashGetVaueVar(rhs_points_to, scope, -1, pHistory, true);

        int *value = (int *)varPointedTo->prev->value;
        if (!value)
        {
          printf("%s value is null or not initializedu\n", rhs_id);
          //return 0;
        }
        return *value;
      }
      case PLUS:
      {
        return numberEvaluator(tree->getChild(tree, 0), scope, pHistory) + numberEvaluator(tree->getChild(tree, 1), scope, pHistory);
      }
      case MINUS:
      {
        return numberEvaluator(tree->getChild(tree, 0), scope, pHistory) - numberEvaluator(tree->getChild(tree, 1), scope, pHistory);
      }
      case MULT:
      {
        return numberEvaluator(tree->getChild(tree, 0), scope, pHistory) * numberEvaluator(tree->getChild(tree, 1), scope, pHistory);
      }
      case DIVI:
      {
        return numberEvaluator(tree->getChild(tree, 0), scope, pHistory) / numberEvaluator(tree->getChild(tree, 1), scope, pHistory);
      }
      case REST:
      {
        return numberEvaluator(tree->getChild(tree, 0), scope, pHistory) % numberEvaluator(tree->getChild(tree, 1), scope, pHistory);
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

  void *evaluate(pANTLR3_BASE_TREE tree, scope_tree * *pScope, History * *pHistory, int saveHistory)
  {
    scope_tree *scope = *pScope;
    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    if (tok)
    {
      History *history = *pHistory;
      if (saveHistory)
      {
        //istanzio history per salvare le azioni svolte
        if (!history)
        {
          history = malloc(sizeof(History));
          history->subTree = tree;
          history->written = NULL;
          history->read = NULL;
          history->prev = NULL;
          history->next = NULL;
          history->scopeTree = scope;
          *pHistory = history;
        }
        else
        {
          if (!history->next)
          { //se esiste history->next vuol dire che sono tornato indietro (p) e sto tornando avanti (n)
            History *tempH = malloc(sizeof(History));
            tempH->subTree = tree;
            tempH->written = NULL;
            tempH->read = NULL;
            tempH->prev = history;
            tempH->next = NULL;
            tempH->scopeTree = *pScope;

            history->next = tempH;
          }
          else
          { //elimino gli eventuali salvataggi fatti da history precedentemente
            if (history->next->written)
            {
              freeVarList(history->next->written);
              //free(history->next->written);
              history->next->written = NULL;
            }
            if (history->next->written)
            {
              freeVarList(history->next->read);
              //free(history->next->read);
              history->next->read = NULL;
            }
          }
          *pHistory = history->next;
        }
      }
      switch (tok->type)
      {
      case INT:
      {
        //printf("token data int: %s\n", &tok->input->nextChar);
        //const char *s = tree->getText(tree)->chars;
        int value = numberEvaluator(tree, scope, pHistory);
        returnVal = &value;
        return returnVal;
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
        var_list *varL = (var_list *)hashGetVaueVar(var, scope, -1, pHistory, true);
        if (!varL)
        {
          printf("no values for %s found\n", var);
        }

        return varL->prev->value;
      }
      case UN_OP:
      {
        //dcontrollo '*' dereferenziazione
        pANTLR3_BASE_TREE unop = (pANTLR3_BASE_TREE)tree->getChild(tree, 0); //controllo se si tratta di & o *
        if (strcmp(unop->getText(unop)->chars, "*") != 0)
        {
        }
        pANTLR3_BASE_TREE rhs_id_tree = (pANTLR3_BASE_TREE)tree->getChild(tree, 1); //controllo se si tratta di & o *
        char *rhs_id = rhs_id_tree->getText(rhs_id_tree)->chars;
        var_list *varL = (var_list *)hashGetVaueVar(rhs_id, scope, -1, pHistory, true);
        if (!varL->isPointer)
        {
          printf("dereferencing a non pointer variable: %s\n", rhs_id); //da rivedere con char*
          return 0;
        }
        char *rhs_points_to = varL->prev->pointsTo;
        var_list *varPointedTo = (var_list *)hashGetVaueVar(rhs_points_to, scope, -1, pHistory, true);

        return varPointedTo;
      }
      case EQEQ:
      {
        int firstValue = (int)numberEvaluator(tree->getChild(tree, 0), scope, pHistory);
        int secondValue = (int)numberEvaluator(tree->getChild(tree, 1), scope, pHistory);
        printLine(tok);
        inputString(scope, pHistory);
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
        int firstValue = (int)numberEvaluator(tree->getChild(tree, 0), scope, pHistory);
        int secondValue = (int)numberEvaluator(tree->getChild(tree, 1), scope, pHistory);
        printLine(tok);
        inputString(scope, pHistory);
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
        int firstValue = numberEvaluator(tree->getChild(tree, 0), scope, pHistory);
        int secondValue = numberEvaluator(tree->getChild(tree, 1), scope, pHistory);
        printLine(tok);
        inputString(scope, pHistory);
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
        returnValue = numberEvaluator(tree, scope, pHistory);
        return &returnValue;
      }
      case MINUS:
      {
        returnValue = numberEvaluator(tree, scope, pHistory);
        return &returnValue;
      }
      case MULT:
      {
        returnValue = numberEvaluator(tree, scope, pHistory);
        return &returnValue;
      }
      case DIVI:
      {
        returnValue = numberEvaluator(tree, scope, pHistory);
        return &returnValue;
      }
      case REST:
      {
        returnValue = numberEvaluator(tree, scope, pHistory);
        return &returnValue;
      }
      case VAR_DEF:
      {
        //metti la variabile nella tabella hash con valore null
        pANTLR3_BASE_TREE child0 = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        char *variableType = child0->getText(child0)->chars;
        pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);
        char *variableName = child1->getText(child1)->chars;
        hashInsertVar(variableName, variableType, pNull, scope, pHistory, true);
        printLine(child0->getToken(child0));
        inputString(scope, pHistory);
        return pTrue;
      }
      case POI_DEF:
      {
        //puntatore e tipo nelle rispettive tabelle hash
        pANTLR3_BASE_TREE child0 = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        pANTLR3_BASE_TREE child00 = (pANTLR3_BASE_TREE)child0->getChild(child0, 0);
        char *variableType = child00->getText(child00)->chars;
        pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);
        char *variableName = child1->getText(child1)->chars;
        char asterisk = '*';
        strncat(variableType, &asterisk, 1);
        printf("variable type = %s\n", variableType);
        hashInsertVar(variableName, variableType, pNull, scope, pHistory, true);
        printLine(child00->getToken(child00));
        inputString(scope, pHistory);
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
          if (child0->getChildCount(child0) == 0)
          {
            variableType = child0->getText(child0)->chars;
          }
          //caso di un puntatore
          else if (child0->getChildCount(child0) == 1)
          {
            child0 = child0->getChild(child0, 0);
            variableType = child0->getText(child0)->chars;
            char asterisk = '*';
            strncat(variableType, &asterisk, 1);
            printf("type1  = %s\n", variableType);
          }
          pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);
          char *variableName = child1->getText(child1)->chars;
          printf("type2 = %s\n", variableType);

          hashInsertVar(variableName, variableType, pNull, scope, pHistory, false); // da non inserire come written
          i = i + 1;
        }

        //caso di assegnamento
        pANTLR3_BASE_TREE child00 = (pANTLR3_BASE_TREE)tree->getChild(tree, i);
        char *variableName = child00->getText(child00)->chars;
        var_list *varL = (var_list *)hashGetVaueVar(variableName, scope, -1, pHistory, false);

        char *type = (char *)varL->type;
        printf("type = %s\n", type);
        //string var(getText(getChild(tree, 0)));
        if (!type)
        {
          printf("error, no type found for variable\n");
          return pError;
        }
        if (strcmp(type, "int") == 0)
        {
          //da rivedere assegnamento tramite funzione per i tipi non int

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
            char *fReturnType = function->func_ret_type;
            if (strcmp(fReturnType, "int") == 0)
            {
              value = *(int *)evaluate(child1, pScope, pHistory, 0);
              printf("func eq value: %d\n", value);
            }
            else if (strcmp(fReturnType, "int*") == 0)
            {
              int *pValue = (int *)evaluate(child1, pScope, pHistory, 0);
            }
            else if (strcmp(fReturnType, "char*") || strcmp(fReturnType, "char"))
            {
              char *pValue = (char *)evaluate(child1, pScope, pHistory, 0);
            }
          }
          else
          {
            value = numberEvaluator(child1, scope, pHistory);
          }
          char strVal[12];
          int *pValue = malloc(sizeof(int));
          *pValue = value;
          printf("equation right value : %d\n", *pValue);
          //sprintf(strVal, "%d", value);
          hashInsertVar(variableName, type, pValue, scope, pHistory, true);
          printLine(tok);
          inputString(scope, pHistory);
          return pValue;
        }

        else if (strcmp(type, "char") == 0)
        {
          pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, i + 1);
          char *value = charEvaluator(child1, scope, pHistory);
          char *pValue;
          pValue = malloc(sizeof(char));
          printf("value[1]:%c", value[1]);
          *pValue = value[1];
          hashInsertVar(variableName, type, pValue, scope, pHistory, true);
          printLine(tok);
          inputString(scope, pHistory);
          return pValue;
        }
        else if (strcmp(type, "char*") == 0)
        {
          pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, i + 1);
          char *value = charEvaluator(child1, scope, pHistory);
          int valueSize = strlen(value);
          value[valueSize - 1] = '\0';
          value = value + 1;

          gpointer pValue = g_strdup(value);
          hashInsertVar(variableName, type, pValue, scope, pHistory, true);
          printLine(tok);
          inputString(scope, pHistory);
          return pValue;
        }

        else if (strcmp(type, "int*") == 0)
        {
          //si aspetta un puntatore come rhs
          var_list *pValue;
          pValue = malloc(sizeof(var_list));
          pANTLR3_BASE_TREE child1 = (pANTLR3_BASE_TREE)tree->getChild(tree, i + 1); // right hand side
          printf("child1 %s\n", child1->toStringTree(child1)->chars);
          //o idp o &variabile o puntatore alla variabile
          //1)
          //da rivedere
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
            var_list *pValue = (var_list *)evaluate(child1, pScope, pHistory, 0);
            printf("test pValue int* func in line 1103 %s\n", pValue->name);
            hashInsertVar(variableName, type, pValue, scope, pHistory, true); //ispointer
          }

          else if (child1->getToken(child1)->type == UN_OP)
          {
            pANTLR3_BASE_TREE unop = (pANTLR3_BASE_TREE)child1->getChild(child1, 0); //controllo se si tratta di & o *
            if (strcmp(unop->getText(unop)->chars, "&") != 0)
            {
              printf("wrong type assigned to pointer %s\n", variableName);
            }
            pANTLR3_BASE_TREE rhs_id_tree = (pANTLR3_BASE_TREE)child1->getChild(child1, 1); //controllo se si tratta di & o *
            char *rhs_id = rhs_id_tree->getText(rhs_id_tree)->chars;
            *pValue = *(var_list *)hashGetVaueVar(rhs_id, scope, -1, pHistory, true);
            printf("test pValue int* unop %s\n", pValue->name);
            printf("pvalue unop; %d\n", *(int *)pValue->prev->value);
            hashInsertVar(variableName, type, pValue, scope, pHistory, true); //ispointer
          }

          else if (child1->getToken(child1)->type == ID)
          {
            char *rhs_id = child1->getText(child1)->chars;
            //prendo la variabile a cui punta il puntatore a destra dell'uguale
            var_list *pointerValue = (var_list *)hashGetVaueVar(rhs_id, scope, -1, pHistory, true);
            *pValue = *(var_list *)hashGetVaueVar(pointerValue->prev->pointsTo, scope, -1, pHistory, true);
            printf("test pValue int* id %s\n", pValue->prev->pointsTo);
            //lo inserisco come oggetto puntato dal puntatore a sinistra dell'uguale
            hashInsertVar(variableName, type, pValue, scope, pHistory, true); //ispointer
          }

          printLine(tok);
          inputString(scope, pHistory);
          return pValue;
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

        int *ifClauseValue = (int *)evaluate(childIF, pScope, pHistory, 1);

        //condizione dell'if è vera quindi svolgo il codice nell'if
        if (ifClauseValue)
        {
          return evaluate((pANTLR3_BASE_TREE)childIFELSE->getChild(childIFELSE, 1), pScope, pHistory, 1);
        }
        else if (tree->getChildCount(tree) > 1)
        {
          {
            pANTLR3_BASE_TREE childELSEIF = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);
            pANTLR3_BASE_TREE childELSE = (pANTLR3_BASE_TREE)childELSEIF->getChild(childELSEIF, 0);
            return evaluate(childELSE, pScope, pHistory, 1);
          }
        }
      }
      case WHI_STAT:
      {
        pANTLR3_BASE_TREE conditionalExpr = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        int *conditionalExprEval = (int *)evaluate(conditionalExpr, pScope, pHistory, 1);

        while (conditionalExprEval)
        {
          evaluate((pANTLR3_BASE_TREE)tree->getChild(tree, 1), pScope, pHistory, 1);
          conditionalExprEval = evaluate(conditionalExpr, pScope, pHistory, 1);
        }
        return conditionalExprEval;
      }
      case FUNC_DEF:
      {
        //funcTable come variable globale(da passa recome parametro se si sposta)

        pANTLR3_BASE_TREE header = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        pANTLR3_BASE_TREE body = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);
        pANTLR3_BASE_TREE funType = (pANTLR3_BASE_TREE)header->getChild(header, 0);
        char *funTypeChar = funType->getText(funType)->chars;
        pANTLR3_BASE_TREE funName = (pANTLR3_BASE_TREE)header->getChild(header, 1);

        if (!g_hash_table_contains(funcTable, (gconstpointer)funName))
        {

          fun_def *pFun = g_malloc0(sizeof(fun_def));

          pFun->func_ret_type = funTypeChar;
          pFun->treeBody = body;

          //nome della funzione
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
          hashInsert(funNameChar, pFun, funcTable);
          return pTrue;
        }
        else
          return pFalse;
      }
      case FUNC_CALL:
      {
        pANTLR3_BASE_TREE fName = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        char *fNameChar = fName->getText(fName)->chars;
        int nParams = tree->getChildCount(tree);
        fun_def *function = (fun_def *)hashGetVaue(fNameChar, funcTable, NULL);
        char *fReturnType = function->func_ret_type;
        if (!function)
        {
          printf("function %s not found\n", fNameChar);
          return 0;
        }

        function->scopeValues = g_hash_table_new_full(g_str_hash, g_str_equal, free_data, freeVarList);
        par_list *temp = function->parameters;
        char *paramType;
        char *paramName;
        //inserimento nell'albero degli scope
        scope_tree *scopeChild = malloc(sizeof(scope_tree));
        scopeChild->parent = *pScope;
        scopeChild->scope = function->scopeValues;
        History *tempH = *pHistory;
        tempH->scopeTree = scopeChild;

        for (int i = 1; i < nParams; i++)
        {
          /* metti i parametri nello scope */
          paramType = temp->type;
          paramName = temp->name;
          if (strcmp(paramType, "int") == 0)
          {
            int *paramValue = malloc(sizeof(int));
            pANTLR3_BASE_TREE paramValueTree = (pANTLR3_BASE_TREE)tree->getChild(tree, i);
            *paramValue = numberEvaluator(paramValueTree, scopeChild, pHistory);
            printf("paramtype: %s ", paramType);
            printf("paramValue: %d\n", *paramValue);
            hashInsertVar(paramName, paramType, paramValue, scopeChild, pHistory, false);
          }
          else if (strcmp(paramType, "int*") == 0)
          {
            int *paramValue = malloc(sizeof(int));
            pANTLR3_BASE_TREE paramValueTree = (pANTLR3_BASE_TREE)tree->getChild(tree, i);
            paramValue = evaluate(paramValueTree, pScope, pHistory, 1); //da rivedere -> evaluatepointer
            printf("paramtype: %s ", paramType);
            printf("paramValue: %d\n", *paramValue);
            hashInsertVar(paramName, paramType, paramValue, scopeChild, pHistory, false);
          }
          else if (strcmp(paramType, "char") == 0 || strcmp(paramType, "char*") == 0)
          {
            printf("paramtype: %s ", paramType);
            char *paramValue = charEvaluator((pANTLR3_BASE_TREE)tree->getChild(tree, i), scope, pHistory);
            printf("paramValue: %d\n", *paramValue);
            hashInsertVar(paramName, paramType, paramValue, scopeChild, pHistory, false);
          }
          temp = temp->next;
        }
        printLine(fName->getToken(fName));
        inputString(scope, pHistory);
        if (strcmp(fReturnType, "int") == 0 || strcmp(fReturnType, "int*") == 0)
          return (int *)evaluate(function->treeBody, &scopeChild, pHistory, 0);
        if (strcmp(fReturnType, "char") == 0 || strcmp(fReturnType, "char*") == 0)
          return (char *)evaluate(function->treeBody, &scopeChild, pHistory, 0);
      }
      case RET:
      {
        pANTLR3_BASE_TREE expr = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
        gpointer retValue = evaluate(expr, pScope, pHistory, 0);

        printLine(expr->getToken(expr));
        inputString(scope, pHistory);
        return retValue;
      }
      case BLOCK:
      {
        //devo togliere l'ultimo blocco history dato che conterrebbe tutto il blocco e non le singole istruzioni
        /*History *temp = *pHistory;
        *pHistory = temp->prev;
        temp->prev->next = NULL;
        free(temp);
        */
        int k = tree->getChildCount(tree);
        int *r = NULL;
        for (int i = 0; i < k; i++)
        {
          History *history = *pHistory;
          while (history && history->next)
          {
            r = evaluate(history->next->subTree, &history->next->scopeTree, pHistory, 1);
            printf("--------redoing historyy--------\n");
            history = *pHistory;
          }
          r = evaluate(tree->getChild(tree, i), pScope, pHistory, 1);
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

        History *history = *pHistory;
        while (history && history->next)
        {
          r = evaluate(history->next->subTree, &history->next->scopeTree, pHistory, 1);
          printf("--------redoing historyy--------\n");
          history = *pHistory;
        }
        pANTLR3_BASE_TREE child = tree->getChild(tree, i);
        r = evaluate(child, pScope, pHistory, 1);

        if (history && history->read)
        {
          History *tempwhiler = history;
          do
          {
            if (tempwhiler->read)
            {
              printf("variables read %s, ", tempwhiler->read->name);
              var_list *tempvarlist = tempwhiler->read;
              while (tempvarlist->next != tempwhiler->read)
              {
                tempvarlist = tempvarlist->next;
                printf("%s", tempvarlist->name);
              }
              printf("\n");
            }
            tempwhiler = tempwhiler->prev;
          } while (tempwhiler);
          printf("\n");
        }
        if (history && history->written)
        {
          History *tempwhilew = history;
          do
          {
            if (tempwhilew->written)
            {
              printf("variables written %s, ", tempwhilew->written->name);
              var_list *tempvarlistw = tempwhilew->written;
              while (tempvarlistw->next != tempwhilew->written)
              {
                tempvarlistw = tempvarlistw->next;
                printf("%s", tempvarlistw->name);
              }
              printf("\n");
            }
            tempwhilew = tempwhilew->prev;
          } while (tempwhilew);
          printf("\n");
        }
        /*if (child->getToken(child)->type == RET)
        {
          break;
        }*/
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
    evaluate(rootTree, &scopeTree, &isHistory, 1);

    //libero la memoria allocata dinamicamente
    g_hash_table_remove_all(valuesTable);
    g_hash_table_remove_all(funcTable);

    g_hash_table_unref(valuesTable);
    g_hash_table_unref(funcTable);

    freeHistory(&isHistory);

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
