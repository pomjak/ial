/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) 
{
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) 
{

  for(int i = 0; i < HT_SIZE; i++)
    (*table)[i] = NULL;

}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) 
{
  ht_item_t *item; //pomocny prvek pro prochazeni a porvnavni

  for(item = (*table)[get_hash(key)]; item ; item = item->next )//iterace prvku z hash-tabulky[klic] 
  {                                                             //a nasledna inkremntace na dalsi prvek v tabulce
    if(!strcmp(key,item->key))
      return item;//v pripade uspechu vraci ukazatel na spravny prvke jinak NULL
  }

  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) 
{
  ht_item_t *item = ht_search(table,key);//hledani zda nov

  if(item)
    item->value = value;//aktualizace dat
  else{
    ht_item_t *new = malloc(sizeof(ht_item_t));
    if(new != NULL)
    {
      new->key = key;
      new->value = value;

      new->next = (*table)[get_hash(key)];//uprava navaznosti
      (*table)[get_hash(key)] = new;//ulozeni na zacatek
    }
  }
  
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) 
{
  ht_item_t *item = ht_search(table,key);

  if(item)
    return &item->value;

  return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) 
{
  ht_item_t *wanted = NULL;
  ht_item_t *pre_wanted = NULL;

  for(wanted = (*table)[get_hash(key)]; wanted; pre_wanted = wanted, wanted = wanted->next)
  {
    if(!strcmp(wanted->key,key))
    {
      if(pre_wanted == NULL)//prvek se nachazi na zacatku hash tabulky
        (*table)[get_hash(key)] = wanted->next;//uprava zacatku tabulky

      else
        pre_wanted->next = wanted->next;//jinak uprava navaznosti 

      free(wanted);
    }
  }

}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) 
{
  ht_item_t *item = NULL; 
  ht_item_t *tmp = NULL;

  for(int i = 0; i < HT_SIZE; ++i)
  {
    item = (*table)[i];

    while(item != NULL)//dokud nedojde na konec seznamu
    {
      tmp = item;
      item = item->next;
      free(tmp);//smazani predchoziho prvku
      tmp = NULL;
    }
    (*table)[i] = NULL;//zahozeni ukazatele z hash table
  }
}
