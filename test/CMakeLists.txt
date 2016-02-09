FILE (GLOB_RECURSE test_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)

# Add libraries here. Example: ${Boost_LIBRARIES}
FIND_PACKAGE (Boost)
FIND_PACKAGE (SFML COMPONENTS system window graphics network audio REQUIRED)

SET (test_LIBS ${PROJECT_LIBRARIES})
SET (test_BIN ${PROJECT_NAME}-unittests)

INCLUDE_DIRECTORIES("${gmock_SOURCE_DIR}/include"
                    "${gmock_SOURCE_DIR}"
                    "${gtest_SOURCE_DIR}/include"
                    # This directory is needed to build directly from Google
                    # Test sources.
                    "${gtest_SOURCE_DIR}"
                    "./../src/inc"
                    "./../src/inc/Components"
                    "./../src/inc/PainterComponents"
                    "./../src/inc/ProcessorComponents"
                    "./../src/inc/Ethertee")

SET (test_SRCS
  ${test_SRCS}
  # Any extra source files
  )

ADD_EXECUTABLE(${test_BIN} ${test_SRCS})
TARGET_LINK_LIBRARIES(${test_BIN} ${test_LIBS} gtest gtest_main gmock gmock_main)
TARGET_COMPILE_OPTIONS(${test_BIN} PRIVATE -Wnarrowing)

ADD_CUSTOM_TARGET(check ALL "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS})
ADD_CUSTOM_TARGET(unittests "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS})
