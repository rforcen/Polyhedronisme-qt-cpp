//
//  main.cpp
//  profile_poly
//
//  Created by asd on 06/11/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#include "parser.hpp"
#include "../../../Timer.h"


int main(int argc, const char * argv[]) {
    
//    Parser::test_maps();
    
    Polyhedron p=Parser::parse("qqqqqD"), pp;
    
    puts("starting last q ------------------------\n\n");
    Timer t;
    
    pp= PolyOperations::quinto(p);
    printf("mt=%ld\n",t.lap());
    p.print_stat();
    
    return 0;
}
