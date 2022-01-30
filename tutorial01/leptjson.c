#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */

/* 如果宏里有多于一条语句，就需要用do{ ... } while(0)，包裹成单个语句。*/
#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

typedef struct {
    const char* json;
}lept_context;

/*
    忽略lept_context开头的空格字符或转义字符
*/
static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

/*
    解析lept_context（即接收到的json），
    1、如果为null则正确返回LEPT_PARSE_OK，并且把lept_context的json指针往后挪三位(因为EXPECT中已经将json++)，
    再置lept_value的类型为LEPT_NULL。
    2、如果解析时不止一个数据则返回LEPT_PARSE_ROOT_NOT_SINGULAR。
*/
static int lept_parse_null(lept_context* c, lept_value* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}


/* 
    仿照lept_parse_null编写lept_parse_true
*/
static int lept_parse_true(lept_context* c, lept_value* v) {
    EXPECT(c,'t');
    if(c->json[0]!='r'||c->json[1]!='u'||c->json[2]!='e') {
        return LEPT_PARSE_INVALID_VALUE;
    }
    c->json+=3;
    v->type=LEPT_TRUE;
    return LEPT_PARSE_OK;
}

/* 
    仿照lept_parse_null编写lept_parse_false
*/
static int lept_parse_false(lept_context* c, lept_value* v) {
    EXPECT(c,'f');
    if(c->json[0]!='a'||c->json[1]!='l'||c->json[2]!='s'||c->json[3]!='e') {
        return LEPT_PARSE_INVALID_VALUE;
    }
    c->json+=4;
    v->type=LEPT_FALSE;
    return LEPT_PARSE_OK;
}

/*
    解析lept_context:
    1、解析null
    2、解析true
    3、解析false
*/
static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*(c->json)) {
        case 'n':  return lept_parse_null(c, v);
        case 't':  return lept_parse_true(c, v);
        case 'f':  return lept_parse_false(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default:   return LEPT_PARSE_INVALID_VALUE;
    }
}

int lept_parse(lept_value* v, const char* json) {
    int ret;
    lept_context c;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL; 
    lept_parse_whitespace(&c);
    if((ret=lept_parse_value(&c,v))==LEPT_PARSE_OK){
        lept_parse_whitespace(&c);
        if(*(c.json)) {
            ret=LEPT_PARSE_ROOT_NOT_SINGULAR; 
        }
    }
    return ret;
}

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}
