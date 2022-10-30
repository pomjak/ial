/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) 
{
  bst_node_t *temp = tree;

  while(temp)
  {
    if(key == temp->key)
    {
      *value = temp->value; 
      return true;
    }
    else if(key > temp->key)
      temp = temp->right;
    else if(key < temp->key)
      temp = temp->left;
    else
      return false;
  }

  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) 
{
  bst_node_t *temp = (*tree), *parent = NULL;

  while(temp != NULL)
  {

    if(key > temp->key)
    {
      parent = temp;
      temp = temp->right;
      continue;
    }

    if(key < temp->key)
    {
      parent = temp;
      temp = temp->left;
      continue;
    }

    if(key == temp->key)
    {
      temp->value = value;
      return;
    }
  }

  bst_node_t *new = malloc(sizeof(bst_node_t)); 
  if(new != NULL)
  {
    new->key = key;
    new->value = value;
    new->right = NULL;
    new->left = NULL;

    if(!parent)
      (*tree) = new;
    else
      temp  = new;

    if(parent && key > parent->key)
      parent->right = new;

    else if(parent && key < parent->key)
      parent->left = new;

  }
  else return;//malloc failed
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) 
{
  if(target == NULL || (*tree) == NULL)
    return;

  for(bst_node_t *temp = (*tree),*pre = NULL; temp; temp = temp->right)
  {
    if(temp->right == NULL)
    {
      target->value = temp->value;
      target->key = temp->key;

      target->left = NULL;

      if(temp->left != NULL)
        target->left = temp->left;

      free(temp);
      break;
    }

    else if(temp->right->right == NULL)
    {
      pre = temp;
      temp = temp->right;

      target->value = temp->value;
      target->key = temp->key;

      pre->right = NULL;

      if(temp->left != NULL)
        pre->right = temp->left;

      free(temp);
      break;
    }
  }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) 
{
  if((*tree) == NULL)
    return;

  bst_node_t *temp = (*tree), *pre = NULL;

  while (temp != NULL && temp->key != key) 
  {
    pre = temp;

    if(key < temp->key)
      temp = temp->left;

    if(key > temp->key)
      temp = temp->right;
  }

  if(!temp)
    return;

  if(temp->left == NULL || temp->right == NULL) 
  {
    bst_node_t *sub = NULL;

    if(temp->left == NULL)
      sub = temp->right;
    else
      sub = temp->left;

    if(temp == pre->left)
      pre->left = sub;

    else
      pre->right = sub;

    free(temp);
  }
  else bst_replace_by_rightmost(temp,&temp->left);
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) 
{
  stack_bst_t stack;
  stack_bst_init(&stack);

  do
  {
    if((*tree) == NULL && !stack_bst_empty(&stack))
    {
      (*tree) = stack_bst_top(&stack);
      stack_bst_pop(&stack);
    }
    else if ((*tree)->right != NULL)
    {
      stack_bst_push(&stack,(*tree)->right);
    }

    bst_node_t *temp = (*tree);
    (*tree) = (*tree)->right;
    free(temp);

  }while ((*tree)!= NULL || !stack_bst_empty(&stack));
  
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
}
