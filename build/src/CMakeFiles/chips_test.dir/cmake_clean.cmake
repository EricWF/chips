FILE(REMOVE_RECURSE
  "chips_test.pdb"
  "chips_test"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/chips_test.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
