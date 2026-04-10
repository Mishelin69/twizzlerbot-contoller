idf_component_register(
    SRCS
        src/nanopb/pb_common.c
        src/nanopb/pb_decode.c
        src/nanopb/pb_encode.c

        src/protodefs/MotorData.pb.c

    INCLUDE_DIRS
        include/
    REQUIRES 
)
