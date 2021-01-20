#ifndef INSTRUCTION_EVALUATOR
#define INSTRUCTION_EVALUATOR

#include <gmodule.h>
#include "simplecTreeParser.h"
#include <search.h>




 int true;
 int false;
 int error;
 void *pNull;

 int *pTrue;
 int *pFalse;
 int *pError;
 void *returnVal;




void free_data(gpointer data);

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

  void freeVarList(gpointer listHead);

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
  
  void freeHistory(History * *pHistory);

  void freeListPar(par_list * head);

  void free_function(gpointer function);


  void insertWrittenHistory(History * *pHistory, struct Variable * varHistory);

  void hashInsert(char *keyChar, gpointer value, GHashTable *hashTable);

  void hashInsertVar(char *keyChar, char *type, gpointer value, scope_tree *scopeTree, History **pHistory, int save);

  gpointer hashGetVaue(char *key, GHashTable *hashTable, GHashTable *globalHashTable);

  gpointer hashGetVaueVar(char *key, scope_tree *scopeTree, int position, History **pHistory, int save);

  void printLine(pANTLR3_COMMON_TOKEN token);

  void inputString(scope_tree * scope, History * *pHistory);

  char *charEvaluator(pANTLR3_BASE_TREE tree, scope_tree * scope, History * *pHistory);

  int numberEvaluator(pANTLR3_BASE_TREE tree, scope_tree * scope, History * *pHistory);

  void addEnd(par_list * *head, char *type, char *name);

  void *evaluate(pANTLR3_BASE_TREE tree, scope_tree * *pScope, History * *pHistory, int saveHistory);

#endif