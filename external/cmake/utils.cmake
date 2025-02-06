# make folder only if it does not exist
function(conditional_make_directory folder)
    if(NOT EXISTS "${folder}")
        message("Making directory: ${folder}")
        file(MAKE_DIRECTORY "${folder}")
    endif()
endfunction()

# rename file only if the source exists
function(conditional_rename src dst)
    if(EXISTS "${src}")
        message("Renaming: ${src} -> ${dst}")
        file(RENAME "${src}" "${dst}")
    endif()
endfunction()

# remove file only if it exists
function(conditional_remove_file file)
    if(EXISTS "${file}")
        message("Removing file: ${file}")
        file(REMOVE "${file}")
    endif()
endfunction()

# copy file only if it does not exist
function(conditional_copy_file src_file dst_file)
    if (EXISTS "${dst_file}")
        file(SHA256 "${src_file}" source_hash)
        file(SHA256 "${dst_file}" destination_hash)
        if (NOT source_hash STREQUAL destination_hash)
            message("Copying: ${src_file} -> ${dst_file}")
            configure_file("${src_file}" "${dst_file}" COPYONLY)
        endif()
    else()
        message("Copying: ${src_file} -> ${dst_file}")
        configure_file("${src_file}" "${dst_file}" COPYONLY)
    endif()
endfunction()


# remove folder only if it exists
function(conditional_remove_directory folder)
    if(EXISTS "${folder}")
    message("Removing directory: ${folder}")
        file(REMOVE_RECURSE "${folder}")
    endif()
endfunction()

# copy directory only if the files are different
function(conditional_copy_directory src dst)

    file(GLOB_RECURSE source_files RELATIVE "${src}" "${src}/*")

    foreach(file IN LISTS source_files)
        set(src_file "${src}/${file}")
        set(dst_file "${dst}/${file}")

        if (EXISTS "${dst_file}")
            file(SHA256 "${src_file}" source_hash)
            file(SHA256 "${dst_file}" destination_hash)
            if (NOT source_hash STREQUAL destination_hash)
                message("Copying: ${src_file} -> ${dst_file}")
                configure_file("${src_file}" "${dst_file}" COPYONLY)
            endif()
        else()
            message("Copying: ${src_file} -> ${dst_file}")
            configure_file("${src_file}" "${dst_file}" COPYONLY)
        endif()
    endforeach()

endfunction()

