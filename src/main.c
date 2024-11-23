#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/ast.h"
#include "../include/parser.h"
#include "../include/stack.h"

int main() {
    FileData data = getData("../data.txt");

    Node tree = createTree(data);
    printTree(&tree);
    bool treeCheck = validateTree(&tree);
    printf("Is tree valid: %i\n", treeCheck);
    if (treeCheck) {
        ExpressionValue val = executeNode(&tree);
        if (val.type == ScalarType) {
            printf("final value: %f\n", val.data.scalar);
        } else {
            printf("final value: [");
            for (int i = 0; i < val.data.vector.dimension; i++) {
                printf("%f", val.data.vector.components[i]);
                if (i == val.data.vector.dimension - 1) {
                    printf("]");
                } else {
                    printf(", ");
                }
            }
        }
    }
    destroyTree(&tree);
    free(data.string);
    return 0;
}
