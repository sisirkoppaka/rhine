%{ open Ast %}

%token LPAREN RPAREN LSQBR RSQBR NIL TRUE FALSE EOF
%token <int> INTEGER
%token <float> DOUBLE
%token <string> SYMBOL
%token <string> STRING

%start prog
%type <Ast.prog> prog
%type <Ast.atom> atom
%type <Ast.sexpr> sexpr

%%
prog:
    sexprs EOF { Prog($1) }

atom:
   NIL { Nil }
 | TRUE { Bool(true) }
 | FALSE { Bool(false) }
 | INTEGER { Int($1) }
 | DOUBLE { Double($1) }
 | STRING { String($1) }
 | SYMBOL { Symbol($1) }

sexpr:
   atom { Atom($1) }
 | LPAREN sexprs RPAREN {
       let rec buildDP = function
            [] -> Atom(Nil)
          | h::t -> DottedPair(h, buildDP t)
       in buildDP($2)
   }

qsexpr:
   LSQBR sexprs RSQBR { Vector(Array.of_list $2) }

sexprs:
   sexpr sexprs { $1::$2 }
 | qsexpr sexprs { $1::$2 }
 | sexpr { [$1] }
 | qsexpr { [$1] }
