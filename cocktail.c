/**
 * Important : ne changez PAS ni les structures, ni les defines, ni les
 * prototypes, ni les variables globales  au risque d'avoir tous les exercices
 * faux.
 *
 * Bien que les `const` aient été mis pour vous aider, s'ils vous gènent vous
 * pouvez les retirer des prototypes.
 *
 * EXPERIMENTAL:
 *  Cet exercice est un test de la nouvelle infrasctucture de test unitaire.
 * Les messages d'erreurs ont étés améliorés même en cas de segfault.
 * Cependant comme pour tout projet exerimental son comportement peut être
 * erratique. Merci de me faire remonter tous les bogues (comportements
 * étranges, message pas téribles).
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Nous souhaitons réaliser une application de gestion de cocktail.
 * Pour cela nous définissons les structures suivantes :
 * - struct product;
 * - struct recipe;
 * - struct ingredient;
 *
 * Remarque :
 *
 * - Si une fonction ne compile pas, commentez là (et revenez-y plus tard, s'il
 *   vous reste du temps) ! Tout fichier qui ne compile pas ne sera pas
 *   regardé.
 *
 * - Si vous ne parvenez pas a faire une fonction, vous pouvez tout de même
 *   l'utiliser (comme si vous l'aviez faîte), nous vous fournissons une
 *   version fonctionelle.
 *
 * Barème indicatif :
 * - 1 pt/fonction :
 *   product_by_name, cost, most_profitable, name_recipe, print_recipe, 
 *   recipe_contains, can_mix_cocktail, recipes_available
 * - 2 pt/fonction :
 *   merge_ingredient, mix_cocktail, cocktails_with, concat, sneaky_bartender,
 *   program
 **/

#define MAX_INGREDIENTS 10

struct product {
    int id;           // `id` correspond à son indice dans le stock.
    const char *name; // Le nom d'un produit
    int quantity;     // La quantité disponible dans le stock
    int unit_price;   // Le prix unitaire d'un produit
};

struct ingredient {
    int product_id; // `product_id` correspond à son indice dans le stock.
    int quantity;   // La quantité nécessaire d'un produit pour réaliser
                    // le cocktail
};

struct recipe {
    const char *name;     // Le nom d'un cocktail
    int price;            // Le prix de vente d'un cocktail
    int ingredient_count; // Le nombre d'ingredients
    struct ingredient ingredients[MAX_INGREDIENTS]; // La liste des ingredients
};

// Le stock de produits et le nombre d'élements du stock
#define STOCK_LENGTH    12
extern struct product stock[];

// L'ensemble des cocktails disponibles et le nombre de cocktails
#define MENU_LENGTH     5
extern struct recipe menu[];

/**
 * Cette fonction retourne un pointeur sur un produit du stock nommé
 * `name`.  S'il n'existe pas de tel produit, elle retourne NULL.
 */
struct product *product_by_name(const char *name);
    /*
    {int n = 0;
    struct product *curr = stock;
    while (n != STOCK_LENGTH){
        if (strcmp(name, curr[n].name)){
            return curr;
        }
    n++;
    }
    return NULL;
}*/

/**
 * Cette fonction retourne le coût d'un `cocktail`.
 * Le coût est la somme des coûts unitaires de chaque produit
 * multiplié par le quantité d'un produit dans la recette
 */
int cost(struct recipe cocktail){
    int n = 0;
    int cost = 0;
    int id = 0;
    while(n != cocktail.ingredient_count){
        id = cocktail.ingredients[n].product_id;
        int count = cocktail.ingredients[n].quantity;
        int y = 0;
        while (y!= STOCK_LENGTH ){
            if(stock[y].id == id){
                cost  += count * stock[y].unit_price;
            }
            y++;
        }
        n++;

    }
    return cost;
}

/**
 * Retourne la recette la plus rentable parmi le tableau `list` de taille
 * `size`, i.e. celle où la différence entre le prix et le coût est la plus
 * élevée.
 * Cette fonction retourne NULL si la liste est vide;
 *
 * Attention: on est autorisé à vendre à perte.
 */
