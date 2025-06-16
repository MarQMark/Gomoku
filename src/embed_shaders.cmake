function(embed_shaders_as_c_arrays TARGET SHADER_DIR)
    file(GLOB SHADER_FILES "${SHADER_DIR}/*.vert" "${SHADER_DIR}/*.frag")
    set(GENERATED_SOURCES "")
    set(DATA_HEADER "${CMAKE_BINARY_DIR}/shader_data.h")
    set(MAP_HEADER "${CMAKE_BINARY_DIR}/shader_assets.h")
    set(MAP_FILE "${CMAKE_BINARY_DIR}/shader_assets_map.cpp")

    file(WRITE ${DATA_HEADER} "#pragma once\n")
    file(APPEND ${DATA_HEADER} "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n")

    file(WRITE ${MAP_HEADER} "#pragma once\n#include \"shader_data.h\"\n#include <map>\n#include <string>\n\n")
    set(MAP_INIT "const std::map<std::string, std::pair<const unsigned char*, size_t>> shader_assets = {\n")

    foreach(SHADER_FILE ${SHADER_FILES})
        get_filename_component(FILENAME ${SHADER_FILE} NAME)
        get_filename_component(BASENAME_WE ${SHADER_FILE} NAME_WE)
        string(REPLACE "." "_" SYMBOL_NAME ${FILENAME})
        string(REPLACE "-" "_" SYMBOL_NAME ${SYMBOL_NAME})
        string(REGEX REPLACE "[^a-zA-Z0-9_]" "_" SYMBOL_NAME ${SYMBOL_NAME})

        set(C_FILE "${CMAKE_BINARY_DIR}/shader_${SYMBOL_NAME}.cpp")
        list(APPEND GENERATED_SOURCES ${C_FILE})

        file(READ "${SHADER_FILE}" BINARY_DATA HEX)
        string(REGEX MATCHALL ".." HEX_PAIRS "${BINARY_DATA}")
        set(FINAL_HEX "")
        foreach(BYTE ${HEX_PAIRS})
            string(APPEND FINAL_HEX "0x${BYTE}, ")
        endforeach()

        file(WRITE ${C_FILE}
                "// Auto-generated from ${FILENAME}
#include \"shader_data.h\"

const unsigned char ${SYMBOL_NAME}[] = {
  ${FINAL_HEX}
};
const unsigned int ${SYMBOL_NAME}_len = sizeof(${SYMBOL_NAME});
")

        file(APPEND ${DATA_HEADER}
                "extern const unsigned char ${SYMBOL_NAME}[];
extern const unsigned int ${SYMBOL_NAME}_len;\n")

        string(APPEND MAP_INIT "  { \"${FILENAME}\", { ${SYMBOL_NAME}, ${SYMBOL_NAME}_len } },\n")
    endforeach()

    string(APPEND MAP_INIT "};\n")

    file(APPEND ${DATA_HEADER} "\n#ifdef __cplusplus\n}\n#endif\n")
    file(APPEND ${MAP_HEADER} "extern const std::map<std::string, std::pair<const unsigned char*, size_t>> shader_assets;\n")
    file(WRITE ${MAP_FILE} "#include \"shader_assets.h\"\n\n${MAP_INIT}")

    list(APPEND GENERATED_SOURCES ${MAP_FILE})

    target_sources(${TARGET} PRIVATE ${GENERATED_SOURCES})
    target_include_directories(${TARGET} PRIVATE ${CMAKE_BINARY_DIR})

    add_custom_target(embed_shader_assets_header DEPENDS ${DATA_HEADER} ${MAP_HEADER} ${MAP_FILE})
    add_dependencies(${TARGET} embed_shader_assets_header)
endfunction()
