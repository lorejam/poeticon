set(appname activeSeg)

file(GLOB conf ${CMAKE_CURRENT_SOURCE_DIR}/conf/*.ini)
file(GLOB conf ${CMAKE_CURRENT_SOURCE_DIR}/conf/*.txt)
file(GLOB scripts ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.template)

yarp_install( FILES ${conf}
              DESTINATION ${ICUBCONTRIB_CONTEXTS_INSTALL_DIR}/${appname}
            )

yarp_install(FILES ${scripts}
             DESTINATION ${ICUBCONTRIB_APPLICATIONS_TEMPLATES_INSTALL_DIR})

