indent example_input.c -o example_std.c
indent -kr example_input.c -o example_kr.c
indent -orig example_input.c -o example_orig.c
indent --line-length80 --no-tabs --indent-level4 \
--blank-lines-after-declarations --blank-lines-after-procedures \
--blank-lines-after-procedure-declarations --dont-format-comments \
--dont-format-first-column-comments --comment-indentation33 \
--declaration-comment-column33 --else-endif-column33 --comment-line-length78 \
--blank-lines-before-block-comments --leave-optional-blank-lines \
--line-comments-indentation0 --comment-delimiters-on-blank-lines \
--start-left-side-of-comments --braces-on-if-line --cuddle-else \
--cuddle-do-while --case-indentation4 --case-brace-indentation0 \
--dont-space-special-semicolon --no-space-after-function-call-names \
--space-after-cast --no-blank-before-sizeof --space-after-for \
--space-after-if --space-after-while --no-space-after-parentheses \
--blank-lines-after-commas --dont-break-function-decl-args \
--dont-break-function-decl-args-end --procnames-start-lines \
--braces-on-func-def-line --continue-at-parentheses --parameter-indentation4 \
--preprocessor-indentation4 --remove-preprocessor-space \
--break-before-boolean-operator --honour-newlines \
example_input.c -o example_sipoks.c
