//Autor: Denys Malytskyi xmalytd00
/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
 bool search = false;
 bool finish = (tree == NULL);
 while (!finish){
  if (tree->key == key){
    finish = true;
    search = true;
    *value = &(tree->content);
  } else if(key < tree->key){
      tree = tree->left;
    } else {
      tree = tree->right;
    }
  if(tree == NULL){
    finish = true;
  }
  }
  return search;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  bst_node_t *currentNode = *tree;
  bst_node_t *parentNode = NULL;

  while(currentNode != NULL){
    parentNode = currentNode;
    if(key == currentNode->key){
      free(currentNode->content.value);
      currentNode->content = value;
      return;
    }
    if(key < currentNode->key){
      currentNode = currentNode->left;
    } else {
      currentNode = currentNode->right;
    }
  }

  bst_node_t *newPtr = (bst_node_t *) malloc(sizeof(bst_node_t));
  if(newPtr == NULL){
    return;
  }
  newPtr->content = value;
  newPtr->key = key;
  newPtr->left = NULL;
  newPtr->right = NULL;
  
  if(parentNode == NULL){
    *tree = newPtr;
    return;
  }
  if(key < parentNode->key){
    parentNode->left = newPtr;
  } else {
    parentNode->right = newPtr;
  }

}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  bst_node_t *currentNode = *tree;
  bst_node_t *parentNode = NULL;

  while (currentNode->right != NULL) {
    parentNode = currentNode;
    currentNode = currentNode->right;
  }
  free(target->content.value);
  target->key = currentNode->key;
  target->content = currentNode->content;

  if (parentNode != NULL) {
    if (currentNode->left != NULL) {
      parentNode->right = currentNode->left;
    } else {
      parentNode->right = NULL;
    }
    } else {
      *tree = currentNode->left;
    }

  free(currentNode);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  bst_node_t *currentNode = *tree;
  bst_node_t *parentNode = NULL;

  while (currentNode != NULL && currentNode->key != key) {
    parentNode = currentNode;
    if (key < currentNode->key) {
      currentNode = currentNode->left;
    } else {
      currentNode = currentNode->right;
    }
  }

  if (currentNode == NULL) {
    return;
  }

  if (currentNode->left != NULL && currentNode->right != NULL) {
    bst_replace_by_rightmost(currentNode, &currentNode->left);
  }
  else if (currentNode->left != NULL || currentNode->right != NULL) {
    bst_node_t *child = (currentNode->left != NULL) ? currentNode->left : currentNode->right;
    if (parentNode == NULL) {
      *tree = child;  
    } else if (parentNode->left == currentNode) {
        parentNode->left = child;
      } else {
        parentNode->right = child;
      }
        free(currentNode->content.value);
        free(currentNode);
  }
  else {
    if (parentNode == NULL) {
      *tree = NULL;  
    } else if (parentNode->left == currentNode) {
      parentNode->left = NULL;
    } else {
      parentNode->right = NULL;
    }
    free(currentNode->content.value);
    free(currentNode);
  }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  if(*tree == NULL){
    return;
  }

  stack_bst_t stack;
  stack_bst_init(&stack);
  do{
    if(*tree == NULL){
      if(!stack_bst_empty(&stack)){
        *tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
      }
    } else {
      if((*tree)->right != NULL){
        stack_bst_push(&stack, (*tree)->right);
      }
      bst_node_t *aux = *tree;
      *tree = (*tree)->left;
      free(aux->content.value);
      free(aux);
    }
  } while((*tree != NULL) || (!stack_bst_empty(&stack)));
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{
  while(tree != NULL){
    stack_bst_push(to_visit, tree);
    bst_add_node_to_items(tree, items);
    tree = tree->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_preorder(tree, &stack, items);
  while(!stack_bst_empty(&stack)){
    tree = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_leftmost_preorder(tree->right, &stack, items);
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while(tree != NULL){
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_inorder(tree, &stack);
  while(!stack_bst_empty(&stack)){
    tree = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_add_node_to_items(tree, items);
    bst_leftmost_inorder(tree->right, &stack);
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  while(tree != NULL){
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  bool fromLeft;
  stack_bst_t stackBst;
  stack_bool_t stackBool;
  stack_bst_init(&stackBst);
  stack_bool_init(&stackBool);
  bst_leftmost_postorder(tree, &stackBst, &stackBool);
  while(!stack_bst_empty(&stackBst)){
    tree = stack_bst_top(&stackBst);
    fromLeft = stack_bool_top(&stackBool);
    stack_bool_pop(&stackBool);
    if(fromLeft){
      stack_bool_push(&stackBool, false);
      bst_leftmost_postorder(tree->right, &stackBst, &stackBool);
    } else {
      stack_bst_pop(&stackBst);
      bst_add_node_to_items(tree, items);
    }
  }
}
