//
//  parser.hpp
//  test_polygon
//
//  Created by asd on 09/09/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef parser_hpp
#define parser_hpp

#include "common.hpp"
#include "poly_operations_mt.hpp"
#include "polyhedron.hpp"
#include "seeds.hpp"
#include <ctype.h>

class Parser {
public:
  Parser() {}

  static void test_tuple_performance() {

    int n = 2e6;
    vector<Int4> _v4;
    vector<Int4> v4;
    Int4 i0, i1(1), i2(1, 2), i3(1, 2, 3), i4(1, 2, 3, 4);

    Timer t;

    _v4.resize(n);
    t.timer("_Int4 init");
    v4.resize(n);
    t.timer("Int4 init");

    for (int i = 0; i < n; i++)
      _v4[i] = Int4(rand(), rand(), rand(), rand());
    t.timer("_Int4 fill");
    for (int i = 0; i < n; i++)
      v4[i] = Int4(rand(), rand(), rand(), rand());
    t.timer("Int4 fill");

    sort(_v4.begin(), _v4.end());
    t.timer("sort _Int4");

    sort(v4.begin(), v4.end());
    t.timer("sort Int4");

    printf("is sorted _v4:%d, v4:%d\n", std::is_sorted(_v4.begin(), _v4.end()),
           std::is_sorted(v4.begin(), v4.end()));
  }

  static Polyhedron parse(string s) { // ttttBN
    Polyhedron p;
    int n = 0;
    size_t slen = s.length(), i = 0;
    string sd;

//    test_tuple_performance();

    reverse(s.begin(), s.end()); // NBtttt

    for (i = 0; isdigit(s[i]); i++)
      sd += s[i]; // N

    if (!sd.empty()) {
      reverse(sd.begin(), sd.end());
      try {
        n = std::stoi(sd);
      } catch (std::invalid_argument) {
        n = -1;
      }
    }

    switch (s[i]) { //  base poly
    case 'T':
      p = Seeds::tetrahedron();
      break;
    case 'C':
      p = Seeds::cube();
      break;
    case 'O':
      p = Seeds::octahedron();
      break;
    case 'I':
      p = Seeds::icosahedron();
      break;
    case 'D':
      p = Seeds::dodecahedron();
      break;
    case 'P':
      p = Seeds::prism(n);
      break;
    case 'A':
      p = Seeds::antiprism(n);
      break;
    case 'Y':
      p = Seeds::pyramid(n);
      break;
    case 'U':
      p = Seeds::cupola(n);
      break;
    case 'V':
      p = Seeds::anticupola(n);
      break;
    case 'J':
      p = Seeds::johnson(n);
      break;
    default:
      return p; // wrong base
    }

    for (i++; i < slen; i++) { // transformations: dagprPqkcwnxlH
      switch (s[i]) {
      case 'd':
        p = PolyOperations::dual(p);
        break;
      case 'a':
        p = PolyOperations::ambo(p);
        break;
      case 'g':
        p = PolyOperations::gyro(p);
        break;
      case 'p':
        p = PolyOperations::propellor(p);
        break;
      case 'r':
        p = PolyOperations::reflect(p);
        break;
      case 'P':
        p = PolyOperations::perspectiva1(p);
        break;
      case 'q':
        p = PolyOperations::quinto(p);
        break;

      case 'k':
        p = PolyOperations::kisN(p);
        break; // parameters
      case 'c':
        p = PolyOperations::chamfer(p);
        break;
      case 'w':
        p = PolyOperations::whirl(p);
        break;
      case 'n':
        p = PolyOperations::insetN(p);
        break;
      case 'x':
        p = PolyOperations::extrudeN(p);
        break;
      case 'l':
        p = PolyOperations::loft(p);
        break;
      case 'H':
        p = PolyOperations::hollow(p);
        break;

      default:
        break;
      }
    }

    return p.recalc();
  }
};

#endif /* parser_hpp */
