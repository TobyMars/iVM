//
//  main.c
//  iVM
//
//  Created by TobyMars on 2021/2/13.
//  Copyright © 2021 TobyMars. All rights reserved.
//

#include <stdio.h>

#include "monalisp.h"

int
main(int argc, const char * argv[])
{
    int ret = monalisp(argc, argv);
    
    return ret;
}
