#include <stdio.h>

#include "file.h"
#include "preprocessor.h"
#include "tokenizer.h"

int main()
{
  const char *filename = "../../../../test.fl";
  char *file_content = fl_read_file_as_str(filename);

  if (file_content == NULL)
  {
    fprintf(stderr, "Failed to read file: %s\n", filename);
    return 1;
  }

  // Tokenize the content
  tok_context context = tok_make_context();

  size_t tokens_num;
  inp_token *tokens = tok_tokenize(file_content, context, &tokens_num);

  tok_free_context(&context);

  fl_free_file(file_content);

  if (tokens == NULL)
  {
    fprintf(stderr, "Failed to tokenize content.\n");
    fl_free_file(file_content);
    return 1;
  }

  ppc_preprocess(tokens, tokens_num);

  // Print tokens
  // for (size_t i = 0; i < tokens_num; i++)
  // {
  //   printf("[%zu, %zu] `%s` %i %i\n", tokens[i].line, tokens[i].column,
  //          tokens[i].value, tokens[i].type, tokens[i].alias);
  //   // for (size_t i = 0; i < tokens_num; i++) {
  //   //   printf("%s", tokens[i]);
  //   // }
  // }

  tok_free_tokens(tokens, tokens_num);

  return 0;
}