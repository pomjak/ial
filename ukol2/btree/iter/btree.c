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

  while(temp)//iterace dokud neni NULL
  {
    if(key == temp->key)
    {
      *value = temp->value; //predani dat
      return true;
    }
    else if(key > temp->key)//posunuti podle hodnoty klice
      temp = temp->right;
    else if(key < temp->key)
      temp = temp->left;
    else
      return false;//nenalezeno
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

  while(temp != NULL)//vyhledavani iteraci a ukladani otce a daneho uzlu
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

  bst_node_t *new = malloc(sizeof(bst_node_t)); //vytvoreni noveho uzlu
  if(new != NULL)
  {
    new->key = key;
    new->value = value;
    new->right = NULL;
    new->left = NULL;

    if(!parent)//vlozeni korenoveho uzlu
      (*tree) = new;
    else
      temp = new;

    if(parent && key > parent->key)//vytoreni navaznosti
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
    if(temp->right == NULL)//pripad kdy uzel je hned jako root podstromu a ma syny
    {
      target->value = temp->value;
      target->key = temp->key;

      target->left = NULL;//zruseni navaznosti

      if(temp->left != NULL)
        target->left = temp->left;//preklenuti navaznosti na levy podstrom presunuteho uzlu

      free(temp);
      break;
    }

    else if(temp->right->right == NULL)//nalezeni uzlu
    {
      pre = temp;//ulozeni otce
      temp = temp->right;

      target->value = temp->value;//predani dat
      target->key = temp->key;

      pre->right = NULL;//zruseni navaznosti otce

      if(temp->left != NULL)
        pre->right = temp->left;//navaznost otce na levy podstrom syna

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

  while (temp != NULL && temp->key != key) // iterovat do doby nez se nalezne hledany prvek
  {
    pre = temp;//ulozeni otce

    if(key < temp->key)
      temp = temp->left;

    if(key > temp->key)
      temp = temp->right;
  }

  if(!temp)
    return;

  if(temp->left == NULL || temp->right == NULL) //uzel ma nanejvys 1 potomka
  {
    bst_node_t *sub = NULL;

    if(temp->left == NULL)//nalezeni syna a ulozeni
      sub = temp->right;
    else
      sub = temp->left;

    if(temp == pre->left)//uprava navaznosti z rodice na dalsiho potomka
      pre->left = sub;

    else
      pre->right = sub;

    free(temp);
  }
  else bst_replace_by_rightmost(temp,&temp->left);//v pripade ze uzel ma 2 potomky
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
  bst_node_t *temp = NULL;

  if(!(*tree))
    return;

  stack_bst_push(&stack,(*tree));//ulozeni koren. uzlu na stack

  while (!stack_bst_empty(&stack))//iterovat dokud jsou prvky k smazani
  {
    temp = stack_bst_pop(&stack);//pop ze stacku k uvolneni korenoveho uzlu

    if(temp->right)//pri existenci synu ulozit syny na stack a pri dalsi iteraci popnout a smazat
      stack_bst_push(&stack,temp->right);

    if(temp->left)
      stack_bst_push(&stack,temp->left);
    free(temp);//smazani ponuteho uzlu
  }

  (*tree) = NULL;
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
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) 
{
  while(tree)
  {
    bst_print_node(tree);
    stack_bst_push(to_visit,tree);
    tree = tree->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) 
{
  stack_bst_t stack;
  stack_bst_init(&stack);

  bst_leftmost_preorder(tree,&stack);//ulozeni nejlevejsiho uzlu na stack
  
  while(!stack_bst_empty(&stack))//dokud neni stack prazdny
  {
    tree = stack_bst_pop(&stack);
    bst_leftmost_preorder(tree->right, &stack);//hledani nejlevejsiho uzlu u praveho syna
  }

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
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) 
{
  while(tree)
  {
    stack_bst_push(to_visit,tree);
    tree = tree->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) 
{ 
  stack_bst_t stack;
  stack_bst_init(&stack);

  bst_leftmost_inorder(tree,&stack);//nalezeni leftmost v celem stromu

  while(!stack_bst_empty(&stack))
  {
    tree = stack_bst_pop(&stack);//vraceni uzlu 
    bst_print_node(tree);//vypsani se zacatkem vlevo
    bst_leftmost_inorder(tree->right,&stack);//po leve vetvi v pravem podstromu
  }
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
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) 
{
  while(tree)
  {
    stack_bst_push(to_visit,tree);
    stack_bool_push(first_visit,true);
    tree = tree->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) 
{
  stack_bool_t stack_bool;
  stack_bst_t stack_bst;

  stack_bool_init(&stack_bool);
  stack_bst_init(&stack_bst);

  bst_leftmost_postorder(tree, &stack_bst, &stack_bool);

  while(!stack_bst_empty(&stack_bst))
  {
    tree = stack_bst_pop(&stack_bst);//vraceni uzlu pro praci s nim

    if(stack_bool_pop(&stack_bool))//pokud byl jednou zpracovan
    {
      stack_bst_push(&stack_bst,tree);
      stack_bool_push(&stack_bool,false);//zpracovani podruhe
      bst_leftmost_postorder(tree->right, &stack_bst, &stack_bool);

      continue;
    }

    bst_print_node(tree);
  }

}
