#pragma once


#include "types/AST.h"
#include <stdio.h>

void llvm_genIR(AST* root, FILE* fdout);
