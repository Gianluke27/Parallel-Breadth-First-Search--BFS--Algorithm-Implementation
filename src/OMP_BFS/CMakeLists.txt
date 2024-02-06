#[[
 * Course: High Performance Computing 2021/2022
 * 
 * Lecturer: Francesco Moscato    fmoscato@unisa.it
 *
 *
 * Copyright (C) 2021 - All Rights Reserved
 *
 * This file is part of Contest-OMP.
 *
 * Contest-OMP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Contest-OMP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Contest-OMP.  If not, see http://www.gnu.org/licenses/.
]]


# ____________________________________ SETTINGS _________________________________________
cmake_minimum_required(VERSION 3.9)

project(CountingSort
	LANGUAGES C
)
enable_testing()

# ______________________________________ MAIN ___________________________________________
# Modify this to compile needed files
set(MainSrc "")
set(Optimizations "O0" "O1" "O2" "O3")
set(MainNames "main_bfs" "main_bfs_parallel")
# ---------------------------------------------------------------------------------------

foreach(Opt IN LISTS Optimizations)
    foreach(MainName IN LISTS MainNames)
        # Create Variables
        string(CONCAT File ${MainName} ".c")
        string(CONCAT MainNameOpt ${MainName} ${Opt})

        # Generate the exe
        add_executable(${MainNameOpt} ${File})

		# Link the library "math"
		target_link_libraries(${MainNameOpt} m)   

        # Link OpenMp if it has been found
        find_package(OpenMP)
        if(OpenMP_C_FOUND)
            target_link_libraries(${MainNameOpt} OpenMP::OpenMP_C)
        endif()

        # Add optimization options
        target_compile_options(${MainNameOpt} PRIVATE "-${Opt}")
    endforeach()
endforeach()

add_executable("generate_graph" "generate_graph.c")
target_link_libraries("generate_graph" m) 

add_executable("graph_bfs_test" "graph_bfs_test.c")
target_link_libraries("graph_bfs_test" m)
# _______________________________________________________________________________________

add_custom_target(
    generate_output

    # Generate the exe
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/file.bash ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Inizio Misure"

    foreach(Opt IN LISTS Optimizations)
        foreach(MainName IN LISTS MainNames)
            # Create Variables
            
            string(CONCAT MainNameOpt ${MainName} ${Opt})
            MainNameOpt
            
        endforeach()
    endforeach()
        
)