const struct recipe *most_profitable(int size, const struct recipe list[]){
    if (size == 0 || list == NULL){
        return NULL;
    }
    int n = 0;
    int recipe = 0;
    int ccost = 0;
    int cprice =0 ;
    int gain =0;
    int res = 0;
    while(n!=size){
        ccost = cost(list[n]);
        cprice = list[n].price;
        gain = cprice - ccost;
        if (gain>res){ 
            res = gain;
            recipe = n;
        }
        n++;
    }
    return &list[recipe];
}

/**
 * Affiche une recette sous la forme :
 * `<Nom> (<Prix>e): <Ing1> (<Qty1>), <Ing2> (<Qty2>), ...`
 * Remarque: Il n'y a pas de virgule inutile en fin de liste d'ingrédients
 * Exemple:
 * Sex on the beach (3e): Vodka (3), Peach (2), Chambord (2), Pineapple (5), Cramberry (6)
 */

void print_recipe(struct recipe cocktail){
    printf("%s (%de): ",cocktail.name, cocktail.price);
    int n = 0;
    while (n != cocktail.ingredient_count){
        int y=0;
        while(y != STOCK_LENGTH){
            if (stock[y].id == cocktail.ingredients[n].product_id){
                printf("%s (%d)",stock[y].name,cocktail.ingredients[n].quantity);
            }
            y++;
        }
        if (n <cocktail.ingredient_count-1){
            printf(", ");
        }
        
        n++;
    }
}

/**
 * Si la recette `cocktail` contient un ingrédient de même `id` que
 * `product_id`, retourne l'adresse de cet ingrédient, sinon NULL.
 */
struct ingredient *recipe_contains(struct recipe *cocktail, int product_id){
    int n = 0;

    while(n != cocktail->ingredient_count){
        if(product_id ==  cocktail->ingredients[n].product_id){
            return &cocktail->ingredients[n];
        }
        n++;
    }
    return NULL;
}

/**
 * Rajoute l'`ingredient` à la recette `cocktail`. Si la recette contenait
 * déjà un tel produit, la quantité est rajoutée à la précédente.
 *
 * Remarque: Attention à MAX_INGREDIENTS
 */
void merge_ingredient(struct recipe *cocktail, const struct ingredient ingredient){

    int n = 0;
    int test = 0;
    while(n != cocktail->ingredient_count){
        if(ingredient.product_id ==  cocktail->ingredients[n].product_id){
            cocktail->ingredients[n].quantity += ingredient.quantity;
            test = 1;
        }
        n++;
    }
    if (test == 0){
        cocktail->ingredients[n] = ingredient;
        cocktail->ingredient_count +=1;
    }
}
/**
 * Retourne 1 si le `cocktail` est réalisable compte tenu des stocks donnés.
 * i.e. il y a tous les produits nécessaires en quantités suffisantes. Sinon
 * cette fonction retourne 0.
 */
int can_mix_cocktail(struct recipe cocktail){
    int n = 0;
    
    while (n!=cocktail.ingredient_count){
        int y=0;
        while (y !=STOCK_LENGTH){
            if (cocktail.ingredients[n].product_id == stock[y].id){
                if (cocktail.ingredients[n].quantity > stock[y].quantity){
                    return 0;
                }
            }
            y++;

        }
        n++;
    }
    return 1;
}

/**
 * Cette fonction réalise un `cocktail`, i.e. soustrait du stock les
 * quantités nécessaires pour le réaliser.
 *
 * Cette fonction ne prépare le cocktail que s'il est réalisable.
 *
 * Remarque: Cette fonction n'a pas encore de test
 */
void mix_cocktail(struct recipe cocktail){
    if (can_mix_cocktail(cocktail)){
        int n = 0;
        while (n!=cocktail.ingredient_count){
            int y=0;
            while (y !=STOCK_LENGTH){
                if (cocktail.ingredients[n].product_id == stock[y].id){
                     stock[y].quantity -= cocktail.ingredients[n].quantity;
                }
                y++;

            }
            n++;
        }
    }
}

