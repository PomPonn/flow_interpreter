#include <stdio.h>

#include "file.h"
#include "tokenizer.h"

const char* TOKEN_ESCAPES[] = {
    "==",
    "!=",
    "=>",
    "<=",
    ">=",
    "<<",
    ">>",
    "=",
    "<",
    ">",
    "+",
    "-",
    "*",
    "/",
    "%",
    "&&",
    "||",
    "!",
    "&",
    "|"
    "~",
    "?",
    ":",
    "\n",
    "\"",
    "'",
    ".",
    "(",
    ")",
    "{",
    "}",
    ";",
    "?",
    "#",
};

int main() {
  const char* filename = "../../../../test.fl";
  char* file_content = fl_read_file_as_str(filename);

  if (file_content == NULL) {
    fprintf(stderr, "Failed to read file: %s\n", filename);
    return 1;
  }

  // Tokenize the content
  size_t tokens_num;
  char** tokens =
      tok_tokenize(file_content, TOKEN_ESCAPES,
                   sizeof(TOKEN_ESCAPES) / sizeof(char*), " \r\t", &tokens_num);

  fl_free_file(file_content);

  if (tokens == NULL) {
    fprintf(stderr, "Failed to tokenize content.\n");
    fl_free_file(file_content);
    return 1;
  }

  // Print tokens
  for (size_t i = 0; i < tokens_num; i++) {
    printf("Token %zu: %s\n", i + 1, tokens[i]);
  }
  // for (size_t i = 0; i < tokens_num; i++) {
  //   printf("%s", tokens[i]);
  // }

  tok_free_tokens(tokens, tokens_num);

  return 0;
}
