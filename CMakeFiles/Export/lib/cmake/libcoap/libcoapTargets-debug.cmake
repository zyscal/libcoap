#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libcoap::coap-3" for configuration "Debug"
set_property(TARGET libcoap::coap-3 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libcoap::coap-3 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libcoap-3.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS libcoap::coap-3 )
list(APPEND _IMPORT_CHECK_FILES_FOR_libcoap::coap-3 "${_IMPORT_PREFIX}/lib/libcoap-3.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
