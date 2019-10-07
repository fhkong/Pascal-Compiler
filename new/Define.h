#ifndef _DEFINE_H
#define _DEFINE_H


#define NoUse -1

/**************KeyWord********************/
#define    KeyWord      0   //type

#define    PROGRAM      1
#define    CONST        2
#define    VAR          3
#define    PROCEDURE    4
#define    BEGIN        5
#define    END          6
#define    IF           7
#define    ELSE         8    
#define    WHILE        9
#define    DO           10
#define    CALL         11
#define    READ         12
#define    WRITE        13
#define    ODD          14
#define    THEN         15


/**************Operator*******************/
#define     Operator    99      //type

#define     ASSIGN      100     // :=
#define     LESS_THAN   101     // <
#define     LESS_EQUA   102     // <=
#define     GRT_THAN    103     // >
#define     GRT_EQUA    104     // >=
#define     ADD         105     // +
#define     SUB         106     // -
#define     MUL         107     // *
#define     DIV         108     // /
#define     POWER       109     // ^
#define     NOREQUA     110     // <>
#define     EQUAL       111     // =

/**************Boundary*******************/
#define     Boundary     200     //type

#define     LEFT_SBRA    201     // (
#define     RIGHT_SBRA   202     // )
#define     LEFT_MBRA    203     // [
#define     RIGHT_MBRA   204     // ]
#define     POINTER      205     // .
#define     COMMA        206     // ,
#define     SEMI         207     // ;


/*************Identifier*******************/
#define     IDENTIFIER          500     //type,include id and const number
   
/************NorTerMap start num**********/
#define     NorTerBase      1000  //非终结符的起始数

/************Grammer Special char************/
#define     GRAMMAR_ARROW   2000  // ->
#define     GRAMMAR_OR      2001  // |
#define     GRAMMAR_NULL    2002  // $
#define     GRAMMAR_END     2003  // #




#endif
