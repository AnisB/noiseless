cmake_minimum_required(VERSION 3.2)

# The defines we need for the library
set(NOISELESS_SDK_INCLUDE ${NOISELESS_SDK_ROOT}/include)
set(NOISELESS_SDK_SOURCE ${NOISELESS_SDK_ROOT}/src)

sub_directory_list(sub_projects_headers "${NOISELESS_SDK_INCLUDE}")
foreach(header_dir ${sub_projects_headers})
	bento_headers(tmp_header_list "${NOISELESS_SDK_INCLUDE}/${header_dir}" "${header_dir}")
	list(APPEND header_files "${tmp_header_list}")
endforeach()

sub_directory_list(sub_projects_sources "${NOISELESS_SDK_SOURCE}")
foreach(source_dir ${sub_projects_sources})
	bento_sources(tmp_source_list "${NOISELESS_SDK_SOURCE}/${source_dir}" "${source_dir}")
	list(APPEND source_files "${tmp_source_list}")
endforeach()

# Generate the static library
bento_static_lib("noiseless_sdk" "sdk" "${header_files};${source_files};" "${NOISELESS_SDK_INCLUDE};${NOISELESS_3RD_LIBRARIES};${BENTO_SDK_ROOT}/include;${OpenCL_INCLUDE_DIRS};")