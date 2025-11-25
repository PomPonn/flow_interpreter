// #include "compiler.h"

// #include <assert.h>

// #include "shared/str.h"
// #include "compiler/ast.h"

// static inline void _cmp_fl_ast_gen_import_children(fl_ast_node *import_node,
//                                                    fl_token *tokens,
//                                                    fl_size *index)
// {
//     assert(import_node != FL_NULL);
//     assert(tokens != FL_NULL);
//     assert(index != FL_NULL);

//     if (tokens[*index].alias == FL_TOKEN_ALIAS_DOT)
//     {
//         // add global import flag
//         fl_ast_node *global_flag_node;
//         fl_ast_node_init(&global_flag_node, FL_AST_NODE_TYPE_FLAG, fl_str_dup(tokens[*index].value));
//         fl_ast_node_add_child(import_node, global_flag_node);
//         *index++;
//     }

//     do
//     {
//         if (tokens[*index].type == FL_TOKEN_TYPE_IDENTIFIER)
//         {
//             fl_ast_node *identifier_node;
//             fl_ast_node_init(&identifier_node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(tokens[*index].value));
//             fl_ast_node_add_child(import_node, identifier_node);
//             *index++;
//         }
//         else
//         {
//             // syntax error
//             assert(0);
//         }

//     } while (tokens[*index++].type != FL_TOKEN_TYPE_DELIMITER);
// }

// static inline void _cmp_fl_ast_gen_subtree(fl_ast_node *parent, fl_token *tokens,
//                                            fl_size *index,
//                                            const fl_size tokens_num)
// {
//     assert(parent != FL_NULL);
//     assert(tokens != FL_NULL);
//     assert(index != FL_NULL);

//     if (*index >= tokens_num)
//         return;

//     switch (tokens[*index].type)
//     {
//     case FL_TOKEN_TYPE_COMMENT:
//         *index++;
//         break;
//     case FL_TOKEN_TYPE_KEYWORD:
//         switch (tokens[*index].alias)
//         {
//         case FL_TOKEN_ALIAS_USE:
//             // Handle import keyword

//             if (tokens[*index + 1].type == FL_TOKEN_TYPE_IDENTIFIER)
//             {
//                 index++;

//                 // Create an import FL_AST node
//                 fl_ast_node *import_node;
//                 // fl_ast_node_init(parent, FL_NULL, FL_NULL, FL_AST_NODE_TYPE_IMPORT);

//                 _cmp_fl_ast_gen_import_children(import_node, tokens, index);
//                 assert(import_node->children != FL_NULL);

//                 // ...
//             }
//             break;
//         case FL_TOKEN_ALIAS_EXPORT:
//             // Handle export keyword

//             // ...
//             break;
//         }
//         break;
//     case FL_TOKEN_TYPE_IDENTIFIER:
//         // Handle identifier

//         switch (parent->type)
//         {
//         case FL_AST_NODE_TYPE_BLOCK:

//             break;

//         default:
//             break;
//         }

//         break;
//     }

//     // Recursively generate subtrees
//     _cmp_fl_ast_gen_subtree(parent, tokens, index, tokens_num);
// }

// void cmp_compile_to_intermediate(fl_token *tokens, fl_size tokens_num)
// {
// }