/**
 * Remplit dans `cocktails_left`, la liste des cocktails encore réalisables
 * avec le stock actuel.
 * Cette fonction retourne le nombre d'élements présents dans `cocktail_left`
 */
int recipes_available(int size, const struct recipe cocktails[],
    struct recipe cocktails_left[]){
        int n = 0;
        int y = 0;
        while (n!=MENU_LENGTH){
            if (can_mix_cocktail(cocktails[n])){
                cocktails_left[y] = cocktails[n];
                y++;
            }
            n++;
        }
        return y;
    }

/**
 * Remplit dans `proposals`, la liste des `cocktails` (de taille
 * `cocktails_count`) contenant au moins tous les produits spécifiés par
 * `products_ids` (de taille `products_count`). La fonction retourne le
 * nombre d'éléments présents dans `proposal`.
 */
int cocktails_with(int products_count, const int products_ids[],
    int cocktails_count, struct recipe cocktails[],
    struct recipe proposals[])
        /*
        int n = 0;
        int y = 0;
        int prop_count = 0;
        while (n!=cocktails_count){
            if (cocktails[n].ingredient_count > products_count){
                int test = 1;
                while ((y != products_count) && test !=0){
                    if (recipe_contains(&cocktails[n] ,products_ids[y]) == NULL){
                        test = 0;
                    }
                    y++;
                }
                if (test == 1){
                    proposals[prop_count] = cocktails[n];
                    prop_count ++;
                }
            }
            n++;
        }
        return prop_count;
    }
    */
    ;

/**
 * Retourne le nom recette la plus rentable contenant au moins tous les produits
 * `product_ids` (de taille `products_count`) parmis les recettes proposées
 * `cocktails` de taille cocktails_count, encores réaliasables.
 * S'il n'y a pas de solution cette fonction retoune NULL.
 */
const char *sneaky_bartender(
    int products_count, const int products_ids[],
    int cocktails_count, struct recipe cocktails[]){

        struct recipe cocktails_list[cocktails_count];
        int n=0; int cpt = 0;
        int size = cocktails_with(products_count,products_ids,cocktails_count, cocktails, cocktails_list);
        struct recipe cocktails_available[size];
        while (n!=size){
            if (can_mix_cocktail(cocktails_list[n])){
                cocktails_available[cpt] = cocktails_list[n];
                cpt++;
            }
            n++;
        }
        
        
        return most_profitable(cpt, cocktails_available)->name;

    }

/**
 * Concatène les `n` chaînes de caractères contenues dans
 * `strs` dans `result` et retourne la longueur de la chaîne
 * ainsi créée.
 * Remarque:
 *  - `result` est supposé assez grand pour contenir le résultat.
 *  - L'utilisation de strcat est interdite (car inutile)
 */
int concat(int n, const char *strs[], char *result)
    ;

/**
 * Cette fonction fabrique un nom pour la recette `cocktail`, le nom sera
 * retourné via `name`. Le nom proposé est la concaténation séparé par des
 * `-` de tous les noms de produits contenu dans la recette (et dans
 * l'ordre de la recette).
 * Eg. { { Vodka, 3}, { Orange, 6 } } => Vodka-Orange
 */
void name_recipe(const struct recipe *cocktail, char name[])
    ;

/**
 * On se propose de faire un programme qui affiche la liste des cocktails
 * contenant tous les noms de produits passés en arguments encores
 * réalisables.
 * Le cocktail le plus rentable sera préfixé d'une étoile `*`.
 * Chaque cocktail sera affiché seul sur sa ligne.
 * La valeur retourné sera le nombre de cocktails affichés.
 *
 * Remarque: Dans cette fonction, si vous voulez debugger à l'aide de `printf`
 * utilisez fprintf(stderr, ....) à la place.
 */
int program(int argc, const char *argv[])
    ;
