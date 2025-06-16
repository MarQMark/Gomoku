function(embed_assets_as_c_arrays TARGET ASSET_DIR)
    file(GLOB ASSET_FILES "${ASSET_DIR}/*.png")  # Only .png files
    set(GENERATED_SOURCES "")
    set(HEADER_FILE "${CMAKE_BINARY_DIR}/embedded_assets.h")
    set(MAP_FILE "${CMAKE_BINARY_DIR}/embedded_assets_map.cpp")

    file(WRITE ${HEADER_FILE} "#pragma once\n")
    file(APPEND ${HEADER_FILE} "#include <cstddef>\n#include <map>\n#include <string>\n\n")

    set(MAP_INIT "const std::map<std::string, std::pair<const unsigned char*, size_t>> embedded_assets = {\n")

    foreach(ASSET_FILE ${ASSET_FILES})
        get_filename_component(FILENAME ${ASSET_FILE} NAME)
        get_filename_component(BASENAME_WE ${ASSET_FILE} NAME_WE) # name without extension
        string(REPLACE "." "_" SYMBOL_NAME ${FILENAME})
        string(REPLACE "-" "_" SYMBOL_NAME ${SYMBOL_NAME})
        string(REGEX REPLACE "[^a-zA-Z0-9_]" "_" SYMBOL_NAME ${SYMBOL_NAME})

        set(C_FILE "${CMAKE_BINARY_DIR}/embedded_${SYMBOL_NAME}.cpp")
        list(APPEND GENERATED_SOURCES ${C_FILE})

        # Read binary and format to hex
        file(READ "${ASSET_FILE}" BINARY_DATA HEX)
        string(REGEX MATCHALL ".." HEX_PAIRS "${BINARY_DATA}")
        set(FINAL_HEX "")
        foreach(BYTE ${HEX_PAIRS})
            string(APPEND FINAL_HEX "0x${BYTE}, ")
        endforeach()

        file(WRITE ${C_FILE}
                "// Auto-generated from ${FILENAME}
#include \"embedded_assets.h\"

const unsigned char ${SYMBOL_NAME}[] = {
  ${FINAL_HEX}
};
const unsigned int ${SYMBOL_NAME}_len = sizeof(${SYMBOL_NAME});
")

        file(APPEND ${HEADER_FILE}
                "extern const unsigned char ${SYMBOL_NAME}[];
extern const unsigned int ${SYMBOL_NAME}_len;\n")

        string(APPEND MAP_INIT "  { \"${BASENAME_WE}\", { ${SYMBOL_NAME}, ${SYMBOL_NAME}_len } },\n")
    endforeach()

    string(APPEND MAP_INIT "};\n")

    file(APPEND ${HEADER_FILE} "\nextern const std::map<std::string, std::pair<const unsigned char*, size_t>> embedded_assets;\n")
    file(WRITE ${MAP_FILE} "#include \"embedded_assets.h\"\n\n${MAP_INIT}")

    list(APPEND GENERATED_SOURCES ${MAP_FILE})

    target_sources(${TARGET} PRIVATE ${GENERATED_SOURCES})
    target_include_directories(${TARGET} PRIVATE ${CMAKE_BINARY_DIR})

    add_custom_target(embed_assets_header DEPENDS ${HEADER_FILE} ${MAP_FILE})
    add_dependencies(${TARGET} embed_assets_header)
endfunction()
