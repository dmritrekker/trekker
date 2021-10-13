file(REMOVE_RECURSE
  "libtmp.a"
  "libtmp.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C CXX)
  include(CMakeFiles/tmp.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
