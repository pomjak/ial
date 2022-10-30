/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
void bst_init(bst_node_t **tree) 
{
  (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) 
{
  
  if(!tree)
    return false;

  if(tree->key == key)
  {
    *value = tree->value;//predani hodnoty
    return true;
  }

  else if(key > tree->key)//rekurze podle hodnoty klice
    return bst_search(tree->right, key, value);

  else
    return bst_search(tree->left, key, value);

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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) 
{
  if((*tree) == NULL)
  {
    bst_node_t *new = malloc(sizeof(bst_node_t)); //vytvoreni noveho uzlu
    if(new != NULL)
    {
      new->key = key;
      new->value = value;
      new->right = NULL;
      new->left = NULL;
      (*tree) = new;
    }
    else return;//malloc failed
  }

  else if (key > (*tree)->key)//rekurze podle hodnoty klice
    bst_insert(&(*tree)->right,key,value);

  else if (key < (*tree)->key)
    bst_insert(&(*tree)->left,key,value);
    
  else
    (*tree)->value = value;//aktualizace dat
  
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) 
{
  if(target == NULL || (*tree) == NULL)
    return;
  
  bst_node_t *temp;
  
  if((*tree)->right == NULL)//uzel nalezen jako list
  {
    temp = (*tree);

    target->value = temp->value;//predani dat
    target->key = temp->key;

    target->left = NULL;//zruseni navaznosti

    if(temp->left != NULL)//preklenuti navaznosti
      target->left = temp->left;

    free(temp);
  }

  else if((*tree)->right->right == NULL)//nalezeni nejpravejsiho uzlu v roli otce leveho podstromu
  {
    temp = (*tree)->right;

    target->value = temp->value;//predani dat
    target->key = temp->key;

    (*tree)->right = NULL;//zruseni navaznosti

    if(temp->left != NULL)//preklenuti navaznosti na levy podstrom
      (*tree)->right = temp->left;

    free(temp);
  }

  else bst_replace_by_rightmost(target, &(*tree)->right);//jinak rekurze pro vzhledani 

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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) 
{
  if((*tree) == NULL)
    return;//chyba

  if((*tree)->key == key)//pri nalezeni spravneho uzlu
  {
    if((*tree)->right == NULL)//uzel ma maximalne jeden podstrom a to levy
    {
      bst_node_t *temp = (*tree);
      (*tree) = (*tree)->left;//posunti na levy podstrom
      free(temp);
    }
    
    else if((*tree)->left == NULL)//uzel ma maximalne 1 podstrom a to pravy
    {
      bst_node_t *temp = (*tree);
      (*tree) = (*tree)->right;//posunuti na pravy podstrom
      free(temp);
    }
    else bst_replace_by_rightmost((*tree),&(*tree)->left);//uzel ma 2 podstromy
  }

  else if(key > (*tree)->key)//vzhledavani uzlu rekurzi
    bst_delete(&(*tree)->right, key);

  else if(key < (*tree)->key)
    bst_delete(&(*tree)->left, key);
  
  else return;
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) 
{
  if((*tree)!=NULL)
  {
    if((*tree)->right != NULL)bst_dispose(&(*tree)->right);//vnoreni pro eliminaci praveho podstromu
    if((*tree)->left != NULL)bst_dispose(&(*tree)->left);//to same plati pro levy podstrom
    free((*tree));
    (*tree) = NULL;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) 
{
  if(tree != NULL)
  {
    bst_print_node(tree);//1.vypsat koren
    bst_preorder(tree->left);//pote levou stranu
    bst_preorder(tree->right);//a pak pravou
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) 
{
  if(tree != NULL)
  {
    bst_inorder(tree->left);//vypsani leveho synu pomoci rekurze
    bst_print_node(tree);//pote koren
    bst_inorder(tree->right);//a pak prave syny
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) 
{
  if(tree != NULL)
  {
    bst_postorder(tree->left);//levy syn
    bst_postorder(tree->right);//pravy syn
    bst_print_node(tree);//koren
  }
}
