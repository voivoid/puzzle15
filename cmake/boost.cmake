include(ExternalProject)

find_package(Patch REQUIRED)

set(BoostVersion "1.77.0")
set(BoostSHA256 5347464af5b14ac54bb945dc68f1dd7c56f0dad7262816b956138fc53bcc0131)


if(WIN32)
  set(BoostBootstrapCmd cmd /C "<SOURCE_DIR>/bootstrap.bat")
  set(BoostB2 "<SOURCE_DIR>/b2.exe")
else()
  set(BoostBootstrapCmd $ENV{SHELL} "<SOURCE_DIR>/bootstrap.sh")
  set(BoostB2 "<SOURCE_DIR>/b2")
endif()

# TODO: use config.jam
# echo "using gcc : 4.4 : /usr/bin/g++-4.4 ; " >> tools/build/src/user-config.jam

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(BoostToolset "gcc")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(BoostToolset "clang")
endif()

if(BoostToolset)
  set(BoostBootstrapToolset "--with-toolset=${BoostToolset}")
  set(BoostB2Toolset "toolset=${BoostToolset}")
endif()

if(UNIX AND NOT "${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
  set(BoostBuildVariant "variant=debug")
endif()

if(MSVC)
  if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(BoostAddressModel "address-model=64")
    set(BoostAddressModelTag "-x64")
  else()
    set(BoostAddressModel "address-model=32")
    set(BoostAddressModelTag "-x32")
  endif()

  set(BoostLayout "--layout=tagged")
  set(BoostAsmFlags "asmflags=\\safeseh")
endif()

foreach(BoostLib ${BoostLibs})
  list(APPEND BoostLibsCmdLine "--with-${BoostLib}")
endforeach()

list(TRANSFORM BoostLibs REPLACE test unit_test_framework)

set(BoostByproducts ${BoostLibs})
list(TRANSFORM BoostByproducts PREPEND <SOURCE_DIR>/stage/lib/libboost_)

if(MSVC)
  set(BoostByproductsMt ${BoostByproducts})
  set(BoostByproductsMtGd ${BoostByproducts})
  list(TRANSFORM BoostByproductsMt APPEND -mt${BoostAddressModelTag}.lib)
  list(TRANSFORM BoostByproductsMtGd APPEND -mt-gd${BoostAddressModelTag}.lib)
  set(BoostByproducts ${BoostByproductsMt} ${BoostByproductsMtGd})
else()
  list(TRANSFORM BoostByproducts APPEND .a)
endif()

string(REPLACE . _ BoostVersionUnderscored ${BoostVersion})
ExternalProject_Add(
  get_boost
  PREFIX thirdparties
  URL "https://boostorg.jfrog.io/artifactory/main/release/${BoostVersion}/source/boost_${BoostVersionUnderscored}.tar.gz"
  URL_HASH SHA256=${BoostSHA256}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  BUILD_IN_SOURCE TRUE
  INSTALL_COMMAND ""
  BUILD_BYPRODUCTS ${BoostByproducts}
  )


ExternalProject_Get_Property(get_boost SOURCE_DIR)
set(BoostSrcDir ${SOURCE_DIR})
set(BoostLibDir ${BoostSrcDir}/stage/lib)

add_library(boost_headers INTERFACE)
add_library(Boost::boost ALIAS boost_headers)
set_target_properties(boost_headers PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${BoostSrcDir})
set_target_properties(boost_headers PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${BoostSrcDir})

if(MSVC)
  set_property(TARGET boost_headers PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_ALL_NO_LIB)
endif()