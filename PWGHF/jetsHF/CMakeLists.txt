# **************************************************************************
# * Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
# *                                                                        *
# * Author: The ALICE Off-line Project.                                    *
# * Contributors are mentioned in the code where appropriate.              *
# *                                                                        *
# * Permission to use, copy, modify and distribute this software and its   *
# * documentation strictly for non-commercial purposes is hereby granted   *
# * without fee, provided that the above copyright notice appears in all   *
# * copies and that both the copyright notice and this permission notice   *
# * appear in the supporting documentation. The authors make no claims     *
# * about the suitability of this software for any purpose. It is          *
# * provided "as is" without express or implied warranty.                  *
# **************************************************************************

# Module
set(MODULE PWGHFjetsHF)
add_definitions(-D_MODULE_="${MODULE}")

# Module include folder
include_directories(${AliPhysics_SOURCE_DIR}/PWGHF/jetsHF)

# Additional include folders in alphabetical order except ROOT
include_directories(${ROOT_INCLUDE_DIRS}
                    ${AliPhysics_SOURCE_DIR}/CORRFW
                    ${AliPhysics_SOURCE_DIR}/OADB
                    ${AliPhysics_SOURCE_DIR}/OADB/COMMON/MULTIPLICITY
                    ${AliRoot_SOURCE_DIR}/ANALYSIS/ANALYSIS		
                    ${AliRoot_SOURCE_DIR}/ANALYSIS/ANALYSISalice	
                    ${AliRoot_SOURCE_DIR}/EMCAL/EMCALbase
                    ${AliRoot_SOURCE_DIR}/EMCAL/EMCALUtils
                    ${AliPhysics_SOURCE_DIR}/PWG/EMCAL/EMCALbase
                    ${AliPhysics_SOURCE_DIR}/PWG/Tools
                    ${AliPhysics_SOURCE_DIR}/PWG/JETFW
                    ${AliPhysics_SOURCE_DIR}/PWGGA/Common
                    ${AliPhysics_SOURCE_DIR}/PWGGA/GammaConvBase
                    ${AliPhysics_SOURCE_DIR}/PWGGA/GammaConv
                    ${AliPhysics_SOURCE_DIR}/PWGLF/STRANGENESS/Cascades/Run2
                    ${AliRoot_SOURCE_DIR}/PYTHIA6/pythia6
                    ${AliPhysics_SOURCE_DIR}/PWG/JETFW
                    ${AliPhysics_SOURCE_DIR}/PWGJE/EMCALJetTasks
                    ${AliPhysics_SOURCE_DIR}/PWGHF/vertexingHF
                    ${AliPhysics_SOURCE_DIR}/PWGHF/vertexingHF/charmFlow
                    ${AliPhysics_SOURCE_DIR}/PWGPP/EVCHAR/FlowVectorCorrections/QnCorrections
                    ${AliPhysics_SOURCE_DIR}/PWGPP/EVCHAR/FlowVectorCorrections/QnCorrectionsInterface
                    )



# Sources in alphabetical order
set(SRCS
    AliHFJetsTagging.cxx
    AliHFJetTaggingIP.cxx
    AliAnalysisHFjetTagHFE.cxx
    AliAnalysisTaskEmcalHFCJQA.cxx	    
    AliAnalysisTaskEmcalHFeJetCorrel.cxx		    
    AliAnalysisTaskEmcalJetBJetTaggingIP.cxx
    AliRDHFJetsCuts.cxx
    AliRDHFJetsCutsVertex.cxx
    AliHFJetsTaggingVertex.cxx
    AliHFJetsContainer.cxx
    AliHFJetsContainerVertex.cxx
    AliDJetRawYieldUncertainty.cxx
    AliAnalysisTaskDJetCorrelations.cxx
    AliAnalysisTaskDJetCorrelationsQA.cxx
    AliAnalysisTaskDmesonsFilterCJ.cxx
    AliEmbeddingEventForHFTask.cxx
    AliAnalysisTaskIPCalib.cxx
)

