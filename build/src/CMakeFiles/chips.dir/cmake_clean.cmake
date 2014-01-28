FILE(REMOVE_RECURSE
  "chips.pdb"
  "chips"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/chips.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
