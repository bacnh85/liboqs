# SPDX-License-Identifier: MIT

# Only add liboqs Zephyr module if enabled in Kconfig
if(CONFIG_LIBOQS)

        # Workarounds for Zephyr
        if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
                # Workaround as the generic name "arm" is not a supported architecture in liboqs.
                # In Zephyr, however, it is exclusively used for 32-bit ARM architectures.
                set(CMAKE_SYSTEM_PROCESSOR "armv7")
        elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "riscv")
                # Zephyr doesn't distinguish between 32-bit and 64-bit RISC-V architectures.
                if(CONFIG_64BIT)
                        set(CMAKE_SYSTEM_PROCESSOR "riscv64")
                else()
                        set(CMAKE_SYSTEM_PROCESSOR "riscv32")
                endif()
        elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "posix")
                # Workaround to enable the native Zephyr builds on the Linux host system.
                if(BOARD MATCHES "native_posix|native_sim")
                        set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})
                else()
                        message(FATAL_ERROR "Unsupported board ${BOARD} with posix architecture")
                endif()
        elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
                # Zephyr doesn't distinguish between 32-bit and 64-bit x86 architectures.
                if(CONFIG_64BIT)
                        set(CMAKE_SYSTEM_PROCESSOR "x86_64")
                endif()
        endif()

        # We have to set CMAKE_SIZEOF_VOID_P manually as CMake can't detect it properly in Zephyr
        if(CONFIG_64BIT)
                set(CMAKE_SIZEOF_VOID_P 8)
        else()
                set(CMAKE_SIZEOF_VOID_P 4)
        endif()

        # Configuration for liboqs
        set(OQS_DIST_BUILD  OFF)
        set(OQS_BUILD_ONLY_LIB ON)
        set(OQS_USE_OPENSSL OFF)
        set(OQS_EMBEDDED_BUILD ON)

        set(CMAKE_CROSSCOMPILING ON)

        # Disable features by hand, as CMake won't find them properly with Zephyr
        set(CMAKE_HAVE_GETENTROPY OFF)
        set(CMAKE_HAVE_ALIGNED_ALLOC OFF)
        set(CMAKE_HAVE_POSIX_MEMALIGN OFF)
        set(CMAKE_HAVE_MEMALIGN OFF)
        set(CMAKE_HAVE_EXPLICIT_BZERO OFF)
        set(CMAKE_HAVE_MEMSET_S OFF)
        set(CC_SUPPORTS_WA_NOEXECSTACK OFF)
        set(LD_SUPPORTS_WL_Z_NOEXECSTACK OFF)

        # Algorithm selection (based on Kconfig)
        if(CONFIG_LIBOQS_ENABLE_KEM_BIKE)
                set(OQS_ENABLE_KEM_BIKE ON)
        else()
                set(OQS_ENABLE_KEM_BIKE OFF)
        endif()

        if(CONFIG_LIBOQS_ENABLE_KEM_FRODOKEM)
                set(OQS_ENABLE_KEM_FRODOKEM ON)
        else()
                set(OQS_ENABLE_KEM_FRODOKEM OFF)
        endif()

        if(CONFIG_LIBOQS_ENABLE_KEM_NTRUPRIME)
                set(OQS_ENABLE_KEM_NTRUPRIME ON)
        else()
                set(OQS_ENABLE_KEM_NTRUPRIME OFF)
        endif()

        if(CONFIG_LIBOQS_ENABLE_KEM_CLASSIC_MCELIECE)
                set(OQS_ENABLE_KEM_CLASSIC_MCELIECE ON)
        else()
                set(OQS_ENABLE_KEM_CLASSIC_MCELIECE OFF)
        endif()

        if(CONFIG_LIBOQS_ENABLE_KEM_HQC)
                set(OQS_ENABLE_KEM_HQC ON)
        else()
                set(OQS_ENABLE_KEM_HQC OFF)
        endif()

        if(CONFIG_LIBOQS_ENABLE_KEM_KYBER)
                set(OQS_ENABLE_KEM_KYBER ON)
        else()
                set(OQS_ENABLE_KEM_KYBER OFF)
        endif()

        if(CONFIG_LIBOQS_ENABLE_SIG_DILITHIUM)
                set(OQS_ENABLE_SIG_DILITHIUM ON)
        else()
                set(OQS_ENABLE_SIG_DILITHIUM OFF)
        endif()

        if(CONFIG_LIBOQS_ENABLE_SIG_FALCON)
                set(OQS_ENABLE_SIG_FALCON ON)
        else()
                set(OQS_ENABLE_SIG_FALCON OFF)
        endif()

        if(CONFIG_LIBOQS_ENABLE_SIG_SPHINCS)
                set(OQS_ENABLE_SIG_SPHINCS ON)
        else()
                set(OQS_ENABLE_SIG_SPHINCS OFF)
        endif()

        # Add the actual liboqs targets 
        add_subdirectory(.. build)

        # Add target specific options to all liboqs targets
        zephyr_get_targets(.. "STATIC_LIBRARY;OBJECT_LIBRARY" ALL_TARGETS)
        foreach(target ${ALL_TARGETS})
                # Zephyr include directories
                target_include_directories(${target} PRIVATE
                        $<TARGET_PROPERTY:zephyr_interface,INTERFACE_INCLUDE_DIRECTORIES>
                )

                # Zephyr system include directories
                target_include_directories(${target} SYSTEM PRIVATE
                        $<TARGET_PROPERTY:zephyr_interface,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>
                )

                # Definitions
                target_compile_definitions(${target} PRIVATE
                        $<TARGET_PROPERTY:zephyr_interface,INTERFACE_COMPILE_DEFINITIONS>
                )

                # Compile options (includes compiler flags)
                target_compile_options(${target} PRIVATE
                        $<TARGET_PROPERTY:zephyr_interface,INTERFACE_COMPILE_OPTIONS>
                        $<TARGET_PROPERTY:compiler,no_builtin>
                )

                # liboqs depends on unistd.h, which ultimately needs the generated syscall_list.h file,
                # which is generated as part of ${SYSCALL_LIST_H_TARGET} target. Therefore, we have to
                # make sure that target is built before liboqs.
                add_dependencies(${target} ${SYSCALL_LIST_H_TARGET})

                # We don't want position independent code
                set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE OFF)
        endforeach()

        # Link the liboqs library
        zephyr_link_libraries(oqs)

        # Include the liboqs headers
        zephyr_include_directories(${CMAKE_CURRENT_BINARY_DIR}/build/include)

        # Undo the Zephyr workarounds from above to not interfere with other modules
        if(CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7")
                set(CMAKE_SYSTEM_PROCESSOR "arm")
        elseif(CMAKE_SYSTEM_PROCESSOR EQUAL CMAKE_HOST_SYSTEM_PROCESSOR)
                set(CMAKE_SYSTEM_PROCESSOR "posix")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "riscv")
                set(CMAKE_SYSTEM_PROCESSOR "riscv")
        elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
                set(CMAKE_SYSTEM_PROCESSOR "x86")
        endif()
endif()