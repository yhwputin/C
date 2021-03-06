//
//  main.c
//  L1S1.4E2
//
//  Created by Christine Chan on 9/23/19.
//  Copyright © 2019 Christine Chan. All rights reserved.
//

#include <stdio.h>

int main() {
    int a = 0;
    int characters = 0;
    int words = 1 ;
    int newlines = 0;
    int space = 0;
    
    do{
        if ((a >= 'a'&&a <= 'z') || (a >= 'A'&&a <= 'Z')){
            space = 0;
            characters++;
        }else if ((a == ' ') && space!=1){
            space = 1;
            words++;
        }else if (a == '\n'){
            space = 0;
            newlines++;
        }
    }while ((a = getchar())!=EOF);
    printf("The number of characters is %d\n", characters);
    printf("The number of words is %d\n", words);
    printf("The number of newlines is %d\n", newlines);
    
    return 0;
}
