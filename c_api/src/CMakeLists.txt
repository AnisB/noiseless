cmake_minimum_required(VERSION 3.2)

# The defines we need for the library
set(NOISELESS_SDK_INCLUDE ${NOISELESS_SDK_ROOT}/include)
set(NOISELESS_CAPI_INCLUDE ${NOISELESS_CAPI_ROOT}/include)
set(NOISELESS_CAPI_SRC ${NOISELESS_CAPI_ROOT}/src)

bento_headers(header_files "${NOISELESS_CAPI_INCLUDE}" "c_api")
bento_sources(source_files "${NOISELESS_CAPI_SRC}" "c_api")

# Generate the static library
bento_dynamic_lib("noiseless_dylib" "c_api" "${header_files};${source_files};" "${NOISELESS_SDK_INCLUDE};${NOISELESS_3RD_LIBRARIES};${NOISELESS_CAPI_INCLUDE}")
target_link_libraries("noiseless_dylib" "noiseless_sdk" "bento_sdk" "${THIRD_PARTY_LIBRARIES}")