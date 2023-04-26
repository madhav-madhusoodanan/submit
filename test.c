#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "apple,banana,orange";
    char str2[] = "red,green,blue";
    char result[1000] = ""; // initialize the result string

    // split the first string into individual words
    char *token1 = strtok(str1, ",");
    int i = 1;

    // loop through the words in the first string
    while (token1 != NULL) {
        strcat(result, token1); // add the word to the result string

        // if there are more words in the second string, add a comma
        if (i % 2 == 0) {
            strcat(result, ",");
        }

        // get the next word from the second string
        char *token2 = strtok(str2, ",");

        // if there are no more words in the second string, break the loop
        if (token2 == NULL) {
            break;
        }

        strcat(result, token2); // add the word to the result string

        // if there are more words in the first string, add a comma
        if (token1 = strtok(NULL, ",") != NULL) {
            strcat(result, ",");
        }

        // get the next word from the first string
        token1 = strtok(NULL, ",");
        i += 2;
    }

    printf("Result: %s\n", result); // print the result string

    return 0;
}
