/* Include Guard */
#ifndef LEPTJSON_H__
#define LEPTJSON_H__

typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;

typedef struct {
    lept_type type;
}lept_value;

/*
    lept_parse 返回值：
    LEPT_PARSE_OK表示解析成功
    LEPT_PARSE_EXPECT_VALUE表示当前json输入为空，期望一个输入值
    LEPT_PARSE_INVALID_VALUE表示无效值
    LEPT_PARSE_ROOT_NOT_SINGULAR表示输入的json有多个数据类型
*/
enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR
};

int lept_parse(lept_value* v, const char* json);

lept_type lept_get_type(const lept_value* v);

#endif /* LEPTJSON_H__ */