if(FASTJET_FOUND)
    set(SRCS ${SRCS}
    AliAnalysisTaskEmcalJetBtagSV.cxx          
    AliAnalysisTaskBJetTC.cxx		
    AliATDJetCorrDev.cxx
    )


    include_directories(#${AliPhysics_SOURCE_DIR}/HLT/FJWrapper
                        ${AliPhysics_SOURCE_DIR}/JETAN/JETAN
                       )
    include_directories(SYSTEM ${FASTJET_INCLUDE_DIR})
    link_directories(${FASTJET_LIBS_DIR})
    add_definitions(${FASTJET_DEFINITIONS})

endif(FASTJET_FOUND)

# Headers from sources
string(REPLACE ".cxx" ".h" HDRS "${SRCS}")
list (APPEND HDRS AliHFJetsUtils.h)

if(FASTJET_FOUND)
  list(APPEND HDRS ../../PWGJE/EMCALJetTasks/FJ_includes.h)
endif(FASTJET_FOUND)

# Generate the dictionary
# It will create G_ARG1.cxx and G_ARG1.h / ARG1 = function first argument
get_directory_property(incdirs INCLUDE_DIRECTORIES)
generate_dictionary("${MODULE}" "${MODULE}LinkDef.h" "${HDRS}" "${incdirs}" "${FASTJET_ROOTDICT_OPTS}")

set(ROOT_DEPENDENCIES Core EG Gpad Hist MathCore Physics Tree)
set(ALIROOT_DEPENDENCIES ANALYSIS ANALYSISalice AOD ESD PWGJETFW STEERBase PWGTools PWGHFvertexingHF)

# Generate the ROOT map
# Dependecies
set(LIBDEPS ${ALIROOT_DEPENDENCIES} ${ROOT_DEPENDENCIES})
generate_rootmap("${MODULE}" "${LIBDEPS}" "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}LinkDef.h")

# Generate a PARfile target for this library
add_target_parfile(${MODULE} "${SRCS}" "${HDRS}" "${MODULE}LinkDef.h" "${LIBDEPS}")


# Create an object to be reused in case of static libraries 
# Otherwise the sources will be compiled twice
add_library(${MODULE}-object OBJECT ${SRCS} G__${MODULE}.cxx)
# Add a library to the project using the object
add_library_tested(${MODULE} SHARED $<TARGET_OBJECTS:${MODULE}-object>)
target_link_libraries(${MODULE} ${ALIROOT_DEPENDENCIES} ${ROOT_DEPENDENCIES} ${FASTJET_LIBS})

# Setting the correct headers for the object as gathered from the dependencies
target_include_directories(${MODULE}-object PUBLIC $<TARGET_PROPERTY:${MODULE},INCLUDE_DIRECTORIES>)
set_target_properties(${MODULE}-object PROPERTIES COMPILE_DEFINITIONS $<TARGET_PROPERTY:${MODULE},COMPILE_DEFINITIONS>)

# Public include folders that will be propagated to the dependecies
target_include_directories(${MODULE} PUBLIC ${incdirs})

set(MODULE_COMPILE_FLAGS)
set(MODULE_LINK_FLAGS)

if(DATE_FOUND)
    set(MODULE_COMPILE_FLAGS "${DATE_CFLAGS}")
    set(MODULE_LINK_FLAGS "${DATE_LDFLAGS} ${DATE_LIBS}")
endif(DATE_FOUND)

# Additional compilation and linking flags
set(MODULE_COMPILE_FLAGS " ${MODULE_COMPILE_FLAGS}")

# System dependent: Modify the way the library is build
if(${CMAKE_SYSTEM} MATCHES Darwin)
    set(MODULE_LINK_FLAGS "-undefined dynamic_lookup ${MODULE_LINK_FLAGS}")
endif(${CMAKE_SYSTEM} MATCHES Darwin)

# Setting compilation flags for the object
set_target_properties(${MODULE}-object PROPERTIES COMPILE_FLAGS "${MODULE_COMPILE_FLAGS}")
# Setting the linking flags for the library
set_target_properties(${MODULE} PROPERTIES LINK_FLAGS "${MODULE_LINK_FLAGS}")

# Installation
install(TARGETS ${MODULE}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

install(FILES ${HDRS} DESTINATION include)

# Installing the macros
install (DIRECTORY macros DESTINATION PWGHF/jetsHF)
