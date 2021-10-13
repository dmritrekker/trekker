file(REMOVE_RECURSE
  "libTrekker.pdb"
  "libTrekker.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang C CXX)
  include(CMakeFiles/Trekker.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
