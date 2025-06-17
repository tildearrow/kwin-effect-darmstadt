kcoreaddons_add_plugin(darmstadt INSTALL_NAMESPACE "kwin/effects/plugins/")
target_sources(darmstadt PRIVATE
    darmstadt.cpp
    main.cpp
)

target_link_libraries(darmstadt
    KF6::CoreAddons
    KF6::I18n
    
    KWin::kwin
)
