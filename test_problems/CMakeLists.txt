###################################################################################################
#                                                                                                 #
# This file is part of HPIPM.                                                                     #
#                                                                                                 #
# HPIPM -- High-Performance Interior Point Method.                                                #
# Copyright (C) 2017-2018 by Gianluca Frison.                                                     #
# Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              #
# All rights reserved.                                                                            #
#                                                                                                 #
# This program is free software: you can redistribute it and/or modify                            #
# it under the terms of the GNU General Public License as published by                            #
# the Free Software Foundation, either version 3 of the License, or                               #
# (at your option) any later version                                                              #.
#                                                                                                 #
# This program is distributed in the hope that it will be useful,                                 #
# but WITHOUT ANY WARRANTY; without even the implied warranty of                                  #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                                   #
# GNU General Public License for more details.                                                    #
#                                                                                                 #
# You should have received a copy of the GNU General Public License                               #
# along with this program.  If not, see <https://www.gnu.org/licenses/>.                          #
#                                                                                                 #
# The authors designate this particular file as subject to the "Classpath" exception              #
# as provided by the authors in the LICENSE file that accompained this code.                      #
#                                                                                                 #
# Author: Gianluca Frison, gianluca.frison (at) imtek.uni-freiburg.de                             #
#                                                                                                 #
###################################################################################################

add_executable(d_cond test_d_cond.c d_tools.c)
target_link_libraries(d_cond hpipm blasfeo m)

add_executable(d_part_cond test_d_part_cond.c d_tools.c)
target_link_libraries(d_part_cond hpipm blasfeo m)

add_executable(d_dense_qp test_d_dense.c)
target_link_libraries(d_dense_qp hpipm blasfeo m)

#add_executable(s_dense_qp test_s_dense.c)
#target_link_libraries(s_dense_qp hpipm blasfeo m)

add_executable(d_ocp_qp test_d_ocp.c d_tools.c)
target_link_libraries(d_ocp_qp hpipm blasfeo m)

#add_executable(s_ocp_qp test_s_ocp.c s_tools.c)
#target_link_libraries(s_ocp_qp hpipm blasfeo m)

#add_executable(m_ocp_qp test_m_ocp.c d_tools.c)
#target_link_libraries(m_ocp_qp hpipm blasfeo m)

add_executable(d_tree_ocp_qp test_d_tree_ocp.c d_tools.c)
target_link_libraries(d_tree_ocp_qp hpipm blasfeo m)

#add_executable(s_tree_ocp_qp test_s_tree_ocp.c s_tools.c)
#target_link_libraries(s_tree_ocp_qp hpipm blasfeo m)

