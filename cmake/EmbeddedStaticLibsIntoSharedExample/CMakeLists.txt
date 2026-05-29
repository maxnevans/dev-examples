cmake_minimum_required(VERSION 3.28)
project(EmbeddedStaticLibsIntoSharedExample CXX)

# This standard is not necessary, though in example the coded uses std::ranges 
set(CMAKE_CXX_STANDARD 20)

include(GenerateExportHeader)

# Just wrapped into a function to avoid boilerplate and increase readability of essence
function(AddObjectLibrary Name EndLibType EndUserOfObjects)
  add_library(${Name} OBJECT)

  generate_export_header(${Name} BASE_NAME ${Name} EXPORT_MACRO_NAME ${Name}_API EXPORT_FILE_NAME GeneratedSources/${Name}API.h)
  target_include_directories(${Name} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/GeneratedSources")
  target_sources(${Name} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/GeneratedSources/${Name}API.h")

  # First part of the magic: because of transitive inheritance, everyone gonna link against what we
  # supply here. The important thing here is $<TARGET_OBJECTS:${Name}>. But we can't add it to everything 
  # that is going to include dependent library (gobby in that example) because of the symbol duplication. 
  # So we have to add it only to the end user (consumer library/executable) of objects.
  target_link_libraries(${Name}
    INTERFACE
      $<$<STREQUAL:$<TARGET_PROPERTY:NAME>,${EndUserOfObjects}>:$<TARGET_OBJECTS:${Name}>>
  )

  # generate_export_header doesn't add compile definitions to the target, add them ourselves
  if (EndLibType STREQUAL SHARED)
    target_compile_definitions(${Name} 
      PRIVATE 
        ${Name}_EXPORTS
      INTERFACE
        # This doesn't work, the idea was if consumer of our library has definition COMPILING_AS_OBJECT
        # then we add our definition ${Name}_EXPORTS also. In this way the consumer shouldn't
        # worry about all OBJECT libraries it directly or indirectly depends on. In particular this
        # is the case because the consumer includes our headers in its implementation.
        #$<$<IN_LIST:COMPILING_AS_OBJECT,$<TARGET_PROPERTY:COMPILE_DEFINITIONS>>:${Name}_EXPORTS>

        # But this with target property works.
        $<$<BOOL:$<TARGET_PROPERTY:COMPILE_AS_OBJECT>>:${Name}_EXPORTS>
  )
  elseif(EndLibType STREQUAL STATIC)
    string(TOUPPER ${Name} UpperCaseName)
    target_compile_definitions(${Name} PUBLIC ${UpperCaseName}_STATIC_DEFINE)
  endif()

  target_sources(${Name}
    PUBLIC
      src/libs/${Name}/${Name}.h
    PRIVATE
      src/libs/${Name}/${Name}.cpp
  )

  target_include_directories(${Name}
    PUBLIC
      src/libs/${Name}
  )
endfunction()

# Overall hierarchy:
#
# Main -> baz -> bar
#             -> gobby -> foo
# 
# baz can be set to STATIC or SHARED. If set to SHARED, all dependencies baked into it.
# if set to STATIC, it's impossible to bake dependencies into it (only if you directly add sources of deps
# to compilation of baz), so the end user of object libraries changes to the user of static library baz - main.
#

# You can set this to STATIC or to SHARED, or set it via cache variables, 
# or during configuration like -DBUILD_SHARED_LIBS=ON or -DBUILD_SHARED_LIBS=OFF
set(EndLibraryType SHARED)
if(DEFINED BUILD_SHARED_LIBS)
  if(BUILD_SHARED_LIBS)
    set(EndLibraryType SHARED)
  else()
    set(EndLibraryType STATIC)
  endif()
endif()

# End user changes because .lib cant consume object libraries itself
if(EndLibraryType STREQUAL STATIC)
  set(EndUserOfObjects main)
elseif(EndLibraryType STREQUAL SHARED)
  set(EndUserOfObjects baz)
endif()

AddObjectLibrary(foo ${EndLibraryType} ${EndUserOfObjects})
AddObjectLibrary(bar ${EndLibraryType} ${EndUserOfObjects})
AddObjectLibrary(gobby ${EndLibraryType} ${EndUserOfObjects})

# directories and sources transitively don't go outside of gobby but foo.obj is going to be linked against baz.dll 
# (though baz.dll can't use foo.h, like main.exe as well).
target_link_libraries(gobby PRIVATE foo)

# We also have to duplicate foo_EXPORTS to compile gobby against foo correctly. In case gobby itself includes headers of
# foo, it's going to have them dllimport, and as both gobby and foo are objects that going to be embedded into a single
# unit (baz in case it's SHARED, main in case baz STATIC), we gonna have both dllexport and dllimport declarations in
# the same unit and thus linker warning.
#target_compile_definitions(gobby PRIVATE foo_EXPORTS)

# This clever approach instead of listing all export macros of dependencies we use inside gobby doesn't work.
#target_compile_definitions(gobby PRIVATE COMPILING_AS_OBJECT)

# Well, this approach with target properties works.
set_target_properties(gobby PROPERTIES COMPILE_AS_OBJECT TRUE)

# This library is either SHARED or STATIC for our example.
add_library(baz ${EndLibraryType} src/libs/baz/baz.cpp)
target_link_libraries(baz PUBLIC gobby bar)

# Finally this our consumer of baz, it doesn't link against other libs though can use all of their functionality
# either through baz.dll (in case it's SHARED) or directly (in case baz is STATIC).
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE baz)